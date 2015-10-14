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
/// @file ASECCIv3AFUService.h
/// @brief AAL Service Module definitions for ASE CCIv3 AFU
/// @ingroup ASECCIv3AFU
/// @verbatim
/// Intel(R) QuickAssist Technology Accelerator Abstraction Layer
///
/// AUTHORS: Henry Mitchel, Intel Corporation
///
/// HISTORY:
/// WHEN:          WHO:     WHAT:
/// 07/20/2015     HM       Initial version.@endverbatim
//****************************************************************************
#ifndef __SERVICE_ASECCIV3AFUSERVICE_H__
#define __SERVICE_ASECCIV3AFUSERVICE_H__
#include <aalsdk/osal/OSServiceModule.h>
#include <aalsdk/INTCDefs.h>

/// @addtogroup ASECCIv3AFU
/// @{

#if defined ( __AAL_WINDOWS__ )
# ifdef ASECCIV3AFU_EXPORTS
#    define ASECCIV3AFU_API __declspec(dllexport)
# else
#    define ASECCIV3AFU_API __declspec(dllimport)
# endif // ASECCIV3AFU_EXPORTS
#else
# ifndef __declspec
#    define __declspec(x)
# endif // __declspec
# define ASECCIV3AFU_API    __declspec(0)
#endif // __AAL_WINDOWS__

#define ASECCIV3AFU_SVC_MOD         "libASECCIv3AFU" AAL_SVC_MOD_EXT
#define ASECCIV3AFU_SVC_ENTRY_POINT "libASECCIv3AFU" AAL_SVC_MOD_ENTRY_SUFFIX

#define ASECCIV3AFU_BEGIN_SVC_MOD(__svcfactory) AAL_BEGIN_SVC_MOD(__svcfactory, libASECCIv3AFU, ASECCIV3AFU_API, ASECCIV3AFU_VERSION, ASECCIV3AFU_VERSION_CURRENT, ASECCIV3AFU_VERSION_REVISION, ASECCIV3AFU_VERSION_AGE)
#define ASECCIV3AFU_END_SVC_MOD()               AAL_END_SVC_MOD()

AAL_DECLARE_SVC_MOD(libASECCIv3AFU, ASECCIV3AFU_API)

/// ASE CCIv3 AFU interface ID.
#define iidASECCIv3AFU __INTC_IID(INTC_sysSampleAFU, 0x0010)

#define ASECCIV3AFU_MANIFEST \
"9 20 ConfigRecordIncluded\n \
\t10\n \
\t\t9 17 ServiceExecutable\n \
\t\t\t9 14 libASECCIv3AFU\n \
\t\t9 18 _CreateSoftService\n \
\t\t0 1\n \
9 29 ---- End of embedded NVS ----\n \
9999\n"

/// @}

#endif // __SERVICE_ASECCIV3AFUSERVICE_H__
