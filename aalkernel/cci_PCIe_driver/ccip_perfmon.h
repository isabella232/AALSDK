//******************************************************************************
// This  file  is  provided  under  a  dual BSD/GPLv2  license.  When using or
//         redistributing this file, you may do so under either license.
//
//                            GPL LICENSE SUMMARY
//
//  Copyright(c) 2011-2016, Intel Corporation.
//
//  This program  is  free software;  you  can redistribute it  and/or  modify
//  it  under  the  terms of  version 2 of  the GNU General Public License  as
//  published by the Free Software Foundation.
//
//  This  program  is distributed  in the  hope that it  will  be useful,  but
//  WITHOUT   ANY   WARRANTY;   without   even  the   implied   warranty    of
//  MERCHANTABILITY  or  FITNESS  FOR  A  PARTICULAR  PURPOSE.  See  the   GNU
//  General Public License for more details.
//
//  The  full  GNU  General Public License is  included in  this  distribution
//  in the file called README.GPLV2-LICENSE.TXT.
//
//  Contact Information:
//  Henry Mitchel, henry.mitchel at intel.com
//  77 Reed Rd., Hudson, MA  01749
//
//                                BSD LICENSE
//
//  Copyright(c) 2011-2016, Intel Corporation.
//
//  Redistribution and  use  in source  and  binary  forms,  with  or  without
//  modification,  are   permitted  provided  that  the  following  conditions
//  are met:
//
//    * Redistributions  of  source  code  must  retain  the  above  copyright
//      notice, this list of conditions and the following disclaimer.
//    * Redistributions in  binary form  must  reproduce  the  above copyright
//      notice,  this  list of  conditions  and  the  following disclaimer  in
//      the   documentation   and/or   other   materials   provided  with  the
//      distribution.
//    * Neither   the  name   of  Intel  Corporation  nor  the  names  of  its
//      contributors  may  be  used  to  endorse  or promote  products derived
//      from this software without specific prior written permission.
//
//  THIS  SOFTWARE  IS  PROVIDED  BY  THE  COPYRIGHT HOLDERS  AND CONTRIBUTORS
//  "AS IS"  AND  ANY  EXPRESS  OR  IMPLIED  WARRANTIES,  INCLUDING,  BUT  NOT
//  LIMITED  TO, THE  IMPLIED WARRANTIES OF  MERCHANTABILITY  AND FITNESS  FOR
//  A  PARTICULAR  PURPOSE  ARE  DISCLAIMED.  IN NO EVENT  SHALL THE COPYRIGHT
//  OWNER OR CONTRIBUTORS BE LIABLE  FOR  ANY  DIRECT,  INDIRECT,  INCIDENTAL,
//  SPECIAL,  EXEMPLARY,  OR  CONSEQUENTIAL   DAMAGES  (INCLUDING,   BUT   NOT
//  LIMITED  TO,  PROCUREMENT  OF  SUBSTITUTE GOODS  OR SERVICES; LOSS OF USE,
//  DATA,  OR PROFITS;  OR BUSINESS INTERRUPTION)  HOWEVER  CAUSED  AND ON ANY
//  THEORY  OF  LIABILITY,  WHETHER  IN  CONTRACT,  STRICT LIABILITY,  OR TORT
//  (INCLUDING  NEGLIGENCE  OR OTHERWISE) ARISING  IN ANY WAY  OUT  OF THE USE
//  OF  THIS  SOFTWARE, EVEN IF ADVISED  OF  THE  POSSIBILITY  OF SUCH DAMAGE.
//******************************************************************************
//****************************************************************************
/// @file ccip_perfmon.h
/// @brief  Definitions for ccip performance counter.
/// @ingroup aalkernel_ccip
/// @verbatim
//        FILE: ccip_perfmon.h
//     CREATED: Sept 24, 2015
//      AUTHOR: Ananda Ravuri, Intel Corporation
//              Joseph Grecco, Intel Corporation
// 
// PURPOSE:
// HISTORY:
// COMMENTS:
// WHEN:          WHO:     WHAT:
// 02/19/2016     JG       Refactored OS specific portion
//****************************************************************************///
#ifndef __AALKERNEL_CCIP_PERFMON_H_
#define __AALKERNEL_CCIP_PERFMON_H_

#include <aalsdk/kernel/aaltypes.h>
#include <aalsdk/kernel/ccipdriver.h>
#include <aalsdk/kernel/ccip_defs.h>
#include "cci_pcie_driver_internal.h"

BEGIN_NAMESPACE(AAL)

#define CACHE_EVENT_COUNTER_MAX_TRY 30
#define PERFMON_POLLING_SLEEP       10

#define PERF_MONITOR_VERSION 1
#define PERF_MONITOR_COUNT 15

#define PMONITOR_VERSION               "version"
#define NUM_COUNTERS                   "number of counters"
#define CACHE_READ_HIT                 "Read_Hit"
#define CACHE_WRITE_HIT                "Write_Hit"
#define CACHE_READ_MISS                "Read_Miss"
#define CACHE_WRITE_MISS               "Write_Miss"
#define CACHE_EVICTIONS                "Evictions"
#define FABRIC_PCIE0_READ              "PCIe0 Read"
#define FABRIC_PCIE0_WRITE             "PCIe0 Write"
#define FABRIC_PCIE1_READ              "PCIe1 Read"
#define FABRIC_PCIE1_WRITE             "PCIe1 Write"
#define FABRIC_UPI_READ                "UPI Read"
#define FABRIC_UPI_WRITE               "UPI Write"

#define VTD_AFU_MEMREAD_TRANS          "VT-d AFU Memory Read Transaction"
#define VTD_AFU_MEMWRITE_TRANS         "VT-d AFU Memory Write Transaction"
#define VTD_AFU_DEVTLBREAD_HIT         "VT-d AFU DevTLB Read Hit"
#define VTD_AFU_DEVTLBWRITE_HIT        "VT-d AFU DevTLB Write Hit"



/// Name:    get_perfmonitor_counters
/// @brief   get snapshot of performance counters
///
/// @param[in] pfme_dev fme device pointer.
/// @param[in] pPerf performance counters pointer
/// @return    error code
bt32bitInt get_perfmonitor_snapshot(struct fme_device *pfme_dev,
                                    struct CCIP_PERF_COUNTERS* pPerf);

/// Name:    update_fabric_event_counters
/// @brief   get Fabric performance counters
///
/// @param[in] event_code performance counters event device pointer.
/// @param[in] pfme_dev fme device pointer.
/// @param[in] pPerf performance counters pointer
/// @return    error code
bt32bitInt update_fabric_event_counters(bt32bitInt event_code ,
                                       struct fme_device *pfme_dev,
                                       struct CCIP_PERF_COUNTERS* pPerf);

/// Name:    update_cache_event_counters
/// @brief   get cache performance counters
///
/// @param[in] event_code performance counters event device pointer.
/// @param[in] pfme_dev fme device pointer.
/// @param[in] pPerf performance counters pointer
/// @return    error code
bt32bitInt update_cache_event_counters(bt32bitInt event_code ,
                                       struct fme_device *pfme_dev,
                                       struct CCIP_PERF_COUNTERS* pPerf);

/// Name:    get_perfmon_counters
/// @brief   get  performance counters
///
/// @param[in] pfme_dev fme device pointer.
/// @param[in] pPerf performance counters pointer
/// @return    error code
bt32bitInt get_perfmon_counters(struct fme_device* pfme_dev,
                                struct CCIP_PERF_COUNTERS* pPerfCounter);

/// Name:    update_vtd_event_counters
/// @brief   get VT-D performance counters
///
/// @param[in] event_code VTD event code.
/// @param[in] pfme_dev fme device pointer.
/// @param[in] pPerf performance counters pointer
/// @return    error code
bt32bitInt update_vtd_event_counters(bt32bitInt event_code,
                                     struct fme_device *pfme_dev,
                                     struct CCIP_PERF_COUNTERS* pPerf);

END_NAMESPACE(AAL)

#endif //__AALKERNEL_CCIP_PERFMON_H_
