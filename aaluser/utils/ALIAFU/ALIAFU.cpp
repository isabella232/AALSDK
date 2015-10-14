// Copyright (c) 2015, Intel Corporation
//
// Redistribution  and  use  in source  and  binary  forms,  with  or  without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of  source code  must retain the  above copyright notice,
//   this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// * Neither the name  of Intel Corporation  nor the names of its contributors
//   may be used to  endorse or promote  products derived  from this  software
//   without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,  BUT NOT LIMITED TO,  THE
// IMPLIED WARRANTIES OF  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT  SHALL THE COPYRIGHT OWNER  OR CONTRIBUTORS BE
// LIABLE  FOR  ANY  DIRECT,  INDIRECT,  INCIDENTAL,  SPECIAL,  EXEMPLARY,  OR
// CONSEQUENTIAL  DAMAGES  (INCLUDING,  BUT  NOT LIMITED  TO,  PROCUREMENT  OF
// SUBSTITUTE GOODS OR SERVICES;  LOSS OF USE,  DATA, OR PROFITS;  OR BUSINESS
// INTERRUPTION)  HOWEVER CAUSED  AND ON ANY THEORY  OF LIABILITY,  WHETHER IN
// CONTRACT,  STRICT LIABILITY,  OR TORT  (INCLUDING NEGLIGENCE  OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,  EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//****************************************************************************
/// @file ALIAFU.cpp
/// @brief Implementation of ALI AFU Service.
/// @ingroup ALIAFU
/// @verbatim
/// Intel(R) QuickAssist Technology Accelerator Abstraction Layer Sample Application
///
///    This application is for example purposes only.
///    It is not intended to represent a model for developing commercially-deployable applications.
///    It is designed to show working examples of the AAL programming model and APIs.
///
/// AUTHORS: Henry Mitchel, Intel Corporation
///          Joseph Grecco, Intel Corporation
///          
/// HISTORY:
/// WHEN:          WHO:     WHAT:
/// 07/20/2015     HM       Initial version.@endverbatim
//****************************************************************************
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif // HAVE_CONFIG_H

#include <aalsdk/AAL.h>
#include <aalsdk/AALLoggerExtern.h>
#include <aalsdk/aas/AALInProcServiceFactory.h>
#include <aalsdk/service/HWALIAFUService.h>
#include <aalsdk/service/ASEALIAFUService.h>
#include <aalsdk/service/SWSimALIAFUService.h>
#include <aalsdk/Dispatchables.h>

#include "ALIAFU.h"

BEGIN_NAMESPACE(AAL)

/// @addtogroup ALIAFU
/// @{

btBool ALIAFU::init( IBase *pclientBase,
                     NamedValueSet const &optArgs,
                     TransactionID const &TranID)
{
   ICCIClient *pClient = dynamic_ptr<ICCIClient>(iidCCIClient, getServiceClientBase());
   ASSERT( NULL != pClient );
   if(NULL == pClient){
      /// ObjectCreatedExceptionEvent Constructor.
     initFailed(new CExceptionTransactionEvent( this,
                                                 TranID,
                                                 errBadParameter,
                                                 reasMissingInterface,
                                                 "Client did not publish ICCIClient Interface"));
      return false;
   }

   // Default target is hardware AFU.
   NamedValueSet manifest(std::string(HWALIAFU_MANIFEST));

   // Determine which AFU implementation has been requested (HW, ASE, or SW Sim).
   if ( m_optArgs.Has(ALIAFU_NVS_KEY_TARGET) ) {

      m_optArgs.Get(ALIAFU_NVS_KEY_TARGET, &m_TargetAFU);
      ASSERT(NULL != m_TargetAFU);

      if ( 0 == std::string(m_TargetAFU).compare(ALIAFU_NVS_VAL_TARGET_ASE) ) {
         manifest = NamedValueSet(std::string(ASEALIAFU_MANIFEST));
      } else if ( 0 == std::string(m_TargetAFU).compare(ALIAFU_NVS_VAL_TARGET_SWSIM) ) {
         manifest = NamedValueSet(std::string(SWSIMALIAFU_MANIFEST));
      }
   }

   // TODO Use wrap/unwrap utils.
   m_TranIDFrominit = TranID;
   allocService(dynamic_ptr<IBase>(iidBase, this), manifest, TransactionID());
   return true;
}

btBool ALIAFU::Release(TransactionID const &TranID, btTime timeout)
{
   if ( NULL != m_pDelegate ) {
      dynamic_cast<IAALService *>(m_pDelegate)->Release(TransactionID(), timeout);
      m_TranIDFromRelease  = TranID;
      m_TimeoutFromRelease = timeout;
      return true;
   }

   return ServiceBase::Release(TranID, timeout);
}

btBool ALIAFU::Release(btTime timeout)
{
   if ( NULL != m_pDelegate ) {
      dynamic_cast<IAALService *>(m_pDelegate)->Release(timeout);
      m_pDelegate = NULL;
      return true;
   }

   return ServiceBase::Release(timeout);
}


void ALIAFU::serviceAllocated(IBase               *pServiceBase,
                                TransactionID const &TranID)
{
   m_pDelegate = dynamic_ptr<IALIAFU>(iidALIAFU, pServiceBase);
   ASSERT(NULL != m_pDelegate);

   if ( NULL != m_TargetAFU ) {
      if ( 0 == std::string(m_TargetAFU).compare(ALIAFU_NVS_VAL_TARGET_FPGA) ) {
         SetInterface(iidHWALIAFU, dynamic_ptr<IALIAFU>(iidHWALIAFU, pServiceBase));
      } else if ( 0 == std::string(m_TargetAFU).compare(ALIAFU_NVS_VAL_TARGET_ASE) ) {
         SetInterface(iidASEALIAFU, dynamic_ptr<IALIAFU>(iidASEALIAFU, pServiceBase));
      } else if ( 0 == std::string(m_TargetAFU).compare(ALIAFU_NVS_VAL_TARGET_SWSIM) ) {
         SetInterface(iidSWSIMALIAFU, dynamic_ptr<IALIAFU>(iidSWSIMALIAFU, pServiceBase));
      } else {
         ASSERT(false); // unsupported target AFU
      }
   } else { // default to hardware
      SetInterface(iidHWALIAFU, dynamic_ptr<IALIAFU>(iidHWALIAFU, pServiceBase));
   }

   initComplete(m_TranIDFrominit);
}

void ALIAFU::serviceAllocateFailed(const IEvent &Event)
{
   // Reflect the error to the outer client.
// TODO extract the Exception info and put in this event
   initFailed( new(std::nothrow) CExceptionTransactionEvent( NULL,
                                                             m_TranIDFrominit,
                                                             errInternal,
                                                             reasCauseUnknown,
                                                             "Allocate Failed") );
}

void ALIAFU::serviceReleased(TransactionID const &TranID)
{
   m_pDelegate = NULL;
   ServiceBase::Release(m_TranIDFromRelease, m_TimeoutFromRelease);
}

void ALIAFU::serviceReleaseFailed(const IEvent &Event)
{
   // Reflect the error to the outer client.
// TODO extract the Exception info and put in this event
   getRuntime()->schedDispatchable( new ServiceClientCallback( ServiceClientCallback::AllocateFailed,
                                                               getServiceClient(),
                                                               getRuntimeClient(),
                                                               NULL,
                                                               new CExceptionTransactionEvent( NULL,
                                                                                               m_TranIDFrominit,
                                                                                               errInternal,
                                                                                               reasCauseUnknown,
                                                                                               "Release Failed")));
}

void ALIAFU::serviceEvent(const IEvent &Event)
{
   // Reflect the message to the outer client.
   getRuntime()->schedDispatchable( new(std::nothrow) ServiceClientCallback(ServiceClientCallback::Event,
                                                                            getServiceClient(),
                                                                            getRuntimeClient(),
                                                                            dynamic_cast<IBase *>(this),
                                                                            &Event) );
}


// We delegate our IALIAFU interface to the chosen implementation strategy.
void ALIAFU::WorkspaceAllocate(btWSSize             Length,
                                 TransactionID const &TranID)
{
   m_pDelegate->WorkspaceAllocate(Length, TranID);
}

void ALIAFU::WorkspaceFree(btVirtAddr           Address,
                             TransactionID const &TranID)
{
   m_pDelegate->WorkspaceFree(Address, TranID);
}

btBool ALIAFU::CSRRead(btCSROffset CSR,
                         btCSRValue *pValue)
{
   return m_pDelegate->CSRRead(CSR, pValue);
}

btBool ALIAFU::CSRWrite(btCSROffset CSR,
                          btCSRValue  Value)
{
   return m_pDelegate->CSRWrite(CSR, Value);
}

btBool ALIAFU::CSRWrite64(btCSROffset CSR,
                            bt64bitCSR  Value)
{
   return m_pDelegate->CSRWrite64(CSR, Value);
}

void ALIAFU::OnWorkspaceAllocated(TransactionID const &TranID,
                                    btVirtAddr           WkspcVirt,
                                    btPhysAddr           WkspcPhys,
                                    btWSSize             WkspcSize)
{
   // Reflect the message to the outer client.
   getRuntime()->schedDispatchable( new(std::nothrow) CCIClientWorkspaceAllocated(dynamic_ptr<ICCIClient>(iidCCIClient, getServiceClientBase()),
                                                                                  TranID,
                                                                                  WkspcVirt,
                                                                                  WkspcPhys,
                                                                                  WkspcSize) );
}

void ALIAFU::OnWorkspaceAllocateFailed(const IEvent &Event)
{
   // Reflect the message to the outer client.
   ICCIClient *pClient = dynamic_ptr<ICCIClient>(iidCCIClient, getServiceClientBase());
   ASSERT(NULL != pClient);
   if ( NULL != pClient ) {
      pClient->OnWorkspaceAllocateFailed(Event);
   }
}

void ALIAFU::OnWorkspaceFreed(TransactionID const &TranID)
{
   // Reflect the message to the outer client.
   getRuntime()->schedDispatchable( new(std::nothrow) CCIClientWorkspaceFreed(dynamic_ptr<ICCIClient>(iidCCIClient, getServiceClientBase()),
                                                                              TranID) );
}

void ALIAFU::OnWorkspaceFreeFailed(const IEvent &Event)
{
   // Reflect the message to the outer client.
   ICCIClient *pClient = dynamic_ptr<ICCIClient>(iidCCIClient, getServiceClientBase());
   ASSERT(NULL != pClient);
   if ( NULL != pClient ) {
      pClient->OnWorkspaceFreeFailed(Event);
   }
}

/// @}

END_NAMESPACE(AAL)


#if defined( __AAL_WINDOWS__ )

BOOL APIENTRY DllMain(HANDLE hModule,
                      DWORD  ul_reason_for_call,
                      LPVOID lpReserved)
{
   switch ( ul_reason_for_call ) {
      case DLL_PROCESS_ATTACH :
         break;
      case DLL_THREAD_ATTACH  :
         break;
      case DLL_THREAD_DETACH  :
         break;
      case DLL_PROCESS_DETACH :
         break;
   }
   return TRUE;
}

#endif // __AAL_WINDOWS__


#define SERVICE_FACTORY AAL::InProcSvcsFact< AAL::ALIAFU >

#if defined ( __AAL_WINDOWS__ )
# pragma warning(push)
# pragma warning(disable : 4996) // destination of copy is unsafe
#endif // __AAL_WINDOWS__

ALIAFU_BEGIN_SVC_MOD(SERVICE_FACTORY)
   /* No commands other than default, at the moment. */
ALIAFU_END_SVC_MOD()

#if defined ( __AAL_WINDOWS__ )
# pragma warning(pop)
#endif // __AAL_WINDOWS__
