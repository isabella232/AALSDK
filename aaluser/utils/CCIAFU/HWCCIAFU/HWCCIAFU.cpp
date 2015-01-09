// Copyright (c) 2014-2015, Intel Corporation
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
/// @file HWCCIAFU.cpp
/// @brief Implementation of CCI AFU Hardware Service.
/// @ingroup HWCCIAFU
/// @verbatim
/// Intel(R) QuickAssist Technology Accelerator Abstraction Layer Sample Application
///
///    This application is for example purposes only.
///    It is not intended to represent a model for developing commercially-deployable applications.
///    It is designed to show working examples of the AAL programming model and APIs.
///
/// AUTHORS: Tim Whisonant, Intel Corporation
///          Joseph Grecco, Intel Corporation
///          
///
/// This sample demonstrates how to create an AFU Service that uses a host-based AFU engine.
///  This design also applies to AFU Services that use hardware via a
///  Physical Interface Protocol (PIP) module.
///
/// HISTORY:
/// WHEN:          WHO:     WHAT:
/// 07/18/2014     TSW      Initial version.@endverbatim
//****************************************************************************
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif // HAVE_CONFIG_H

#include "HWCCIAFU.h"

BEGIN_NAMESPACE(AAL)

/// @addtogroup HWCCIAFU
/// @{

void HWCCIAFU::init(TransactionID const &TranID)
{
   QueueAASEvent( new(std::nothrow) AAL::AAS::ObjectCreatedEvent(getRuntimeClient(),
                                                                 Client(),
                                                                 dynamic_cast<IBase *>(this),
                                                                 TranID) );
}

btBool HWCCIAFU::Release(TransactionID const &TranID, btTime timeout)
{
   return DeviceServiceBase::Release(TranID, timeout);
}

btBool HWCCIAFU::Release(btTime timeout)
{
   return DeviceServiceBase::Release(timeout);
}

void HWCCIAFU::WorkspaceAllocate(btWSSize             Length,
                                 TransactionID const &TranID)
{
   AutoLock(this);
   WkspcAlloc(Length, TranID);
}

void HWCCIAFU::WorkspaceFree(btVirtAddr           Address,
                             TransactionID const &TranID)
{
   AutoLock(this);
   WkspcFree(Address, TranID);
}

btBool HWCCIAFU::CSRRead(btCSROffset CSR,
                         btCSRValue *pValue)
{
   // Divide by 4, because CAFUDev expects 0-based CSR #'s, not byte offsets.
   return AFUDev().atomicGetCSR(CSR >> 2, pValue);
}

btBool HWCCIAFU::CSRWrite(btCSROffset CSR,
                          btCSRValue  Value)
{
   // Divide by 4, because CAFUDev expects 0-based CSR #'s, not byte offsets.
   return AFUDev().atomicSetCSR(CSR >> 2, Value);
}

btBool HWCCIAFU::CSRWrite64(btCSROffset CSR,
                            bt64bitCSR  Value)
{
   if ( CSRWrite(CSR + 4, Value >> 32) ) {
      return CSRWrite(CSR, Value & 0xffffffff);
   }
   return false;
}

/// @} group HWCCIAFU

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


#define SERVICE_FACTORY AAL::AAS::InProcSvcsFact< HWCCIAFU >

#if defined ( __AAL_WINDOWS__ )
# pragma warning(push)
# pragma warning(disable : 4996) // destination of copy is unsafe
#endif // __AAL_WINDOWS__

HWCCIAFU_BEGIN_SVC_MOD(SERVICE_FACTORY)
   /* No commands other than default, at the moment. */
HWCCIAFU_END_SVC_MOD()

#if defined ( __AAL_WINDOWS__ )
# pragma warning(pop)
#endif // __AAL_WINDOWS__

