//******************************************************************************
// This  file  is  provided  under  a  dual BSD/GPLv2  license.  When using or
//         redistributing this file, you may do so under either license.
//
//                            GPL LICENSE SUMMARY
//
//  Copyright(c) 2015-2016, Intel Corporation.
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
//  Copyright(c) 2015-2016, Intel Corporation.
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
/// @file ccip_defs.h
/// @brief  Public definitions for CCI-P compliant devices.
/// @ingroup aalkernel_ccip
/// @verbatim
///        FILE: ccip_defs.h
///     CREATED: Oct 28, 2015
///      AUTHOR: Ananda Ravuri, Intel <ananda.ravuri@intel.com>
///              Joseph Grecco, Intel <joe.grecco@intel.com>
///
/// PURPOSE:   This file contains the definitions for the CCI-P compliant FPGA
///            devices.
/// HISTORY:
/// COMMENTS:
/// WHEN:          WHO:     WHAT:
/// 10/28/15       JG       Created from prototype written by AR@endverbatim
//****************************************************************************
#ifndef __AALKERNEL_CCIP_DEFS_H_
#define __AALKERNEL_CCIP_DEFS_H_
#include "aalsdk/kernel/kosal.h"

#if !defined(MODULE_FLAGS)
#define MODULE_FLAGS CCIPCIE_DBG_MOD // Prints all
#endif

#include <aalsdk/kernel/aaltypes.h>


BEGIN_NAMESPACE(AAL)


/// @addtogroup aalkernel_ccip
/// @{

/// MMIO space Size
/// Simulation  mmio size
#define CCIP_MMIO_SIZE              ( 0x120000 )

// Max number of pci bars to scan
#define CCIP_MAX_PCIBAR             5

#define CCIP_UMSG_SIZE              (0x1000)

// Clear Error CSRs
#define  CLEAR_ALL_ERRORS 0xFFFFFFFFFFFFFFFF

///=================================================================
/// Enumerations
///=================================================================
enum e_CCIP_DEV_FEATURE_ID {
	CCIP_DEVFID_Configuration,
	CCIP_DEVFID_FME,
	CCIP_DEVFID_PORT,
	CCIP_DEVFID_SIGTAP,
};


/// CCIP Device type ID
enum e_CCIP_DEVTPPE_ID {
   CCIP_DFType_rsvd      = 0x0,
   CCIP_DFType_afu       = 0x1,
   CCIP_DFType_bbb       = 0x2,
   CCIP_DFType_private   = 0x3
};

// FPGA Management Engine (FME)
//-----------------------------

// Device Feature ID
enum e_CCIP_DFL_ID {
   CCIP_FME_DFLID_THERM  = 0x1,
   CCIP_FME_DFLID_POWER  = 0x2,
   CCIP_FME_DFLID_GPERF  = 0x3,
   CCIP_FME_DFLID_GERR   = 0x4,
   CCIP_FME_DFLID_PR     = 0x5,
   CCIP_PORT_DFLID_ERROR = 0x10,
   CCIP_PORT_DFLID_USMG  = 0x11,
   CCIP_PORT_DFLID_PR    = 0x12,
   CCIP_PORT_DFLID_STP   = 0x13
};

// Device Feature revision ID
enum e_CCIP_DFL_rev {
   CCIP_DFL_rev0    = 0x0,
   CCIP_DFL_rev1    = 0x1,
   CCIP_DFL_rev2    = 0x2,
   CCIP_DFL_rev3    = 0x3,
   CCIP_DFL_rev4    = 0x4
};


// AFU power states
enum e_AFU_Power_State {
   AFU_Power_Normal = 0x0,
   AFU_Power_AP1 = 0x1,
   AFU_Power_AP2 = 0x2,
   AFU_Power_AP6 = 0x6
};

// Cache Event codes
enum e_Cache_Event_Code {
   Cache_Read_Hit = 0x0,
   Cache_Write_Hit = 0x1,
   Cache_Read_Miss = 0x2,
   Cache_Write_Miss = 0x3,
   Cache_Rsvd = 0x4,
   Cache_Hold_Req = 0x5,
   Cache_Data_WrtPort_Conten = 0x6,
   Cache_Tag_WrtPort_Conten = 0x7,
   Cache_Tx_Req_stall = 0x8,
   Cache_Rx_Stalls = 0x9,
   Cache_Evictions = 0xA

};

// Fabric event codes
enum e_Fabric_Event_Code {
   Fabric_PCIe0_Read = 0x0,
   Fabric_PCIe0_Write = 0x1,
   Fabric_PCIe1_Read = 0x2,
   Fabric_PCIe1_Write = 0x3,
   Fabric_UPI_Read = 0x4,
   Fabric_UPI_Write = 0x5
};

// VT-D event codes
enum e_vtd_Event_Code {
   AFU0_MemRead_Trans        = 0x0,
   AFU1_MemRead_Trans        = 0x1,
   AFU0_MemWrite_Trans       = 0x2,
   AFU1_MemWrite_Trans       = 0x3,
   AFU0_DevTLBRead_Hit       = 0x4,
   AFU1_DevTLBRead_Hit       = 0x5,
   AFU0_DevTLBWrite_Hit      = 0x6,
   AFU1_DevTLBWrite_Hit      = 0x7,
};

/// CCI-P port id
enum e_CCIP_Port_Id {
   CCIP_Port_Id0 = 0x0,
   CCIP_Port_Id1 = 0x1,
   CCIP_Port_Rsvd = 0x2,
   CCIP_Port_Rsvd1 = 0x3
};

/// Partial Reconfiguration ID
enum e_FME_PR_REGION_ID {
   CCIP_PR_Region0 = 0x0,
   CCIP_PR_Region1 = 0x1,
   CCIP_PR_rsvd1 = 0x2,
   CCIP_PR_rsvd2 = 0x3

};

/// Partial Reconfiguration status
enum e_CCIP_PORT_PR_status {
   CCIP_PORT_PR_Idle = 0x0,
   CCIP_PORT_PR_RecStart = 0x1,
   CCIP_PORT_PR_ReSet = 0x2,
   CCIP_PORT_WaitFreeze = 0x3,
   CCIP_PORT_WaitPR = 0x4,
   CCIP_PORT_SendFrst_Data = 0x5,
   CCIP_PORT_WaitPRReady = 0x6,
   CCIP_PORT_PushFIFO_IP = 0x7,
   CCIP_PORT_WaitPR_Resp = 0x8,
   CCIP_PORT_PR_Complete = 0x9,
   CCIP_PORT_PR_UnFreeze = 0xA,
   CCIP_PORT_PR_DeAssert = 0xB
};

/// Partial Reconfiguration  mega function status codes
enum e_CCIP_PR_ControllerBlock_status {
   CCIP_PR_CLB_pwrup = 0x0,
   CCIP_PR_CLB_error = 0x1,
   CCIP_PR_CLB_crc_err = 0x2,
   CCIP_PR_CLB_Incomp_bts_err = 0x3,
   CCIP_PR_CLB_opr_inPros = 0x4,
   CCIP_PR_CLB_por_pass = 0x5,
   CCIP_PR_CLB_rsvd1 = 0x6,
   CCIP_PR_CLB_rsvd2 = 0x7
};

///=================================================================
/// IDs used by the devices and objects
///=================================================================

// FPGA Management Engine GUID
#define CCIP_FME_GUIDL              (0x82FE38F0F9E17764ULL)
#define CCIP_FME_GUIDH              (0xBFAf2AE94A5246E3ULL)
#define CCIP_FME_PIPIID             (0x4DDEA2705E7344D1ULL)

#define CCIP_DEV_FME_SUBDEV         ((btUnsigned16bitInt)(-1))
#define CCIP_DEV_PORT_SUBDEV(s)     (s + 0x10)
#define CCIP_DEV_AFU_SUBDEV(s)      (s + 0x20)

#define PORT_SUBDEV(s)              (s - 0x10 )
#define AFU_SUBDEV(s)               (s - 0x20 )

/// FPGA Port GUID
#define CCIP_PORT_GUIDL             (0x9642B06C6B355B87ULL)
#define CCIP_PORT_GUIDH             (0x3AB49893138D42EBULL)
#define CCIP_PORT_PIPIID            (0x5E82B04A50E59F20ULL)

/// AFU GUID
#define CCIP_AFU_PIPIID             (0x26F67D4CAD054DFCULL)

/// Signal Tap GUID
#define CCIP_STAP_GUIDL             (0xB6B03A385883AB8DULL)
#define CCIP_STAP_GUIDH             (0x022F85B12CC24C9DULL)
#define CCIP_STAP_PIPIID            (0xA710C842F06E45E0ULL)


/// Partial Reconfiguration GUID
#define CCIP_PR_GUIDL               (0x83B54FD5E5216870ULL)
#define CCIP_PR_GUIDH               (0xA3AAB28579A04572ULL)
#define CCIP_PR_PIPIID              (0x7C4D41EA156C4D81ULL)


/// FME Power  GUID
#define CCIP_PWR_GUIDL               (0x99DE979A70CEE6C9ULL)
#define CCIP_PWR_GUIDH               (0x93136515D527451AULL)
#define CCIP_PWR_PIPIID              (0xBBD60200C0A80117ULL)


/// Vender ID and Device ID
#define CCIP_FPGA_VENDER_ID         0x8086

/// PCI Device ID
#define PCIe_DEVICE_ID_RCiEP0       0xBCBD
#define PCIe_DEVICE_ID_RCiEP1       0xBCBE

/// VF Device
#define PCIe_DEVICE_ID_VF           0xBCBF

/// QPI Device ID
#define PCIe_DEVICE_ID_RCiEP2       0xBCBC



/// MMIO Space map
#define FME_DFH_AFUIDL  0x8
#define FME_DFH_AFUIDH  0x10
#define FME_DFH_NEXTAFU 0x18


/******************************************************************************
 *  FPGA  Header and CSR
 *
 *   Description:
 *
 ******************************************************************************/

/// Device Feature Header CSR
struct CCIP_DFH {

   union {
      btUnsigned64bitInt csr;
      struct {
         btUnsigned64bitInt Feature_ID :12;      // Feature ID
         btUnsigned64bitInt Feature_rev :4;      // Feature revision
         btUnsigned64bitInt next_DFH_offset :24; // Next Device Feature header offset
         btUnsigned64bitInt eol :1;              // End of Device feature list
         btUnsigned64bitInt rsvd :19;            // Reserved
         btUnsigned64bitInt Type :4;             // Type of Device

      }; //end struct
   }; // end union

}; //end struct CCIP_DFH
CASSERT(sizeof(struct CCIP_DFH) == 8);

/// AFU ID low CSR
struct CCIP_AFU_ID_L {

   union {
      btUnsigned64bitInt csr;
      struct {
         btUnsigned64bitInt afu_id_l :64;    // AFU ID low

      }; // end struct
   }; // end union

}; //end struct CCIP_AFU_ID_L
CASSERT(sizeof(struct CCIP_AFU_ID_L) == 8);

/// AFU ID high CSR
struct CCIP_AFU_ID_H {

   union {
      btUnsigned64bitInt csr;
      struct {
         btUnsigned64bitInt afu_id_h :64;    // AFU ID low

      }; // end struct
   }; // end union

}; //end struct CCIP_AFU_ID_L
CASSERT(sizeof(struct CCIP_AFU_ID_H) == 8);


/// Next AFU offset CSR
struct CCIP_NEXT_AFU {

   union {
      btUnsigned64bitInt csr;
      struct {
         btUnsigned64bitInt afu_id_offset :24;   // Next AFU DFH byte offset
         btUnsigned64bitInt rsvd :40;            // Reserved

      }; // end struct
   }; // end union

}; // end struct CCIP_NEXT_AFU
CASSERT(sizeof(struct CCIP_NEXT_AFU) == 8);

/// FME Scratch pad CSR
struct CCIP_SCRATCHPAD {
   union {
      btUnsigned64bitInt csr;
      struct {
         btUnsigned64bitInt scratch_pad :64;   //Scratch pad Register

      }; // end struct
   }; // end union

}; // end struct CCIP_SCRATCHPAD
CASSERT(sizeof(struct CCIP_SCRATCHPAD) == 8);

// Fabric capability
struct CCIP_FAB_CAPABILITY {

   union {
      btUnsigned64bitInt csr;
      struct {
         btUnsigned64bitInt fabric_verid :8;             // Fabric version ID
         btUnsigned64bitInt socket_id :1;                // Socket id
         btUnsigned64bitInt rsvd1 :3;                    // Reserved
         btUnsigned64bitInt pci0_link_avile :1;          // pci0 link available yes /no
         btUnsigned64bitInt pci1_link_avile :1;          // pci1 link available yes /no
         btUnsigned64bitInt qpi_link_avile :1;           // Coherent (QPI/UPI) link available yes /no
         btUnsigned64bitInt rsvd2 :1;                    // Reserved
         btUnsigned64bitInt iommu_support :1;            // IOMMU or VT-d supported  yes/no
         btUnsigned64bitInt num_ports :3;                // Number of ports
         btUnsigned64bitInt rsvd3 :4;                    // Reserved
         btUnsigned64bitInt address_width_bits :6;       // Address width supported in bits  BXT -0x26 , SKX -0x30
         btUnsigned64bitInt rsvd4 :2;                    // Reserved
         btUnsigned64bitInt cache_size :12;              // Size of cache supported in kb
         btUnsigned64bitInt cache_assoc :4;              // Cache Associativity
         btUnsigned64bitInt rsvd5 :15;                   // Reserved
         btUnsigned64bitInt lock_bit :1;                 // Lock bit
      }; // end struct
   }; // end union

}; // struct CCIP_FME_CAPABILITY

CASSERT(sizeof(struct CCIP_FAB_CAPABILITY) == 8);

/// FPGA Port offset CSR
struct CCIP_PORT_AFU_OFFSET {

   union {
      btUnsigned64bitInt csr;
      struct {
         btUnsigned64bitInt port_offset :24;          // Port offset
         btUnsigned64bitInt rsvd1 :8;                 // Reserved
         btUnsigned64bitInt port_bar :3;              // Port BAR
         btUnsigned64bitInt rsvd2 :20;                // Reserved
         btUnsigned64bitInt afu_access_control :1;    // afu access type
         btUnsigned64bitInt rsvd3 :4;                 // Reserved
         btUnsigned64bitInt port_imp :1;              // Port Implemented
         btUnsigned64bitInt rsvd4 :3;                 // Reserved
      }; // end struct
   }; // end union

}; // end struct CCIP_PORT_AFU_OFFSET
CASSERT(sizeof(struct CCIP_PORT_AFU_OFFSET) == 8);

// Fabric status
struct CCIP_FAB_STATUS {

   union {
      btUnsigned64bitInt csr;
      struct {
         btUnsigned64bitInt upilink_status :4;       // UPI Link Status
         btUnsigned64bitInt rsvd1 :4;                // Reserved

         btUnsigned64bitInt pcie0link_status :1;    // PCIe0 link status
         btUnsigned64bitInt rsvd2 :3;               // Reserved

         btUnsigned64bitInt pcie1link_status :1;    // PCIe1 link status
         btUnsigned64bitInt rsvd3 :51;              // Reserved
      }; // end struct
   }; // end union

}; // struct CCIP_FAB_STATUS
CASSERT(sizeof(struct CCIP_FAB_STATUS) == 8);

/// Blue bitstream ID  CSR
struct CCIP_BBS_ID {

   union {
      btUnsigned64bitInt csr;
      struct {
         btUnsigned64bitInt bbs_id :64;   // BBS ID

      }; // end struct
   }; // end union

}; //end struct CCIP_BBS_ID
CASSERT(sizeof(struct CCIP_BBS_ID) == 8);


/// Blue bitstream metadata  CSR
struct CCIP_BBS_MD {

   union {
      btUnsigned64bitInt csr;
      struct {
         btUnsigned64bitInt bbs_md :64;   // BBS Meta data

      }; // end struct
   }; // end union

}; //end struct CCIP_BBS_MD
CASSERT(sizeof(struct CCIP_BBS_MD) == 8);

/// FPGA Protected Range Base Address
struct CCIP_FME_GENPROTRANGE2_BASE {

   union {
      btUnsigned64bitInt csr;
      struct {
         btUnsigned64bitInt rsvd1 :16;                  // Reserved
         btUnsigned64bitInt protected_base_addrss:4;    // Base Address of memory range
         btUnsigned64bitInt rsvd2 :44;                  // Reserved
      }; // end struct
   }; // end union

}; // end struct CCIP_FME_GENPROTRANGE2_BASE
CASSERT(sizeof(struct CCIP_FME_GENPROTRANGE2_BASE) == 8);

/// FPGA Protected Range Base limit address
struct CCIP_FME_GENPROTRANGE2_LIMIT {

   union {
      btUnsigned64bitInt csr;
      struct {
         btUnsigned64bitInt rsvd1 :16;                   // Reserved
         btUnsigned64bitInt protected_limit_addrss:4;    // Limit Address of memory range
         btUnsigned64bitInt rsvd2 :44;                   // Reserved
      }; // end struct
   }; // end union

}; // end struct CCIP_FME_GENPROTRANGE2_LIMIT
CASSERT(sizeof(struct CCIP_FME_GENPROTRANGE2_LIMIT) == 8);


/// DXE LOCK
struct CCIP_DXE_LOCK {

   union {
      btUnsigned64bitInt csr;
      struct {
         btUnsigned64bitInt dxe_lock:64;   // DXE LOCK
      }; // end struct
   }; // end union

}; // end struct CCIP_DXE_LOCK
CASSERT(sizeof(struct CCIP_DXE_LOCK) == 8);


// HSSI Control
struct CCIP_HSSI_CTRL {

   union {
      btUnsigned64bitInt csr;
      struct {
         btUnsigned64bitInt data:32;       // Data
         btUnsigned64bitInt address:16;    // Address
         btUnsigned64bitInt command:16;    // Command
      }; // end struct
   }; // end union

}; // end struct CCIP_HSSI_CTRL
CASSERT(sizeof(struct CCIP_HSSI_CTRL) == 8);


// HSSI status
struct CCIP_HSSI_STAT{

   union {
      btUnsigned64bitInt csr;
      struct {
         btUnsigned64bitInt data:32;      // Data
         btUnsigned64bitInt ack:1;        // acknowledge
         btUnsigned64bitInt spare:1;      // Spare
         btUnsigned64bitInt rsvd:30;      // Reserved
      }; // end struct
   }; // end union

}; // end struct CCIP_HSSI_STAT
CASSERT(sizeof(struct CCIP_HSSI_STAT) == 8);

/******************************************************************************
 *  FPGA Management Engine  Header
 *  CCI-P version =1
 *  CCI-P minor version =0
 *  Type = afu
 *  Header Description:
 *
 ******************************************************************************/
///============================================================================
/// Name: CCIP_FME_HDR
/// @brief   FPGA Management Engine  Header
///  Version: 1
///  Type:  Private
///  Feature: AFU
///============================================================================
struct CCIP_FME_HDR {

   // FME  Header
   struct CCIP_DFH                                 dfh;                   // Offset 0

   // FME afu id  low
   struct CCIP_AFU_ID_L                            afu_id_l;              // Offset 0x8

   // FME afu id  high
   struct CCIP_AFU_ID_H                            afu_id_h;              // Offset 0x10

   // Next AFU offset
   struct CCIP_NEXT_AFU                            next_afu;               // Offset 0x18

   // Reserved
   btUnsigned64bitInt                              rsvd_fmehdr;            // Offset 0x20

   // FME Scratch pad
   struct CCIP_SCRATCHPAD                          scratchpad;             // Offset 0x28

   // Fabric capability
   struct CCIP_FAB_CAPABILITY                      fab_capability;          // Offset 0x30

   // Beginning of the list of offsets to
   //the Port regions
   struct CCIP_PORT_AFU_OFFSET                     port_offsets[4];         // Offset 0x38

   // fabric status csr
   struct CCIP_FAB_STATUS                          fab_status ;              // Offset 0x58

   // Bitstream ID
   struct CCIP_BBS_ID                              bbs_id ;                  // Offset 0x60

   // Bitstream medadata
   struct CCIP_BBS_MD                              bbs_md ;                  // Offset 0x68

   struct CCIP_FME_GENPROTRANGE2_BASE              genprotrannge2_base ;     // Offset 0x70

   struct CCIP_FME_GENPROTRANGE2_LIMIT             enprotrannge2_limit ;     // Offset 0x78

   //DXE LOCK
   struct CCIP_DXE_LOCK                            dxe_lock ;                 // Offset 0x80

   //HSSI Lock
   struct CCIP_HSSI_CTRL                           hssi_ctrl ;                // Offset 0x88

   // HSSI Status
   struct CCIP_HSSI_STAT                           hssi_status;               // Offset 0x90


}; //end struct CCIP_FME_HDR
CASSERT(sizeof(struct CCIP_FME_HDR) == (8 * 19));

///============================================================================
/// Name: CCIP_FME_DFL_THERM
/// @brief   FPGA Management Engine  Thermal  Management Feature
///  Feature ID: 0x1
///  Feature Type: = Private
///  Feature Revision: 0
///============================================================================


// Temperature Threshold
struct CCIP_TEMP_THRESHOLD {

   union {
      btUnsigned64bitInt csr;
      struct {
         btUnsigned64bitInt tmp_thshold1 :7;                // temperature Threshold 1
         btUnsigned64bitInt tmp_thshold1_status :1;         // temperature Threshold 1 enable /disable
         btUnsigned64bitInt tmp_thshold2 :7;                // temperature Threshold 2
         btUnsigned64bitInt tmp_thshold2_status :1;         // temperature Threshold 2 enable /disable
         btUnsigned64bitInt rsvd4 :8;                       // Reserved
         btUnsigned64bitInt therm_trip_thshold :7;          // Thermeal Trip Threshold
         btUnsigned64bitInt rsvd3 :1;                       // Reserved
         btUnsigned64bitInt thshold1_status :1;             // Threshold 1 Status
         btUnsigned64bitInt thshold2_status :1;             // Threshold 2 Status
         btUnsigned64bitInt rsvd5 :1;                       // Reserved
         btUnsigned64bitInt therm_trip_thshold_status :1;   // Thermeal Trip Threshold status
         btUnsigned64bitInt rsvd2 :8;                       // Reserved
         btUnsigned64bitInt thshold_policy :1;              // threshold policy
         btUnsigned64bitInt rsvd :19;                       // Reserved

     }; //end struct
   }; // end union

}; // end struct CCIP_TMP_THRESHOLD


// Temperature Sensor Read values
struct CCIP_TEMP_RDSSENSOR_FMT1 {

   union {
      btUnsigned64bitInt csr;
      struct {
         btUnsigned64bitInt tmp_reading :7;               // Reads out FPGA temperature in celsius.
         btUnsigned64bitInt rsvd2 :1;                     // Reserved
         btUnsigned64bitInt tmp_reading_seq_num :16;      // Temperature reading sequence number
         btUnsigned64bitInt tmp_reading_valid :1;         // Temperature reading is valid
         btUnsigned64bitInt rsvd1 :7;                     // Reserved
         btUnsigned64bitInt dbg_mode :8;                  // Debug mode
         btUnsigned64bitInt rsvd :24;                     // Reserved

      }; // end struct
   } ; // end union

}; // end struct CCIP_TMP_RDSSENSOR_FMT1

// Temperature sensor read values
struct CCIP_TEMP_RDSSENSOR_FMT2 {

   union {
      btUnsigned64bitInt csr;
      struct {
         btUnsigned64bitInt rsvd :64;  // TBD
      }; // end struct
   }; // end union

}; // end struct CCIP_TMP_RDSSENSOR_FMT2


struct CCIP_FME_DFL_THERM {

   // Thermal  Management Feature Header
   struct CCIP_DFH                     ccip_fme_tmp_dflhdr ;

   struct CCIP_TEMP_THRESHOLD          ccip_tmp_threshold;
   struct CCIP_TEMP_RDSSENSOR_FMT1     ccip_tmp_rdssensor_fm1;
   struct CCIP_TEMP_RDSSENSOR_FMT2     ccip_tmp_rdssensor_fm2;

}; //end struct CCIP_FME_TMP_DFL
CASSERT(sizeof(struct CCIP_FME_DFL_THERM) == (4*8));


///============================================================================
/// Name: CCIP_FME_DFL_PM
/// @brief   FPGA Management Engine Power Management Feature
///  Feature ID: 0x2
///  Feature Type: = Private
///  Feature Revision: 0
///============================================================================

//Power Management Status
struct CCIP_PM_STATUS {

   union {
      btUnsigned64bitInt csr;
      struct {
         btUnsigned64bitInt pwr_consumed :18;        // power consumed
         btUnsigned64bitInt fpga_latency_report :1;  // FPGA Latency Tolerance Reporting (LTR)
         btUnsigned64bitInt rsvd3 :45;               // Reserved
      }; // end struct
   }; // end union

} ; // end struct CCIP_PM_STATUS
CASSERT(sizeof(struct CCIP_PM_STATUS) == (1*8));


//Power Management AP Thresholds
struct CCIP_PM_AP_THRESHOLDS {

   union {
      btUnsigned64bitInt csr;
      struct {
         btUnsigned64bitInt threshold1 :10;           //Threshold 1
         btUnsigned64bitInt threshold2 :10;           //Threshold 2
         btUnsigned64bitInt threshold3 :10;           //Threshold 3
         btUnsigned64bitInt threshold4 :10;           //Threshold 4
         btUnsigned64bitInt rsvd :24;                 // Reserved
      }; // end struct
   }; // end union

} ; // end struct CCIP_PM_AP_THRESHOLDS
CASSERT(sizeof(struct CCIP_PM_AP_THRESHOLDS) == (1*8));


struct CCIP_FME_DFL_PM {

   // FME Power Management Feature header
   struct CCIP_DFH                      ccip_fme_pm_dflhdr;

   // PM Status
   struct CCIP_PM_STATUS                pm_status;

   // PM AP thresholds
   struct CCIP_PM_AP_THRESHOLDS         pm_ap_thresholds;
  
}; // end struct CCIP_PM_MAXVR
CASSERT(sizeof(struct CCIP_FME_DFL_PM) == (8* 3));

///============================================================================
/// Name: CCIP_FME_DFL_FPMON
/// @brief   FPGA Management Engine Performance Monitor Feature
///  Feature ID: 0x3
///  Feature Type: = Private
///  Feature Revision: 0
///============================================================================
struct CCIP_FME_DFL_FPMON {

   //Performance Fabric control Header
   struct CCIP_DFH ccip_fme_fpmon_dflhdr;

   // FPGA performance counters
   struct CCIP_FPMON_CH_CTL {

      union {
         btUnsigned64bitInt csr;
         struct {
            btUnsigned64bitInt rsvd2 :8;           // Reserved
            btUnsigned64bitInt freeze :1;          // Freeze
            btUnsigned64bitInt rsvd1 :7;           // Reserved
            btUnsigned64bitInt cache_event :4;     // Cache Event code
            btUnsigned64bitInt cci_chsel :1;       // CCI channel selection
            btUnsigned64bitInt rsvd :43;           // Reserved
         }; // end struct
      }; // end union

   }ccip_fpmon_ch_ctl; // end struct CCIP_FPMON_CH_CTL

   // FPGA  Cache port0
   struct CCIP_FPMON_CH_CTR_0 {

      union {
         btUnsigned64bitInt csr;
         struct {
            btUnsigned64bitInt cache_counter : 48;    // Cache counter
            btUnsigned64bitInt rsvd : 12;             // Reserved
            btUnsigned64bitInt event_code : 4;        // Event code
         }; // end struct
      }; // end union

   }ccip_fpmon_ch_ctr_0; // end CIP_FPMON_CH_CTR_0

   // FPGA Cache port1
   struct CCIP_FPMON_CH_CTR_1 {

      union {
         btUnsigned64bitInt csr;
         struct {
            btUnsigned64bitInt cache_counter : 48;    // Cache counter
            btUnsigned64bitInt rsvd : 12;             // Reserved
            btUnsigned64bitInt event_code : 4;        // Event Code
         }; // end struct
      }; // end union

   }ccip_fpmon_ch_ctr_1; // end struct CCIP_FPMON_CH_CTR_1

   // FPGA  Fabric control
   struct CCIP_FPMON_FAB_CTL {

      union {
         btUnsigned64bitInt csr;
         struct {
            btUnsigned64bitInt rsvd3 :8;                 // Reserved
            btUnsigned64bitInt freeze :1;                // Freeze
            btUnsigned64bitInt rsvd2 :7;                 // Reserved
            btUnsigned64bitInt fabric_evt_code :4;       // Fabric Event Code
            btUnsigned64bitInt port_id :2;               // CCI-P Port ID
            btUnsigned64bitInt rsvd1 :1;                 // Reserved
            btUnsigned64bitInt ccip_port_filter :1;      // CCI-P Port Filter enable /disable
            btUnsigned64bitInt rsvd :40;                 // Reserved
         }; // end stuct
      }; // end unon

   }ccip_fpmon_fab_ctl; // end struct CCIP_FPMON_FAB_CTL

   // FPGA  Fabric control
   struct CCIP_FPMON_FAB_CTR {

      union {
         btUnsigned64bitInt csr;
         struct {
            btUnsigned64bitInt fabric_counter : 60;   // Fabric event counter
            btUnsigned64bitInt event_code : 4;        // Event code
         }; // end struct
      }; //end union

   }ccip_fpmon_fab_ctr; //end struct CCIP_FPMON_FAB_CTR

   // FPGA  Fabric control
   struct CCIP_FPMON_CLK_CTRS {

      union {
         btUnsigned64bitInt csr;
         struct {
            btUnsigned64bitInt afu_interf_clock :64;     // AFU interface clock
         }; // end struct
      }; //end union


   }ccip_fpmon_clk_ctrs; // end struct CCIP_FPMON_CLK_CTRS

   // FPGA  VTD control
   struct CCIP_FPMON_VTD_CTL {

      union {
         btUnsigned64bitInt csr;
         struct {
            btUnsigned64bitInt rsvd : 8;              // Reserved
            btUnsigned64bitInt freeze : 1;            // Freeze and un freeze
            btUnsigned64bitInt rsvd1 : 7;             // Reserved
            btUnsigned64bitInt vtd_evtcode : 4;       // VTD Event code
            btUnsigned64bitInt rsvd3 : 44;            // Reserved
         }; // end struct
      }; //end union

   }ccip_fpmon_vtd_ctl; //end struct CCIP_FPMON_VTD_CTL

   // FPGA  VTD counter
   struct CCIP_FPMON_VTD_CTR {

      union {
         btUnsigned64bitInt csr;
         struct {
            btUnsigned64bitInt vtd_counter : 48;      // vt-d event counter
            btUnsigned64bitInt rsvd : 12;             // Reserved
            btUnsigned64bitInt event_code : 4;        // Event code
         }; // end struct
      }; //end union

   }ccip_fpmon_vtd_ctr; //end struct CCIP_FPMON_VTD_CTR
};
CASSERT(sizeof(struct CCIP_FME_DFL_FPMON) == (9*8));

// FME Errors

struct CCIP_FME_ERROR0 {

   union {
      btUnsigned64bitInt csr;
      struct {
         btUnsigned64bitInt fabric_err :1;               // Fabric error
         btUnsigned64bitInt fabFifo_uoflow :1;           // Fabric fifo underflow overflow
         btUnsigned64bitInt pcie0_poison_detected :1;    // PCIe0 Poison Detected
         btUnsigned64bitInt pcie1_poison_detected :1;    // PCIE1 Poison Detected
         btUnsigned64bitInt iommu_parity_error :1;       // IOMMU Parity Error
         btUnsigned64bitInt afuerr_access_mismatch :1;   // AFU error mismatch
         btUnsigned64bitInt mbp_event :1;                // MBP Event
         btUnsigned64bitInt rsvd :57;                    // Reserved
      }; // end struct
   }; // end union

}; // end struct CCIP_FME_ERROR
CASSERT(sizeof(struct CCIP_FME_ERROR0) == (1 *8));


struct CCIP_FME_PCIE0_ERROR {

   union {
      btUnsigned64bitInt csr;
      struct {
         btUnsigned64bitInt formattype_err :1;              // TLP Format/type error
         btUnsigned64bitInt MWAddr_err :1;                  // TLP MW Address error
         btUnsigned64bitInt MWAddrLength_err :1;            // TLP MW Length error
         btUnsigned64bitInt MRAddr_err :1;                  // TLP MR Address error
         btUnsigned64bitInt MRAddrLength_err :1;            // TLP MR Length error
         btUnsigned64bitInt cpl_tag_err :1;                 // TLP CPL TAP error
         btUnsigned64bitInt cpl_status_err :1;              // TLP CPL status error
         btUnsigned64bitInt cpl_timeout_err :1;             // TLP CPL Timeout error
         btUnsigned64bitInt rsvd :54;                       // Reserved

         btUnsigned64bitInt vfnumb_err :1;                  // VF number error
         btUnsigned64bitInt funct_type_err :1;              // PF or VF error

      }; // end struct
   }; // end union

}; // end struct CCIP_FME_PCIE0_ERROR
CASSERT(sizeof(struct CCIP_FME_PCIE0_ERROR) == (1 *8));


struct CCIP_FME_PCIE1_ERROR {

   union {
      btUnsigned64bitInt csr;
      struct {
         btUnsigned64bitInt formattype_err :1;              // TLP Format/type error
         btUnsigned64bitInt MWAddr_err :1;                  // TLP MW Address error
         btUnsigned64bitInt MWAddrLength_err :1;            // TLP MW Length error
         btUnsigned64bitInt MRAddr_err :1;                  // TLP MR Address error
         btUnsigned64bitInt MRAddrLength_err :1;            // TLP MR Length error
         btUnsigned64bitInt cpl_tag_err :1;                 // TLP CPL TAP error
         btUnsigned64bitInt cpl_status_err :1;              // TLP CPL status error
         btUnsigned64bitInt cpl_timeout_err :1;             // TLP CPL Timeout error
         btUnsigned64bitInt rsvd :56;                       // Reserved
      }; // end struct
   }; // end union

}; // end struct CCIP_FME_PCIE1_ERROR
CASSERT(sizeof(struct CCIP_FME_PCIE1_ERROR) == (1 *8));

struct CCIP_FME_FIRST_ERROR {

   union {
      btUnsigned64bitInt csr;
      struct {
         btUnsigned64bitInt err_reg_status :60;          // Error Register trigger bit
         btUnsigned64bitInt errReg_id :4;                // Error Register trigger first
      }; // end struct
   }; // end union

}; // end struct CCIP_FME_ERROR
CASSERT(sizeof(struct CCIP_FME_FIRST_ERROR) == (1 *8));

struct CCIP_FME_NEXT_ERROR {

   union {
      btUnsigned64bitInt csr;
      struct {
         btUnsigned64bitInt err_reg_status :60;          // Error Register trigger bit
         btUnsigned64bitInt errReg_id :4;                // Error Register trigger first
      }; // end struct
   }; // end union

}; // end struct CCIP_FME_ERROR
CASSERT(sizeof(struct CCIP_FME_NEXT_ERROR) == (1 *8));



struct CCIP_FME_RAS_GERROR {

   union {
      btUnsigned64bitInt csr;
      struct {
         btUnsigned64bitInt temp_trash_ap1 :1;                // thremal threshold AP1
         btUnsigned64bitInt temp_trash_ap2 :1;                // thremal threshold AP2
         btUnsigned64bitInt pcie_error :1;                    // pcie Error
         btUnsigned64bitInt afufatal_error :1;                // afu fatal error
         btUnsigned64bitInt prochot_error :1;                 // proc hot error
         btUnsigned64bitInt afu_access_mismatch :1;           // afu access PF/VF mismatch
         btUnsigned64bitInt injected_warn_error :1;           // Injected warning  error
         btUnsigned64bitInt pcie_poison_error :1;             // PCIe poison port  error
         btUnsigned64bitInt gb_crc_err :1;                    // Green bitstream CRC Error
         btUnsigned64bitInt temp_trash_ap6 :1;                // thremal threshold AP6
         btUnsigned64bitInt power_trash_ap1 :1;               // Power threshold AP1
         btUnsigned64bitInt power_trash_ap2 :1;               // Power threshold AP6
         btUnsigned64bitInt mbp_error :1;                     // MBP error Error
         btUnsigned64bitInt rsvd2 :51;                        // Reserved
      }; // end struct
   }; // end union

}; // end struct CCIP_FME_RAS_GERROR
CASSERT(sizeof(struct CCIP_FME_RAS_GERROR) == (1 *8));


struct CCIP_FME_RAS_BERROR {

   union {
      btUnsigned64bitInt csr;
      struct {
         btUnsigned64bitInt ktilink_fatal_err :1;                  // KTI Link layer error detected
         btUnsigned64bitInt tagcch_fatal_err :1;                   // tag-n-cache error detected
         btUnsigned64bitInt cci_fatal_err :1;                      // CCI error detected
         btUnsigned64bitInt ktiprpto_fatal_err :1;                 // KTI Protocol error detected
         btUnsigned64bitInt dma_fatal_err :1;                      // DMA error detected
         btUnsigned64bitInt iommu_fatal_err :1;                    // IOMMU detected
         btUnsigned64bitInt injected_fatal_err :1;                 // Injected fatal error
         btUnsigned64bitInt rsvd :1;                               // Reserved
         btUnsigned64bitInt iommu_catast_err :1;                   // Catastrophic IOMMU Error
         btUnsigned64bitInt crc_catast_err :1;                     // Catastrophic CRC Error
         btUnsigned64bitInt therm_catast_err :1;                   // Catastrophic Thermal Error
         btUnsigned64bitInt injected_catast_err :1;                // Catastrophic Thermal Error
         btUnsigned64bitInt rsvd1 :52;                             // Reserved
      }; // end struct
   }; // end union

}; // end struct CCIP_FME_RAS_BERROR
CASSERT(sizeof(struct CCIP_FME_RAS_BERROR) == (1 *8));


struct CCIP_FME_RAS_WARNERROR {

   union {
      btUnsigned64bitInt csr;
      struct {
         btUnsigned64bitInt event_warn_err :1;                  // Green bitstream fatal event
         btUnsigned64bitInt rsvd :63;                           // Reserved
      }; // end struct
   }; // end union

}; // end struct CCIP_FME_RAS_WARNERROR
CASSERT(sizeof(struct CCIP_FME_RAS_WARNERROR) == (1 *8));


struct CCIP_FME_RAS_ERROR_INJ {

   union {
      btUnsigned64bitInt csr;
      struct {
         btUnsigned64bitInt catast_error :1;                  // sets this bit to enact a Catastrophic error
         btUnsigned64bitInt fatal_error :1;                   // sets this bit to enact a Fatal error
         btUnsigned64bitInt warning_error :1;                 // sets this bit to enact a Warning error
         btUnsigned64bitInt rsvd :61;                         // Reserved
      }; // end struct
   }; // end union

}; // end struct CCIP_FME_RAS_ERROR_INJ
CASSERT(sizeof(struct CCIP_FME_RAS_ERROR_INJ) == (1 *8));

///============================================================================
/// Name: CCIP_FME_DFL_GERROR
/// @brief   FPGA Management Engine Performance Global Error Feature
///  Feature ID: 0x4
///  Feature Type: = Private
///  Feature Revision: 0
///============================================================================
struct CCIP_FME_DFL_GERROR {

   // FME Global Error header
   struct CCIP_DFH                        ccip_gerror_dflhdr;

   // FME pcie0 error mask
   struct CCIP_FME_ERROR0                 fme_err_mask;

   // FME pcie0 error
   struct CCIP_FME_ERROR0                 fme_err;

   // FME pcie0 error mask
   struct CCIP_FME_PCIE0_ERROR            pcie0_err_mask;

   // FME pcie0 error
   struct CCIP_FME_PCIE0_ERROR            pcie0_err;

   // FME pcie1 error mask
   struct CCIP_FME_PCIE1_ERROR            pcie1_err_mask;

   // FME pcie1 error
   struct CCIP_FME_PCIE1_ERROR            pcie1_err;

   // FME first error
   struct CCIP_FME_FIRST_ERROR            fme_first_err;

   // FME next error
   struct CCIP_FME_NEXT_ERROR             fme_next_err;

   // FME RAS Green BS error Mask
   struct CCIP_FME_RAS_GERROR             ras_gerr_mask;

   // FME RAS Green BS error
   struct CCIP_FME_RAS_GERROR             ras_gerr;

   // FME RAS blue bitstream error mask
   struct CCIP_FME_RAS_BERROR             ras_berror_mask;

   // FME RAS blue bitstream error
   struct CCIP_FME_RAS_BERROR             ras_berror;

   // FME RAS warning error mask
   struct CCIP_FME_RAS_WARNERROR          ras_warnerror_mask;

   // FME RAS warning error
   struct CCIP_FME_RAS_WARNERROR          ras_warnerror;

   // FME RAS error injection
   struct CCIP_FME_RAS_ERROR_INJ          ras_error_inj;

}; //end CCIP_FME_GERROR_feature
CASSERT(sizeof(struct CCIP_FME_DFL_GERROR) ==(16* 8));

///============================================================================
/// Name: CCIP_FME_DFL_PR
/// @brief   FPGA Management Engine Partial Reconfiguration Feature
///  Feature ID: 0x5
///  Feature Type: = Private
///  Feature Revision: 0
///============================================================================

struct CCIP_FME_PR_KEY {

   union {
      btUnsigned64bitInt csr;
      struct {
         btUnsigned64bitInt key :64;
      }; // end struct
   }; // end union

}; // end struct CCIP_FME_PR_KEY
CASSERT(sizeof(struct CCIP_FME_PR_KEY) == (1 *8));

//Partial Reconfiguration control  CSR
struct CCIP_FME_PR_CONTROL {

   union {
      btUnsigned64bitInt csr;
      struct {
         btUnsigned64bitInt pr_reset :1;                       // Reset PR Engine
         btUnsigned64bitInt rsvd3 :3;                          // Reserved
         btUnsigned64bitInt pr_reset_ack :1;                   // Reset PR Engine Ack
         btUnsigned64bitInt rsvd4 :3;                          // Reserved
         btUnsigned64bitInt pr_regionid :2;                    // PR Region ID
         btUnsigned64bitInt rsvd1 :2;                          // Reserved
         btUnsigned64bitInt pr_start_req :1;                   // PR Start Request
         btUnsigned64bitInt pr_push_complete :1;               // PR Data push complete
         btUnsigned64bitInt pr_kind :1;                        // PR Data push complete
         btUnsigned64bitInt rsvd :49;                          // Reserved
      }; // end struct
   }; //end union
}; // end struct CCIP_FME_PR_CONTROL
CASSERT(sizeof(struct CCIP_FME_PR_CONTROL) == (1 *8));

//Partial Reconfiguration Status  CSR
struct CCIP_FME_PR_STATUS {

   union {
      btUnsigned64bitInt csr;
      struct {
         btUnsigned64bitInt pr_credit :9;                // PR Credits
         btUnsigned64bitInt rsvd2 :7;                    // Reserved
         btUnsigned64bitInt pr_status :1;                // PR status
         btUnsigned64bitInt rsvd :3;                     // Reserved
         btUnsigned64bitInt pr_contoller_status :3;      // Altra PR Controller Block status
         btUnsigned64bitInt rsvd1 :1;                    // Reserved
         btUnsigned64bitInt pr_host_status :4;           // PR Host status
         btUnsigned64bitInt rsvd3 :36;                   // Reserved
      }; // end struct
   }; // end union
};// end struct CCIP_FME_PR_STATUS
CASSERT(sizeof(struct CCIP_FME_PR_STATUS) == (1 *8));

//Partial Reconfiguration data  CSR
struct CCIP_FME_PR_DATA {
   union {
      btUnsigned64bitInt csr;
      struct {
         btUnsigned64bitInt pr_data_raw :32;             // PR data from the raw binary file
         btUnsigned64bitInt rsvd :32;                    // Reserved
      }; // end struct
   }; // end union
}; // end struct CCIP_FME_PR_DATA
CASSERT(sizeof(struct CCIP_FME_PR_DATA) == (1 *8));


//Partial Reconfiguration data  CSR
 struct CCIP_FME_PR_ERROR {
    union {
       btUnsigned64bitInt csr;
       struct {
          btUnsigned64bitInt PR_operation_err :1;           // Previous errors detected by PR Host
          btUnsigned64bitInt PR_CRC_err :1;                 // CRC errors detected
          btUnsigned64bitInt PR_bitstream_err :1;           // InCompatible errors detected
          btUnsigned64bitInt PR_IP_err :1;                  // PR protocol error
          btUnsigned64bitInt PR_FIFIO_err :1;               // PR FIFO error
          btUnsigned64bitInt PR_timeout_err :1;             // PR Timeout error
          btUnsigned64bitInt PR_secure_load_err :1;         // PR Security error
          btUnsigned64bitInt rsvd : 57;                     // Reserved
       }; // end struct
    }; // end union
 }; // end struct CCIP_FME_PR_ERROR
 CASSERT(sizeof(struct CCIP_FME_PR_ERROR) == (1 *8));

struct CCIP_FME_DFL_PR {

   // FME PR device feature header
   struct CCIP_DFH                 ccip_pr_dflhdr;

   //Partial Reconfiguration control  CSR
   struct CCIP_FME_PR_CONTROL      ccip_fme_pr_control;

   //Partial Reconfiguration Status  CSR
   struct CCIP_FME_PR_STATUS       ccip_fme_pr_status;

   //Partial Reconfiguration data  CSR
   struct CCIP_FME_PR_DATA         ccip_fme_pr_data;

   //Partial Reconfiguration data  CSR
   struct CCIP_FME_PR_ERROR        ccip_fme_pr_err;

   // FME PR Publish HASH
   struct CCIP_FME_PR_KEY fme_pr_pub_harsh0 ;
   struct CCIP_FME_PR_KEY fme_pr_pub_harsh1 ;
   struct CCIP_FME_PR_KEY fme_pr_pub_harsh2 ;
   struct CCIP_FME_PR_KEY fme_pr_pub_harsh3 ;

   // FME PR Private HASH
   struct CCIP_FME_PR_KEY fme_pr_priv_harsh0 ;
   struct CCIP_FME_PR_KEY fme_pr_priv_harsh1 ;
   struct CCIP_FME_PR_KEY fme_pr_priv_harsh2 ;
   struct CCIP_FME_PR_KEY fme_pr_priv_harsh3 ;

   // FME PR License
   struct CCIP_FME_PR_KEY fme_pr_license0 ;
   struct CCIP_FME_PR_KEY fme_pr_license1 ;
   struct CCIP_FME_PR_KEY fme_pr_license2 ;
   struct CCIP_FME_PR_KEY fme_pr_license3 ;

   // FME PR Session Key
   struct CCIP_FME_PR_KEY fme_pr_seskey0 ;
   struct CCIP_FME_PR_KEY fme_pr_seskey1 ;
   struct CCIP_FME_PR_KEY fme_pr_seskey2 ;
   struct CCIP_FME_PR_KEY fme_pr_seskey3 ;

   // PR Interface ID
   struct CCIP_FME_PR_KEY fme_pr_intfc_id0_l;
   struct CCIP_FME_PR_KEY fme_pr_intfc_id0_h;

   struct CCIP_FME_PR_KEY fme_pr_intfc_id1_l;
   struct CCIP_FME_PR_KEY fme_pr_intfc_id1_h ;

   struct CCIP_FME_PR_KEY fme_pr_intfc_id2_l;
   struct CCIP_FME_PR_KEY fme_pr_intfc_id2_h ;

   struct CCIP_FME_PR_KEY fme_pr_intfc_id3_l;
   struct CCIP_FME_PR_KEY fme_pr_intfc_id3_h ;

}; //end struct  CCIP_FME_GERROR_feature
CASSERT(sizeof(struct CCIP_FME_DFL_PR) ==(29* 8));

/******************************************************************************
 *  FPGA port header
 *  Type  = afu
 *  CCI-P version =0x1
 *  CCI-P minor version =0x0
 *  Header Description:
 *
 ******************************************************************************/

struct CCIP_USER_CLK_FREQ {

   union {
      btUnsigned64bitInt csr;
      struct {
         btUnsigned64bitInt usr_clk_frq :64;

      }; // end struct
   }; // end union

}; // end struct CCIP_USER_CLK_FREQ
CASSERT(sizeof(struct CCIP_USER_CLK_FREQ) == (1 *8));


// Port status
struct CCIP_PORT_STATUS {
   union {
      btUnsigned64bitInt csr;
      struct {
         btUnsigned64bitInt port_freeze :1;           // Port freeze
         btUnsigned64bitInt rsvd1 :7;                 // Reserved
         btUnsigned64bitInt afu_pwr_state :4;         // Reports AFU Power state
         btUnsigned64bitInt ap1_event :1;             // AP1 event
         btUnsigned64bitInt ap2_event :1;             // AP2 event
         btUnsigned64bitInt rsvd :46;                 // Reserved
      }; // end struct
   }; // end union

}; // end struct CCIP_PORT_STATUS
CASSERT(sizeof(struct CCIP_PORT_STATUS) == (1 *8));

// Port status
struct CCIP_PORT_HDR {

   // port  header
   struct CCIP_DFH                  ccip_port_dfh;

   // Port afu id  low
   struct CCIP_AFU_ID_L             ccip_port_afuidl;

   // Port afu id  low
   struct CCIP_AFU_ID_H             ccip_port_afuidh;

   // Next AFU offset
   struct CCIP_NEXT_AFU             ccip_port_next_afu;

   // Reserved
   btUnsigned64bitInt               rsvd_porthdr;

   // Port Scratch pad
   struct CCIP_SCRATCHPAD           ccip_port_scratchpad;

   //  Port scratch pad
   struct CCIP_PORT_CAPABILITY {

      union {
         btUnsigned64bitInt csr;
         struct {
            btUnsigned64bitInt port_id :2;         // CCI-P port ID
            btUnsigned64bitInt rsvd1 :6;           // Reserved
            btUnsigned64bitInt mmio_size :16;      // MMIO size
            btUnsigned64bitInt rsvd3 :8;           // Reserved
            btUnsigned64bitInt interrupts :4;      //Interrupts supported
            btUnsigned64bitInt rsvd :28;           // Reserved
         }; // end struct
      }; // end union

   }ccip_port_capability; // end  CCIP_PORT_CAPABILITY

   // Port control
   struct CCIP_PORT_CONTROL {
      union {
         btUnsigned64bitInt csr;
         struct {
            btUnsigned64bitInt port_sftreset_control :1;       // port soft reset control
            btUnsigned64bitInt rsvd1 :1;                       // Reserved
            btUnsigned64bitInt afu_latny_rep :1;               // AFU Latency Tolerance Reorting
            btUnsigned64bitInt rsvd2 :1;                       // Reserved
            btUnsigned64bitInt ccip_outstanding_request :1;    // NO outstanding CCI-P requests, set to 1
            btUnsigned64bitInt rsvd :59;                       // Reserved
         }; // end struct
      }; // end union

   }ccip_port_control; // end struct CCIP_PORT_CONTROL

   // Port status
   struct CCIP_PORT_STATUS ccip_port_status;

   // Reserved
   btUnsigned64bitInt port_rsvd1_hdr;

   struct CCIP_USER_CLK_FREQ user_clk_freq_cmd0;
   struct CCIP_USER_CLK_FREQ user_clk_freq_cmd1;

   struct CCIP_USER_CLK_FREQ user_clk_freq_sts0;
   struct CCIP_USER_CLK_FREQ user_clk_freq_sts1;

}; // end struct CCIP_FME_HDR
CASSERT(sizeof(struct CCIP_PORT_HDR) == (14 *8));


/******************************************************************************
 *  64-bit FPGA Port Error  Feature List
 *  Feature ID =0x10
 *  Feature Description Port Error Management
 *  Feature Type = Private
 *  offset 0x40 bytes form Device Feature Header
 ******************************************************************************/

// CCIP Port Error CSR
struct CCIP_PORT_ERROR {
      union {
         btUnsigned64bitInt csr;
         struct {
            btUnsigned64bitInt tx_ch0_overflow :1;             // Tx Channel0 : Overflow
            btUnsigned64bitInt tx_ch0_invalidreq :1;           // Tx Channel0 : Invalid request encoding
            btUnsigned64bitInt tx_ch0_req_cl_len3 :1;          // Tx Channel0 : Request with cl_len=3
            btUnsigned64bitInt tx_ch0_req_cl_len2 :1;          // Tx Channel0 : Request with cl_len=2
            btUnsigned64bitInt tx_ch0_req_cl_len4 :1;          // Tx Channel0 : Request with cl_len=4

            btUnsigned64bitInt rsvd :4;                        // Reserved
            btUnsigned64bitInt afummio_rdrecv_portreset :1;    // AFU MMIO RD received while PORT is in reset
            btUnsigned64bitInt afummio_wrrecv_portreset :1;    // AFU MMIO WR received while PORT is in reset
            btUnsigned64bitInt rsvd5 :5;                       // Reserved

            btUnsigned64bitInt tx_ch1_overflow :1;             // Tx Channel1 : Overflow
            btUnsigned64bitInt tx_ch1_invalidreq :1;           // Tx Channel1 : Invalid request encoding
            btUnsigned64bitInt tx_ch1_req_cl_len3 :1;          // Tx Channel1 : Request with cl_len=3
            btUnsigned64bitInt tx_ch1_req_cl_len2 :1;          // Tx Channel1 : Request with cl_len=2
            btUnsigned64bitInt tx_ch1_req_cl_len4 :1;          // Tx Channel1 : Request with cl_len=4

            btUnsigned64bitInt tx_ch1_insuff_datapayload :1;   // Tx Channel1 : Insufficient data payload
            btUnsigned64bitInt tx_ch1_datapayload_overrun:1;   // Tx Channel1 : Data payload overrun
            btUnsigned64bitInt tx_ch1_incorr_addr  :1;         // Tx Channel1 : Incorrect address
            btUnsigned64bitInt tx_ch1_sop_detcted  :1;         // Tx Channel1 : NON-Zero SOP Detected
            btUnsigned64bitInt rsvd1 :7;                       // Reserved

            btUnsigned64bitInt mmioread_timeout :1;            // MMIO Read Timeout in AFU
            btUnsigned64bitInt tx_ch2_fifo_overflow :1;        // Tx Channel2 : FIFO overflow
            btUnsigned64bitInt unexp_mmio_resp :1;             // MMIO read is not matching pending request

            btUnsigned64bitInt rsvd2 :5;                       // Reserved

            btUnsigned64bitInt num_pending_req_overflow :1;    // Number of pending Requests: counter overflow
            btUnsigned64bitInt llpr_smrr_err :1;               // Request with Address violating SMM Range
            btUnsigned64bitInt llpr_smrr2_err :1;              // Request with Address violating second SMM Range
            btUnsigned64bitInt llpr_mesg_err :1;               // Request with Address violating ME Stolen message
            btUnsigned64bitInt genport_range_err :1;           // Request with Address violating Generic protect range
            btUnsigned64bitInt legrange_low_err :1;            // Request with Address violating Legacy Range low
            btUnsigned64bitInt legrange_hight_err :1;          // Request with Address violating Legacy Range High
            btUnsigned64bitInt vgmem_range_err :1;             // Request with Address violating VGA memory range
            btUnsigned64bitInt page_fault_err :1;              // Page fault
            btUnsigned64bitInt pmr_err :1;                     // PMR Error
            btUnsigned64bitInt ap6_event :1;                   // AP6 event
            btUnsigned64bitInt vfflr_accesseror:1;             // VF FLR detected on Port with PF access control.

            btUnsigned64bitInt rsvd3 :12;                      // Reserved
         }; // end struct
      }; // end union

};
CASSERT(sizeof(struct CCIP_PORT_ERROR) == (1 *8));


struct CCIP_PORT_DFL_ERR {

   // Port Error Header
   struct CCIP_DFH                  ccip_port_err_dflhdr;

   // Port error Mask
   struct CCIP_PORT_ERROR           ccip_port_error_mask;

   //Port Error
   struct CCIP_PORT_ERROR           ccip_port_error;

   //Port First Error
   struct CCIP_PORT_ERROR           ccip_port_first_error;

   // port malformed request0
   struct CCIP_PORT_MALFORMED_REQ_0 {

      union {
         btUnsigned64bitInt csr;
         struct {
            btUnsigned64bitInt malfrd_req_lsb :64;    // LSB header bit first malformed request
         }; // end struct
      }; // end union

   }ccip_port_malformed_req_0; // end struct CCIP_PORT_MALFORMED_REQ_0

   // port  malformed request1
   struct CCIP_PORT_MALFORMED_REQ_1 {

      union {
         btUnsigned64bitInt csr;
         struct {
            btUnsigned64bitInt malfrd_req_msb :64;    // MSB header bit first malformed request
         }; // end struct
      }; // end union

   }ccip_port_malformed_req_1;  // end struct CCIP_PORT_MALFORMED_REQ_1

   // Port Debug
   struct CCIP_PORT_DEBUG {

      union {
         btUnsigned64bitInt csr;
         struct {
            btUnsigned64bitInt port_debug :64;    // port debug register
         }; // end struct
      }; // end union

   }ccip_port_debug;  // end struct CCIP_PORT_DEBUG


}; // end struct CCIP_PORT_DFL_ERR
CASSERT(sizeof(struct CCIP_PORT_DFL_ERR ) == (7 *8));

/******************************************************************************
 *  FPGA Port USMG Feature
 *  Feature ID =0x11
 *  Feature Description Port USMG
 *  Feature Type = Private
 *
 ******************************************************************************/
struct CCIP_PORT_DFL_UMSG {

   // Port usmg Header
   struct CCIP_DFH ccip_port_umsg_dflhdr;

   // umsg capability
   struct CCIP_UMSG_CAPABILITY {

      union {
         btUnsigned64bitInt csr;
         struct {
            btUnsigned64bitInt no_umsg_alloc_port :8;    // number of umsg allocated to this port
            btUnsigned64bitInt status_umsg_engine :1;    // enable umsg engine for this ort 1-enable 0-disable
            btUnsigned64bitInt umsg_init_status :1;      // usmg initialization status
            btUnsigned64bitInt umsg_trans_error :1;      // IOMMU not able to translate of the umsg base address
            btUnsigned64bitInt rsvd :53;                 // Reserved
         }; // end struct
      }; // end union

   }ccip_umsg_capability; // end struct CCIP_UMSG_CAPABILITY


   // umsg base address
   struct CCIP_UMSG_BASE_ADDRESS {

      union {
         btUnsigned64bitInt csr;
         struct {
            btUnsigned64bitInt umsg_base_address :48;    // UMAS segment start physical byte address
            btUnsigned64bitInt rsvd :16;                 // Reserved

         }; // end struct
      }; // end union

   }ccip_umsg_base_address; // end struct CCIP_UMSG_BASE_ADDRESS


   // umsg mode
   struct CCIP_UMSG_MODE {

      union {
         btUnsigned64bitInt csr;
         struct {
            btUnsigned64bitInt umsg_hit :8;          // UMSG hit enable / disable
            btUnsigned64bitInt rsvd :56;             // Reserved
         }; // ens struct
      }; // end union

   }ccip_umsg_mode; // end struct CCIP_UMSG_MODE


}; // end struct CCIP_PORT_UMSG_feature
CASSERT(sizeof(struct CCIP_PORT_DFL_UMSG) == (4 *8));


/******************************************************************************
 *  64-bit FPGA Port/afu Partial Reconfiguration Feature list
 *  Feature ID =0x12
 *  Feature Description Port/ AFU partial reconfiguration
 *  Feature Type = Private
 *
 ******************************************************************************/
struct CCIP_PORT_DFL_PR {

   // Port usmg Header
   struct CCIP_DFH ccip_port_pr_dflhdr;

   // Partial Receconfiguration Control
   struct CCIP_PORT_PR_CONTROL {

      union {
         btUnsigned64bitInt csr;
         struct {
            btUnsigned64bitInt pr_start_req :1; // PR start Request
            btUnsigned64bitInt rsvd :63;
         }; // end struct
      }; // end union

   }ccip_port_pr_control; // end struct CCIP_PORT_PR_CONTROL


   // Partial Reconfiguration status
   struct CCIP_PORT_PR_STATUS {
      union {
         btUnsigned64bitInt csr;
         struct {
            btUnsigned64bitInt pr_credit :9;             // Credits
            btUnsigned64bitInt rsvd2 :3;                 // Reserved
            btUnsigned64bitInt pr_acces_grant :1;        // PR access Grant
            btUnsigned64bitInt pr_access :1;             // PR access enabled
            btUnsigned64bitInt rsvd3 :2;                 // Reserved
            btUnsigned64bitInt pr_timeout_error :1;      // PR port timeout error
            btUnsigned64bitInt pr_engine_error :1;       // PR Engine error
            btUnsigned64bitInt pr_data_ovrferr :1;       // PR data overfow error
            btUnsigned64bitInt rsvd4 :1;                 // Reserved
            btUnsigned64bitInt pr_mega_fstatus :3;       // Altra PR Mega-function status
            btUnsigned64bitInt rsvd1 :1;                 // Reserved
            btUnsigned64bitInt pr_status :4;             // PR Status
            btUnsigned64bitInt rsvd :35;                 // Reserved
         }; // end struct
      };  // end union

   }ccip_port_pr_status; // end struct CCIP_PORT_PR_STATUS

   // Partial Receconfiguration Data
   struct CCIP_PORT_PR_DATA {

      union {
         btUnsigned64bitInt csr;
         struct {
            btUnsigned64bitInt pr_data_raw :32;          // PR data from the raw-binary file
            btUnsigned64bitInt rsvd :32;                 // Reserved
         }; // end struct
      }; // end union

   }ccip_port_pr_data; // end struct CCIP_PORT_PR_DATA

   struct CCIP_PORT_PR_PBUDGET {

      union {
         btUnsigned64bitInt csr;
         struct {
            btUnsigned64bitInt pwr_format :48;           // AFU Power budget format
            btUnsigned64bitInt rsvd :16;                 // Reserved

         }; // end struct
      }; // end union

   }ccip_port_pr_pbudget; // end struct CCIP_PORT_PR_PBUDGET


   // User Clock Frequency
   struct CCIP_USR_CLK_FREQ {

      union {
         btUnsigned64bitInt csr;
         struct {
            btUnsigned64bitInt rsvd :64;                   // Reserved
         }; // end union
      }; // end stuct

   }ccip_usr_clk_freq; // end struct CCIP_USR_CLK_FREQ


}; // end struct CCIP_PORT_UMSG_feature
CASSERT(sizeof(struct CCIP_PORT_DFL_PR) == (6 *8));


/******************************************************************************
 *  FPGA Port signal tap  feature list
 *  Feature ID =0x13
 *  Feature Description Port signal Tap
 *  Feature Type = Private
 *
 ******************************************************************************/


// Port Signal Tap Error
struct CCIP_PORT_STAP_ERROR {
   union {
      btUnsigned64bitInt csr;
      struct {
         btUnsigned64bitInt rw_timeout :1;             // SLD HUB end-point read/write timeout.
         btUnsigned64bitInt stp_inreset :1;            // Remote SignalTap in Reset/ disabled .
         btUnsigned64bitInt unsupport_read :1;         // Unsupported Read
         btUnsigned64bitInt mmio_timeout :1;           // MMIO timeout detected
         btUnsigned64bitInt txffio_count :4;           // Tx FFIO count
         btUnsigned64bitInt rxffio_count :4;           // Rx FFIO count
         btUnsigned64bitInt txffio_overflow :4;        // Tx FFIO overflow
         btUnsigned64bitInt txffio_underflow :4;       // Tx FFIO underflow
         btUnsigned64bitInt rxffio_overflow :4;        // Rx FFIO overflow
         btUnsigned64bitInt rxffio_underflow :4;       // Rx FFIO underflow
         btUnsigned64bitInt num_mmiowr :12;            // Number of MMIO Writes
         btUnsigned64bitInt num_mmioreq :12;           // Number of MMIO Reads
         btUnsigned64bitInt num_mmioresp :12;          // Number of MMIO Responses

      }; // end struct
   }; // end union

}; // end struct CCIP_PORT_STAP_ERROR
CASSERT(sizeof(struct CCIP_PORT_STAP_ERROR) == (1 *8));

struct CCIP_PORT_DFL_STAP {

   // Port usmg Header
   struct CCIP_DFH ccip_port_stap_dflhdr;

   // Remote Signal Tap Error
   struct CCIP_PORT_STAP_ERROR stap_error;

}; // end struct CCIP_PORT_STAP
CASSERT(sizeof(struct CCIP_PORT_DFL_STAP) == (2*8));


/******************************************************************************
 *  FPGA AFU Header
 *  Feature ID = AFU
 *  Feature Description: AFU header
 *  Feature Type = Private
 *
 ******************************************************************************/
struct CCIP_AFU_Header {

   // FME  Header
   struct CCIP_DFH  ccip_dfh;

   // FME afu id  low
   struct CCIP_AFU_ID_L ccip_afu_id_l;

   // FME afu id  low
   struct CCIP_AFU_ID_H  ccip_afu_id_h;

   // Next AFU offset
   struct CCIP_NEXT_AFU  ccip_next_afu;
} ;
CASSERT(sizeof(struct CCIP_AFU_Header) == (4*8));


/// @} group aalkernel_ccip



END_NAMESPACE(AAL)

#endif /* __AALKERNEL_CCIP_SIM_DEF_H_ */
