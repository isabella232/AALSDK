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
/// @file HWALIAFU.h
/// @brief Definitions for ALI Hardware AFU Service.
/// @ingroup HWALIAFU
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
///
/// HISTORY:
/// WHEN:          WHO:     WHAT:
/// 07/20/2015     HM       Initial version.@endverbatim
//****************************************************************************
#ifndef __HWALIAFU_H__
#define __HWALIAFU_H__
#include <aalsdk/AALLoggerExtern.h>
#include <aalsdk/service/ALIAFUService.h>
#include <aalsdk/service/HWALIAFUService.h>
#include <aalsdk/aas/AALDeviceService.h>
#include <aalsdk/service/IALIAFU.h>
#include <aalsdk/faptrans/FAP10.h>              // WkSp_Single_Allocate_AFUTransaction
												// and WorkSpaceMapper
#include <aalsdk/utils/AALEventUtilities.h>     // UnWrapTransactionIDFromEvent
#include <aalsdk/uaia/AALuAIA_UIDriverClient.h> // IUIDriverClientEvent
#include <aalsdk/uaia/IAFUProxy.h>

BEGIN_NAMESPACE(AAL)

/// @addtogroup HWALIAFU
/// @{

#if defined ( __AAL_WINDOWS__ )
# pragma warning(push)           // ignoring this because IALIAFU is purely abstract.
# pragma warning(disable : 4275) // non dll-interface class 'AAL::IALIAFU' used as base for dll-interface class 'AAL::HWALIAFU'
#endif // __AAL_WINDOWS__

/// @brief This is the Delegate of the Strategy pattern used by ALIAFU to interact with an FPGA-accelerated
///        CCI.
///
/// HWALIAFU is selected by passing the Named Value pair (ALIAFU_NVS_KEY_TARGET, ALIAFU_NVS_VAL_TARGET_FPGA)
/// in the arguments to IRuntime::allocService when requesting a ALIAFU.
class HWALIAFU_API HWALIAFU : public ServiceBase,
                              public IServiceClient,     // for AIA
//                              public IAFUProxyClient,
                              public IALIMMIO,
                              public IALIBuffer,
                              public IALIUMsg,
                              public IALIReset/*
                              public IALIPerf*/
{
#if defined ( __AAL_WINDOWS__ )
# pragma warning(pop)
#endif // __AAL_WINDOWS__
public:
   // <DeviceServiceBase>
   DECLARE_AAL_SERVICE_CONSTRUCTOR(HWALIAFU, ServiceBase)
   {
	   // FIXME: these probably need to go into init() and be exposed based on the AFUDev's capabilities
      SetInterface(        iidALI_MMIO_Service,   dynamic_cast<IALIMMIO *>(this));
      SetInterface(        iidALI_UMSG_Service,   dynamic_cast<IALIUMsg *>(this));
      SetInterface(        iidALI_BUFF_Service,   dynamic_cast<IALIBuffer *>(this));
//      SetInterface(        iidALI_PERF_Service,   dynamic_cast<IALIPerf *>(this)); // still to be defined
      SetInterface(        iidALI_RSET_Service,   dynamic_cast<IALIReset *>(this));
      SetInterface(        iidServiceClient,      dynamic_cast<IServiceClient *>(this));  // for AIA
   }

   virtual btBool init( IBase *pclientBase,
                        NamedValueSet const &optArgs,
                        TransactionID const &rtid);

   virtual btBool Release(TransactionID const &TranID, btTime timeout=AAL_INFINITE_WAIT);
//   virtual btBool Release(btTime timeout=AAL_INFINITE_WAIT);
   // </DeviceServiceBase>

   // <IALIMMIO>
   virtual btVirtAddr   mmioGetAddress( void );
   virtual btCSROffset  mmioGetLength( void );

   virtual btBool   mmioRead32( const btCSROffset Offset,       btUnsigned32bitInt * const pValue);
   virtual btBool  mmioWrite32( const btCSROffset Offset, const btUnsigned32bitInt Value);
   virtual btBool   mmioRead64( const btCSROffset Offset,       btUnsigned64bitInt * const pValue);
   virtual btBool  mmioWrite64( const btCSROffset Offset, const btUnsigned64bitInt Value);
   // </IALIMMIO>

   // <IALIBuffer>
   virtual void bufferAllocate( btWSSize             Length,
                                TransactionID const &TranID,
                                NamedValueSet       *pOptArgs = NULL );
   virtual void     bufferFree( btVirtAddr           Address,
                                TransactionID const &TranID);
   virtual btPhysAddr bufferGetIOVA( btVirtAddr Address);
   // </IALIBuffer>

   // <IALIUMsg>
   virtual btUnsignedInt umsgGetNumber( void );
   virtual btVirtAddr   umsgGetAddress( const btUnsignedInt UMsgNumber );
   virtual bool      umsgSetAttributes( NamedValueSet const &nvsArgs);
   // </IALIUMsg>

   // <IALIReset>
   virtual IALIReset::e_Reset afuQuiesceAndReset( NamedValueSet const *pOptArgs = NULL);
   virtual IALIReset::e_Reset afuReEnable( NamedValueSet const *pOptArgs = NULL);
   virtual IALIReset::e_Reset afuReset( NamedValueSet const *pOptArgs = NULL);
   // </IALIReset>

   // <IServiceClient>
   virtual void serviceAllocated(IBase               *pServiceBase,
                                 TransactionID const &rTranID = TransactionID());
   virtual void serviceAllocateFailed(const IEvent &rEvent);
   virtual void serviceReleased(TransactionID const &rTranID = TransactionID());
   virtual void serviceReleaseFailed(const IEvent &rEvent);
   virtual void serviceEvent(const IEvent &rEvent);
   // </IServiceClient>

/*   // <IAFUProxyClient>
   virtual void AFUEvent(AAL::IEvent const &theEvent);
   // </IAFUProxyClient>*/

protected:
   static void AllocateBufferHandler(IEvent const & );
   static void FreeBufferHandler(IEvent const & );

   IAALService *m_pAALService;
   IAFUProxy   *m_pAFUProxy;
};

/// @}

END_NAMESPACE(AAL)

#endif // __HWALIAFU_H__

