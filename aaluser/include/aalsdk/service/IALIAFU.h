// Copyright(c) 2015-2016, Intel Corporation
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
/// @file IALIAFU.h
/// @brief AFU Link Interface (ALI)
/// @ingroup IALIAFU
/// @verbatim
/// Intel(R) QuickAssist Technology Accelerator Abstraction Layer
/// AFU Link Interface (ALI) definition
///
/// This header defines the various interfaces exposed by the AFU Link
/// Interface. There is no specific separate IALIAFU class. The individual
/// interfaces are:
///
///    IALIMMIO   Functions for accessing AFU MMIO space and discovering
///               features used by the AFU, such as MPF
///    IALIUMsg   Functions for sending UMessages
///    IALIBuffer Functions for allocating shared buffers between software
///               and the AFU
///    IALIPerf   Functions for accessing performance counters
///    IALIReset  Functions for enabling, disabling, quiescing, and resetting
///               the AFU
///    IALIReconfigure
///               Functions for partial reconfiguration
///    IALISignalTap
///               Functions for remote debugging
///
/// Applications or services that need to interact with FPGA-implemented
/// functions (i.e AFUs) will typically allocate an ALI service and obtain
/// one or more of the above interfaces.
//////
/// All ALI IID's will derive from Intel-specific sub-system INTC_sysAFULinkInterface.
///
/// An ALI Service will support zero to all of the following Services Interfaces:
///   iidALI_MMIO_Service         __INTC_IID(INTC_sysAFULinkInterface,0x0001)
///   iidALI_UMSG_Service         __INTC_IID(INTC_sysAFULinkInterface,0x0002)
///   iidALI_BUFF_Service         __INTC_IID(INTC_sysAFULinkInterface,0x0003)
///   iidALI_BUFF_Service_Client  __INTC_IID(INTC_sysAFULinkInterface,0x0004)
///   iidALI_PERF_Service         __INTC_IID(INTC_sysAFULinkInterface,0x0005)
///   iidALI_RSET_Service         __INTC_IID(INTC_sysAFULinkInterface,0x0006)
///   iidALI_CONF_Service         __INTC_IID(INTC_sysAFULinkInterface,0x0007)
///   iidALI_CONF_Service_Client  __INTC_IID(INTC_sysAFULinkInterface,0x0008)
///   iidALI_STAP_Service         __INTC_IID(INTC_sysAFULinkInterface,0x0009)
/// <TODO: LIST INTERFACES HERE>
///
/// If an ALI Service Client needs any particular Service Interface, then it must check at runtime
///    that the returned Service pointer supports the needed Interface and take appropriate action,
///    e.g. possibly failing to start and issuing an appropriate error message, or taking other
///    corrective action if possible.
/// For example:
/// @code
/// void serviceAllocated( IBase *pServiceBase, TransactionID const &rTranID) {
///    ASSERT( pServiceBase );         // if false, then Service threw a bad pointer
///
///    IAALService *m_pAALService;     // used to call Release on the Service
///    m_pAALService = dynamic_ptr<IAALService>( iidService, pServiceBase);
///    ASSERT( m_pAALService );
///
///    IALIMMIO *m_pMMIOService;       // used to call MMIO methods on the Service
///    m_pMMIOService = dynamic_ptr<IALIMMIO>( iidALI_MMIO_Service, pServiceBase);
///    ASSERT( m_pMMIOService );
///
///    IALIBUFFER *m_pBufferService;   // used to call BUFF methods on the Service
///    m_pBUFFERService = dynamic_ptr<IALIBuffer>( iidALI_BUFFER_Service, pServiceBase);
///    ASSERT( m_pBUFFERService );
///
///    <TODO: ADD EXAMPLES HERE>
/// }
/// @endcode
///
/// AUTHORS: Henry Mitchel, Intel Corporation
///          Joseph Grecco, Intel Corporation
///          Ananda Ravuri, Intel Corporation
///          Enno Luebbers, Intel Corporation
///
/// HISTORY:
/// WHEN:          WHO:     WHAT:
/// 07/20/2015     AR       Initial version derived from CCI
/// 08/XX/2015     HM       New material@endverbatim
//****************************************************************************
#ifndef __AALSDK_SERVICE_IALIAFU_H__
#define __AALSDK_SERVICE_IALIAFU_H__
#include <aalsdk/OSAL.h>
#include <aalsdk/AALTypes.h>
#include <aalsdk/AALIDDefs.h>
#include <aalsdk/INTCDefs.h>
#include <aalsdk/AASLib.h>

BEGIN_NAMESPACE(AAL)

// FIXME: declare this where it should be declared...
#define ALI_MMAP_TARGET_VADDR_KEY        "ALIMmapTargetVAddr"
#define ALI_MMAP_TARGET_VADDR_DATATYPE   void *
#define ALI_GETFEATURE_ID_KEY            "ALIGetFeatureID"
#define ALI_GETFEATURE_ID_DATATYPE       btUnsigned64bitInt
#define ALI_GETFEATURE_TYPE_KEY          "ALIGetFeatureTYPE"
#define ALI_GETFEATURE_TYPE_DATATYPE     btUnsigned64bitInt
#define ALI_GETFEATURE_GUID_KEY          "ALIGetFeatureGUID"
#define ALI_GETFEATURE_GUID_DATATYPE     btcString

// CCIP DFH header types
#define ALI_DFH_TYPE_RSVD    0
#define ALI_DFH_TYPE_AFU     1
#define ALI_DFH_TYPE_BBB     2
#define ALI_DFH_TYPE_PRIVATE 3



/// @addtogroup IALIAFU
/// @{


//-----------------------------------------------------------------------------
// Request message IDs. Subject to change.
//-----------------------------------------------------------------------------
typedef enum
{
   ali_errnumOK = 0,
   ali_errnumBadDevHandle,                       // 1
   ali_errnumCouldNotClaimDevice,                // 2
   ali_errnumNoAppropriateInterface,             // 3
   ali_errnumDeviceHasNoPIPAssigned,             // 4
   ali_errnumCouldNotBindPipInterface,           // 5
   ali_errnumCouldNotUnBindPipInterface,         // 6
   ali_errnumNotDeviceOwner,                     // 7
   ali_errnumSystem,                             // 8
   ali_errnumAFUTransaction,                     // 9
   ali_errnumAFUTransactionNotFound,             // 10
   ali_errnumDuplicateStartingAFUTransactionID,  // 11
   ali_errnumBadParameter,                       // 12
   ali_errnumNoMem,                              // 13
   ali_errnumNoMap,                              // 14
   ali_errnumBadMapping,                         // 15
   ali_errnumPermission,                         // 16
   ali_errnumInvalidOpOnMAFU,                    // 17
   ali_errnumPointerOutOfWorkspace,              // 18
   ali_errnumNoAFUBindToChannel,                 // 19
   ali_errnumCopyFromUser,                       // 20
   ali_errnumDescArrayEmpty,                     // 21
   ali_errnumCouldNotCreate,                     // 22
   ali_errnumInvalidRequest,                     // 23
   ali_errnumInvalidDeviceAddr,                  // 24
   ali_errnumCouldNotDestroy,                    // 25
   ali_errnumDeviceBusy,                         // 26
   ali_errnumTimeout                             // 27
} ali_errnum_e;


//-----------------------------------------------------------------------------
// ALI Interface IIDs.
//-----------------------------------------------------------------------------
#define iidALI_MMIO_Service         __INTC_IID(INTC_sysAFULinkInterface,0x0001)
#define iidALI_UMSG_Service         __INTC_IID(INTC_sysAFULinkInterface,0x0002)

#define iidALI_BUFF_Service         __INTC_IID(INTC_sysAFULinkInterface,0x0003)
#define iidALI_BUFF_Service_Client  __INTC_IID(INTC_sysAFULinkInterface,0x0004)

#define iidALI_PERF_Service         __INTC_IID(INTC_sysAFULinkInterface,0x0005)
#define iidALI_RSET_Service         __INTC_IID(INTC_sysAFULinkInterface,0x0006)

#define iidALI_CONF_Service         __INTC_IID(INTC_sysAFULinkInterface,0x0007)
#define iidALI_CONF_Service_Client  __INTC_IID(INTC_sysAFULinkInterface,0x0008)
#define ALI_AFUID_UAFU_CONFIG       "A3AAB285-79A0-4572-83B5-4FD5E5216870"

#define iidALI_STAP_Service         __INTC_IID(INTC_sysAFULinkInterface,0x0009)
#define CCIP_STAP_AFUID             "022F85B1-2CC2-4C9D-B6B0-3A385883AB8D"

/// Key for selecting an AFU delegate.
// TODO: consolidate naming scheme for NVS keys and datatypes
#define ALIAFU_NVS_KEY_TARGET "ALIAFUTarget"
/// Value - selects ASECCIAFU
# define ALIAFU_NVS_VAL_TARGET_ASE   "ALIAFUTarget_ASE"
/// Value - selects HWCCIAFU
# define ALIAFU_NVS_VAL_TARGET_FPGA  "ALIAFUTarget_FPGA"
/// Value - selects SWSimCCIAFU
# define ALIAFU_NVS_VAL_TARGET_SWSIM "ALIAFUTarget_SWSim"


//-----------------------------------------------------------------------------
// IALIMMIO interface.
//-----------------------------------------------------------------------------
/// @brief  Provide access to the MMIO region exposed by the AFU to the Application.
/// @note   There is no client for this Service Interface because all of its methods
///            are synchronous (and fast)
/// @note   This service interface is obtained from an IBase via iidALI_MMIO_Service
/// @code
///         m_pALIMMIOService = dynamic_ptr<IALIMMIO>(iidALI_MMIO_Service, pServiceBase);
/// @endcode
///
class IALIMMIO
{
public:
   virtual ~IALIMMIO() {}

   /// @brief Obtain the user virtual address of the mmapped MMIO region.
   virtual btVirtAddr   mmioGetAddress( void ) = 0;

   /// @brief Obtain the user virtual address of the mmapped MMIO region.
   virtual btCSROffset  mmioGetLength( void ) = 0;

   /// Convenience functions for those who organize an MMIO space as a set of Registers

   /// @brief      Read an MMIO address (or register) as a 32-bit value.
   /// @note       Synchronous function; no TransactionID. Generally very fast.
   /// @param[in]  Offset Byte offset into the MMIO region at which to read.
   /// @param[out] pValue Where to place value read.
   /// @return     True if the read was successful.
   virtual btBool  mmioRead32( const btCSROffset Offset, btUnsigned32bitInt * const pValue) = 0;

   /// @brief      Write an MMIO address (or register) as a 32-bit value.
   /// @note       Synchronous function; no TransactionID. Generally very fast.
   /// @param[in]  Offset Byte offset into the MMIO region at which to write.
   /// @param[in]  Value  Value to write.
   /// @return     True if the write was successful.
   virtual btBool  mmioWrite32( const btCSROffset Offset, const btUnsigned32bitInt Value) = 0;

   /// @brief      Read an MMIO address (or register) as a 64-bit value.
   /// @note       Synchronous function; no TransactionID. Generally very fast.
   /// @param[in]  Offset Byte offset into the MMIO region at which to read.
   /// @param[out] pValue Where to place value read.
   /// @return     True if the read was successful.
   virtual btBool  mmioRead64( const btCSROffset Offset, btUnsigned64bitInt * const pValue) = 0;

   /// @brief      Write an MMIO address (or register) as a 64-bit value.
   /// @note       Synchronous function; no TransactionID. Generally very fast.
   /// @param[in]  Offset Byte offset into the MMIO region at which to write.
   /// @param[in]  Value  Value to write.
   /// @return     True if the write was successful.
   virtual btBool  mmioWrite64( const btCSROffset Offset, const btUnsigned64bitInt Value) = 0;

   /// @brief      Request pointer to a device feature header (DFH).
   ///
   /// Will deposit in *pFeatureAddr the base address of the device feature
   /// MMIO space (aka the address of the respective feature header) for a
   /// particular device feature that was requested. Users can request specific
   /// features by ID, type, and/or GUID, by supplying a NamedValueSet in
   /// rInputArgs. Possible keys are:
   ///
   /// ALI_GETFEATURE_ID      to request a feature with a specific feature ID
   /// ALI_GETFEATURE_GUID    to request a feature with a specific GUID ID
   /// ALI_GETFEATURE_TYPE    to request a feature with a specific feature type
   ///                        (BBB, private feature, …?)
   ///
   /// The function will return the ID, GUID, and TYPE of the first matching
   /// feature in rOutputArgs, if supplied (optional argument via overloading).
   ///
   /// @note       Synchronous function; no TransactionID. Generally very fast.
   /// @param[out] pFeature    Where to place the address of the feature header.
   /// @param[in]  rInputArgs  Arguments specifying which feature to search for.
   /// @param[out] rOutputArgs Additional properties of the returned feature.
   /// @return     True if requested feature was found in DFH space.
   // TODO: do we want this to return a valid pointer, or an offset to be used with the above functions?
   virtual btBool  mmioGetFeatureAddress( btVirtAddr          *pFeature,
                                          NamedValueSet const &rInputArgs,
                                          NamedValueSet       &rOutputArgs ) = 0;
   // overloaded version without rOutputArgs
   virtual btBool  mmioGetFeatureAddress( btVirtAddr          *pFeature,
                                          NamedValueSet const &rInputArgs ) = 0;
   // version that returns an MMIO offset instead of an address
   virtual btBool  mmioGetFeatureOffset( btCSROffset        *pFeatureOffset,
                                         NamedValueSet const &rInputArgs,
                                         NamedValueSet       &rOutputArgs ) = 0;
   // overloaded version without rOutputArgs
   virtual btBool  mmioGetFeatureOffset( btCSROffset        *pFeatureOffset,
                                         NamedValueSet const &rInputArgs ) = 0;




}; // class IALIMMIO


//-----------------------------------------------------------------------------
// IALIUMsg interface.
//-----------------------------------------------------------------------------
/// @brief  Provide access to the UMsg region(s) exposed by the AFU to the Application.
/// @note   This service interface is obtained from an IBase via iidALI_UMSG_Service
/// @note   Once you have a pointer from umsgGetAddress(), you can write anything
///            to it, for up to 64-bytes, in any manner whatsoever. E.g. if you want
///            to atomically write a 16-byte value, you can do so using an SSE
///            intrinsic. If the processor supports AVX512, then one could atomically
///            write 64-bytes at once.
/// @note   If you write to a UMsg atomically, you should get one UMsg signal. If you
///            write to a UMsg atomically N times very quickly, the FPGA might get less
///            than N UMsg signals.
/// @note   If you write a single value that ends up being multiple writes from the point
///            of the view of the processor, then the FPGA will probably see multiple writes,
///            although it might not. An example of this would where where used memcpy()
///            to copy 64 bytes to the UMsg address. This might optimize to 4 pipelined
///            16-byte writes in a row. What the FPGA sees in such a situation is
///            non-deterministic.
/// @code
///         m_pALIUMsgService = dynamic_ptr<IALIUMsg>(iidALI_UMSG_Service, pServiceBase);
/// @endcode
///

#define UMSG_HINT_MASK_KEY "UMSG_HINT_MASK_KEY"
class IALIUMsg
{
public:
   virtual ~IALIUMsg() {}

   /// @brief     Obtain the number of UMsgs available to this AFU
   /// @note      Synchronous
   /// @return    The number of UMsgs available to the AFU.
   virtual btUnsignedInt umsgGetNumber( void ) = 0;

   /// @brief     Obtain the user virtual address of a particular UMsg.
   /// @note      Synchronous
   /// @param[in] Index of UMsg. Index starts from 0 and runs to umsgGetNumber()-1
   /// @return    The virtual address of the cache line which, if written, sends a UMsg
   virtual btVirtAddr    umsgGetAddress( const btUnsignedInt UMsgNumber ) = 0;

   /// @brief     Convenience function to write 64-bit entity to UMsg.
   /// @note      This is the only uMsg triggering method supported by ASE, so
   ///               use it for ASE compatibility.
   /// @note      This is intended to be fast, so there is no check. Passing a bad
   ///               address will probably result in a GPF.
   /// @param[in] pUMsg is pointer returned from umgGetAddress
   /// @param[in] Value is 64-bit value to write
   /// @return    The virtual address of the cache line which, if written, sends a UMsg
   virtual void    umsgTrigger64( const btVirtAddr pUMsg,
                                  const btUnsigned64bitInt Value ) = 0;

   /// @brief  Set attributes associated with the UMsg region and/or
   ///            individual UMsgs, depending on the arguments.
   /// @param[in] nvsArgs Defines the bitmask that will be set. Each bit
   ///	          in the mask corresponds to a UMsg. If set, then the UMsg has
   ///            hint as well as data.
   /// @note   Placeholder for now. Arguments TBD. E.g., for hint information.
   ///            Expectation is that hints will be defined for allocService
   ///            and used there, as well as here. This is only needed for changes.
   /// @note   #define UMSG_HINT_MASK_KEY       "UMsg-Hint-Mask-Key"
   ///         #define UMSG_HINT_MASK_DATATYPE  btUnsigned64bitInt
   ///         Value of bit mask is:
   ///            1 for hint, 0 for not.
   ///            Bit 0 = UMsg[0]
   ///            Bit 1 = UMsg[1]
   ///            etc.
   /// @note   Implication of synchronous is that the UMsg Device Feature Area
   ///            must be mapped in to user space.
   /// @return True of worked. At this point, no reason it would every fail.
   virtual bool umsgSetAttributes( NamedValueSet const &nvsArgs) = 0;

}; // class IALIUMsg


//-----------------------------------------------------------------------------
// IALIBuffer interface.
//-----------------------------------------------------------------------------
/// @brief  Buffer Allocation Service Interface of IALI
///
/// @note   This service interface is obtained from an IBase via iidALI_BUFF_Service
/// @code
///         m_pALIBufferService = dynamic_ptr<IALIBuffer>(iidALI_BUFF_Service, pServiceBase);
/// @endcode
///
class IALIBuffer
{
public:
   virtual ~IALIBuffer() {}

   /// @brief Allocate a Workspace.
   ///
   /// @param[in]  Length       Requested length, in bytes.
   /// @param[out] Bufferptr    Buffer Pointer.
   /// @param[in]  rInputArgs   Reference to optional input arguments if needed.
   /// @param[out] rOutputArgs  Reference to optional return arguments if needed.

   ///
   /// On success, the workspace parameters are notified via IALIBUFFER::bufferAllocated.
   /// On failure, an error notification is sent via IALIBUFFER::bufferAllocateFailed.
   virtual AAL::ali_errnum_e bufferAllocate( btWSSize             Length,
                                             btVirtAddr          *pBufferptr ) = 0;
   virtual AAL::ali_errnum_e bufferAllocate( btWSSize             Length,
                                             btVirtAddr          *pBufferptr,
                                             NamedValueSet const &rInputArgs ) = 0;
   virtual AAL::ali_errnum_e bufferAllocate( btWSSize             Length,
                                             btVirtAddr          *pBufferptr,
                                             NamedValueSet const &rInputArgs,
                                             NamedValueSet       &rOutputArgs ) = 0;

   /// @brief Free a previously-allocated Workspace.
   ///
   /// The provided workspace Address must have been acquired previously by IALIBUFFER::bufferAllocate.
   ///
   /// @param[in]  Address  User virtual address of the workspace.
   /// @param[in]  TranID   Returned in the notification event.
   ///
   /// On success, a notification is sent via IALIBUFFER::bufferFreed.
   /// On failure, an error notification is sent via IALIBUFFER::bufferFreeFailed.
   virtual AAL::ali_errnum_e bufferFree( btVirtAddr           Address) = 0;

   /// @brief Retrieve the location at which the AFU can access the passed in virtual address.
   ///
   /// The user virtual address that the application uses to access a buffer may or
   ///    may not be directly usable by the AFU. The general assumption is that it is not.
   ///
   /// NOTE: The mapping from virtual address to IOVA is not particularly fast. The user
   ///    of this interface is urged to allocate large buffers, obtain the IOVA once, and
   ///    then sub-allocate. Within a buffer, the virtual and IOVA addresses track each other.
   ///    E.g. if the virtual address is 0x6000 and the IOVA is 0x1000, then 5 bytes into 
   ///    the buffer will be at virtual address 0x6005 and at IOVA 0x1005.
   ///
   /// @param[in]  Address User virtual address to be converted to AFU-addressable location
   /// @return     A value that can be passed to the AFU such that when the AFU uses it,
   ///                the AFU will be accessing the byte at the address that was passed in.
   virtual btPhysAddr bufferGetIOVA( btVirtAddr Address) = 0;

}; // class IALIBuffer


//-----------------------------------------------------------------------------
// IALIPerf interface.
//-----------------------------------------------------------------------------
/// @brief  Obtain Global Performance Data (not AFU-specific) (synchronous)
///
/// @note   This service interface is obtained from an IBase via iidALI_PERF_Service
/// @code
///         m_pALIPerfService = dynamic_ptr<IALIPerf>(iidALI_PERF_Service, pServiceBase);
/// @endcode
///
class IALIPerf
{
public:
   virtual ~IALIPerf() {}

   /// @brief Request the Global Performance Data.
   ///
   /// The global performance counters relate to the traffic of all AFUs.
   /// They are 64-bit wrapping counters, and are read-only. Thus, one can
   ///    take a snapshot, perform an action, take another snapshot, subtract
   ///    one from the other (taking into account a possible wrap), and thereby
   ///    obtain the values associated with that operation (or operations if
   ///    across multiple AFUs).
   /// @note One needs to ensure that the operations being measured would not cause
   ///    a 64-bit wrap.
   ///
   /// @param[out]  ppResult  Returns the Performance Counter Values defined below.
   ///                           NULL if a problem.
   /// @param[in]   pOptArgs  Pointer to Optional Arguments if needed. Defaults to NULL.
   ///
   #define AALPERF_DATATYPE         btUnsigned64bitInt
   #define AALPERF_VERSION          "Version"           // Start with Version 0
   #define AALPERF_READ_HIT         "Read_Hit"
   #define AALPERF_WRITE_HIT        "Write_Hit"
   #define AALPERF_READ_MISS        "Read_Miss"
   #define AALPERF_WRITE_MISS       "Write_Miss"
   #define AALPERF_EVICTIONS        "Evictions"

   #define AALPERF_PCIE0_READ       "PCIe 0 Read"
   #define AALPERF_PCIE0_WRITE      "PCIe 0 Write"
   #define AALPERF_PCIE1_READ       "RPCIe 1 Read"
   #define AALPERF_PCIE1_WRITE      "PCIe 1 Write"
   #define AALPERF_UPI_READ         "UPI Read"
   #define AALPERF_UPI_WRITE        "UPI Write"
   ///
   /// @code
   /// INamedValueSet const *pResult = NULL;
   /// performanceCountersGet( &pResult );
   /// if ( NULL != pResult ) {
   ///    // retrieve results
   /// }
   /// @endcode

   virtual btBool performanceCountersGet ( INamedValueSet * const  pResult ) = 0;
   virtual btBool performanceCountersGet ( INamedValueSet * const  pResult,
                                           NamedValueSet    const &pOptArgs ) = 0;
}; // class IALIPerf

//-----------------------------------------------------------------------------
// IALIReset interface.
//-----------------------------------------------------------------------------
/// @brief  Reset the AFU Link Interface to this AFU (synchronous)
///
/// @note   This service interface is obtained from an IBase via iidALI_RSET_Service
/// @code
///         m_pALIResetService = dynamic_ptr<IALIReset>(iidALI_RSET_Service, pServiceBase);
/// @endcode
///
class IALIReset
{
public:
   virtual ~IALIReset() {}

   enum e_Reset {
      e_OK,                      ///< Everything is okay
      e_Internal,                ///< Internal failure
      e_Error_Quiesce_Timeout    ///< Could not disable completely, issued reset anyway
   };

   /// @brief Initiate a Reset.
   ///
   /// Only the Link to this AFU will be reset.
   /// By resetting the link, all outstanding transactions will be quiesced, the
   ///    processing of memory transactions will be disabled,
   ///    the AFU will be sent a Reset signal.
   /// There is a positive affirmation of Quiesce in the form of number of outstanding
   ///    transactions going to 0.
   /// Quiesce is destructive, e.g. outstanding transactions are lost. So state must
   ///    be Reset afterwards.
   ///
   /// TODO: Implementation needs to be via driver transaction so that driver is in
   ///          control, in case it needs to perform its own reset operations.
   ///
   /// @param[in]  rInputArgs      Pointer to Optional Arguments if ever needed. Defaults to NULL.
   /// @return     e_Reset         e_OK if succeeded, other values if a problem.
   ///             e_Error_Quiesce_Timeout indicates that the link did not quiesce within
   ///                             the provided timeout. (Currently no way to set timeout).
   ///
   virtual e_Reset afuQuiesceAndHalt( void ) = 0;
   virtual e_Reset afuQuiesceAndHalt( NamedValueSet const &rInputArgs ) = 0;

   /// @brief Re-enable the AFU after a Reset.
   ///
   /// Only the Link to this AFU will be enabled.
   /// It is an error to do anything other than strictly alternate afuQuiesceAndHalt
   ///    and afuReEnable.
   ///
   /// TODO: Implementation needs to be via driver transaction so that driver is in
   ///          control, in case it needs to perform its own reset operations.
   ///
   /// @param[in]  rInputArgs  Pointer to Optional Arguments if ever needed. Defaults to NULL.
   /// @return     e_Reset     e_OK if succeeded. No errors expected.
   ///
   virtual e_Reset afuEnable( void ) = 0;
   virtual e_Reset afuEnable( NamedValueSet const &rInputArgs) = 0;

   /// @brief Request a complete Reset. Convenience function combining other two.
   ///
   /// The Link to this AFU will be reset.
   /// By resetting the link, all outstanding transactions will quiesced, the
   ///    processing of memory transactions will be disabled,
   ///    the AFU will be sent a Reset signal,
   ///    and transactions will be re-enabled.
   ///
   /// TODO: Implementation needs to be via driver transaction so that driver is in
   ///          control, in case it needs to perform its own reset operations.
   ///
   /// @param[in]  rInputArgs              Pointer to Optional Arguments if ever needed. Defaults to NULL.
   /// @return     e_Reset                 e_OK if succeeded, other values if a problem.
   ///             e_Error_Quiesce_Timeout indicates that the link did not quiesce within
   ///                                     the provided timeout. (Currently no way to set timeout).
   ///
   virtual e_Reset afuReset( void ) = 0;
   virtual e_Reset afuReset( NamedValueSet const &rInputArgs ) = 0;
}; // class IALIReset


/// NOTE: this will be a service that is not typically exported by the ALI Service. Rather,
///       it will be allocated by requesting a PR_ID (an AFU_ID associated with a PR),
///       along with if necessary additional meta information such as bus:function:number of
///       the PCIe device.

//-----------------------------------------------------------------------------
// IALIReconfigure interface.
//-----------------------------------------------------------------------------
/// @brief  Provide Reconfiguration Services (asynchronous and controlled by driver)
///
/// @note   This service interface is obtained from an IBase via iidALI_CONF_Service
/// @code
///         m_pALIReconfigureService = dynamic_ptr<IALIReconfigure>(iidALI_CONF_Service, pServiceBase);
/// @endcode
///
class IALIReconfigure
{
public:
   virtual ~IALIReconfigure() {}

   #define AALCONF_FILENAMEKEY            "BitStreamFile"
   #define AALCONF_FILENAMETYPE           btString
   #define AALCONF_BUFPTRKEY              "BufPointer"
   #define AALCONF_BUFSIZE                "BufSize"

   /// @brief Deactivate an AFU in preparation for it being reconfigured.
   ///
   /// Basically, if there is an AFU currently instantiated and connected to an
   ///    application, then this will send an exception to the application indicating
   ///    that it should release the AFU. There can be a timeout option that specifies
   ///    that if the application does not Release within a particular time, then
   ///    the AFU will be yanked. Then, a CleanIt!(tm) AFU will be loaded to clear
   ///    out all the gates and clear the FPGA memory banks.
   ///
   /// TODO: Implementation needs to be via driver transaction
   ///
   /// @param[in]  rInputArgs Pointer to input Arguments.
   /// @return     void. Callback in IALIReconfigureClient.
   ///
   virtual void reconfDeactivate( TransactionID const &rTranID,
                                  NamedValueSet const &rInputArgs ) = 0;

   /// @brief Configure an AFU.
   ///
   /// Download the defined bitstream to the PR region. Initially, the bitstream
   ///    is a file name. Later, it might be a goal record, and that is why the
   ///    parameter is an NVS. It is also possible in the NVS to specify a PR number
   ///    if that is relevant, e.g. for the PF driver.
   ///
   /// TODO: Implementation needs to be via driver transaction
   ///
   /// @param[in]  rInputArgs Pointer to input Arguments.
   /// @return     void. Callback in IALIReconfigureClient.
   ///
   virtual void reconfConfigure( TransactionID const &rTranID,
                                 NamedValueSet const &rInputArgs ) = 0;

   /// @brief Activate an AFU after it has been reconfigured.
   ///
   /// Once the AFU has been reconfigured there needs to be a "probe" to load
   ///    the AFU configuration information, e.g. AFU_ID, so that the associated
   ///    service can be loaded and the whole shebang returned to the application.
   ///
   /// TODO: Implementation needs to be via driver transaction
   ///
   /// @param[in]  rInputArgs Pointer to input Arguments.
   /// @return     void. Callback in IALIReconfigureClient.
   ///
   virtual void reconfActivate( TransactionID const &rTranID,
                                NamedValueSet const &rInputArgs ) = 0;

}; // class IALIReconfigure


//-----------------------------------------------------------------------------
// IALIReconfigure_Client service client interface.
//-----------------------------------------------------------------------------
/// @brief  Reconfiguration Callback
///
/// @note   This interface is implemented by the client and set in the IBase
///         of the client object as an iidALI_CONF_Service_Client.
/// @code
///         SetInterface(iidALI_CONF_Service_Client, dynamic_cast<IALIReconfigure_Client *>(this));
/// @endcode
///
class IALIReconfigure_Client
{
public:
   virtual ~IALIReconfigure_Client() {}

   /// @brief Deactivate succeeded callback.
   ///
   /// TODO: Implementation needs to be via driver transaction
   ///
   /// @param[in]  rTranID Transaction from original reconfDeactivate call.
   /// @return     void.
   ///
   virtual void deactivateSucceeded( TransactionID const &rTranID ) = 0;

   /// @brief Notification callback for deactivate failed.
   ///
   /// Sent in response to a failed reconfDeactivate.
   ///
   /// @param[in]  Event  An IExceptionTransactionEvent describing the failure.
   ///
   virtual void deactivateFailed( IEvent const &rEvent ) = 0;

   /// @brief Configuration successful.
   ///
   /// TODO: Implementation needs to be via driver transaction
   ///
   /// @param[in]  rTranID Transaction from original reconfConfigure call.
   /// @return     void.
   ///
   virtual void configureSucceeded( TransactionID const &rTranID ) = 0;

   /// @brief Notification callback for deactivate failed.
   ///
   /// Sent in response to a failed reconfConfigure.
   ///
   /// @param[in]  Event  An IExceptionTransactionEvent describing the failure.
   ///
   virtual void configureFailed( IEvent const &rEvent ) = 0;

   /// @brief Activate succeeded callback.
   ///
   /// TODO: Implementation needs to be via driver transaction
   ///
   /// @param[in]  rTranID Transaction from original reconfActivate call.
   /// @return     void.
   ///
   virtual void activateSucceeded( TransactionID const &rTranID ) = 0;

   /// @brief Notification callback for  activate failure.
   ///
   /// Sent in response to a failed reconfActivated.
   ///
   /// @param[in]  Event  An IExceptionTransactionEvent describing the failure.
   ///
   virtual void activateFailed( IEvent const &rEvent ) = 0;

}; // class IALIReconfigure_Client


//-----------------------------------------------------------------------------
// IALISignalTap interface.
//-----------------------------------------------------------------------------
/// @brief  Access Signal Tap PCIe mmio space
///
/// @note This will be a service that is not typically exported by the ALI Service. Rather,
///       it will be allocated by requesting the STAP_ID (an AFU_ID associated with a Signal Tap)
///       along with if necessary additional meta information such as bus:function:number of
///       the PCIe device.
///
/// @note   This service interface is obtained from an IBase via iidALI_STAP_Service
/// @code
///         m_pALISignalTapService = dynamic_ptr<IALISignalTap>(iidALI_STAP_Service, pServiceBase);
/// @endcode
///
class IALISignalTap
{
public:
   virtual ~IALISignalTap() {}

   /// @brief  Obtain an mmio map for the Signal Tap region.
   ///
   /// @return User mode pointer to region if succeeds, NULL otherwise.
   ///
   virtual btVirtAddr stpGetAddress( void ) = 0;

}; // class IALISignalTap



/// @}

END_NAMESPACE(AAL)

#endif // __AALSDK_SERVICE_IALIAFU_H__

