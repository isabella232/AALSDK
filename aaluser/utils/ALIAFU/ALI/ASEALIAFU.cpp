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
/// @file ASEALIAFU.cpp
/// @brief Implementation of ASE ALI AFU Service.
/// @ingroup ASEALIAFU
/// @verbatim
/// Accelerator Abstraction Layer Sample Application
///
///    This application is for example purposes only.
///    It is not intended to represent a model for developing commercially-deployable applications.
///    It is designed to show working examples of the AAL programming model and APIs.
///
/// AUTHORS: Rahul R Sharma (Intel Corporation)
///
/// HISTORY:
/// WHEN:          WHO:     WHAT:
/// 07/20/2015     HM       Initial version.
/// 12/16/2015     RRS      Integration with ASE App-backend.@endverbatim
/// 09/23/2016	   RRS		Porting methods to libALI
//****************************************************************************


#ifdef HAVE_CONFIG_H
# include <config.h>
#endif // HAVE_CONFIG_H


#include <aalsdk/utils/ResMgrUtilities.h>
#include <aalsdk/AALLoggerExtern.h>
#include <aalsdk/service/IALIAFU.h>
#include <aalsdk/aas/AALService.h>
#include <aalsdk/ase/ase_common.h>
#include <aalsdk/uaia/IAFUProxy.h>
#include "ASEALIAFU.h"


BEGIN_NAMESPACE(AAL)

/// @addtogroup ALI
/// @{

//
// ctor, HWALIA Base constructor.
//
CASEALIAFU::CASEALIAFU( IBase *pSvcClient,
                        IServiceBase *pServiceBase,
                        TransactionID transID):
                        CALIBase(pSvcClient,pServiceBase,transID),
                        m_mapWkSpc(),
                        m_MMIORmap(NULL),
                        m_MMIORsize(0),
                        m_Last3c4(0xffffffff),
                        m_Last3cc(0xffffffff)
{

}

//
// ASERelease. Release ASE session.
//
btBool CASEALIAFU::ASERelease()
{
   session_deinit();
   return true;
}

//
// ASEInit. InitializesASE session and Sets interfaces.
//
btBool CASEALIAFU::ASEInit()
{
   session_init();

  
   // update member variables that cache parameters
   m_MMIORmap = mmioGetAddress();
   m_MMIORsize = (MMIO_LENGTH - MMIO_AFU_OFFSET);

   // If we have a valid MMIO region, expose IALIMMIO interface
   if (m_MMIORmap != NULL && m_MMIORsize > 0) {
      if ( EObjOK !=  SetInterface(iidALI_MMIO_Service, dynamic_cast<IALIMMIO *>(this)) ) {
         AAL_ERR( LM_ALI, "Set ASE MMIO Interface failed"<< std::endl);
         m_pServiceBase->initFailed(new CExceptionTransactionEvent( NULL,
                                                                    m_tidSaved,
                                                                    errCreationFailure,
                                                                    reasUnknown,
                                                                    "Error: Could not register interface."));
         return true;
      }
   }

   // Umsg Base settings
   m_uMSGmap  = (btVirtAddr)umsg_umas_vbase;
   m_uMSGsize = UMAS_LENGTH;

   // Populate internal data structures for feature discovery
   if (! _discoverFeatures() ) {
      // FIXME: use correct error classes
      AAL_ERR( LM_ALI, "Discover Features failed"<< std::endl);
      m_pServiceBase->initFailed(new CExceptionTransactionEvent( NULL,
                                                                 m_tidSaved,
                                                                 errBadParameter,
                                                                 reasMissingInterface,
                                                                 "Failed to discover features."));
      return true;
   }

   // Print warnings for malformed device feature lists
   _validateDFL();

   return true;
}

btBool CASEALIAFU::_discoverFeatures() {

   btBool retVal;

   // walk DFH list and populate internal data structure
   // also do some sanity checking
   AAL_DEBUG(LM_AFU, "Populating feature list from DFH list..." << std::endl);
   FeatureDefinition feat;
   btUnsigned32bitInt offset = 0;         // offset that we are currently at

   // look at AFU CSR (mandatory) to get first feature header offset
   retVal = mmioRead64(0, (btUnsigned64bitInt *)&feat.dfh);
   ASSERT( retVal );
   _printDFH(feat.dfh);
   feat.offset = offset;

   // read AFUID
   retVal = mmioRead64(offset +  8, &feat.guid[0]);
   ASSERT( retVal );
   retVal = mmioRead64(offset + 16, &feat.guid[1]);
   ASSERT( retVal );

   // Add AFU feature to list
   m_featureList.push_back(feat);
   offset = feat.dfh.next_DFH_offset;

   // look at chained DFHs until end of list bit is set or next offset is 0
   while (feat.dfh.eol == 0 && feat.dfh.next_DFH_offset != 0) {

      // populate fields
      feat.offset = offset;
      // read feature header
      retVal = mmioRead64(offset, (btUnsigned64bitInt *)&feat.dfh);
      ASSERT( retVal );
      _printDFH(feat.dfh);
      // read guid, if present
      if (feat.dfh.Type == ALI_DFH_TYPE_BBB) {
         retVal = mmioRead64(offset +  8, &feat.guid[0]);
         ASSERT( retVal );
         retVal = mmioRead64(offset + 16, &feat.guid[1]);
         ASSERT( retVal );
      } else {
         feat.guid[0] = feat.guid[1] = 0;
      }

      // create new list entry
      m_featureList.push_back(feat);          // copy dfh to list

      // check for next header
      offset += feat.dfh.next_DFH_offset;
   }

   return true;
}

// This currently only prints warnings.
btBool CASEALIAFU::_validateDFL()
{
   // check for ambiguous featureID/GUID
   AAL_DEBUG(LM_AFU, "Checking detected features for ambiguous IDs..." <<
         std::endl);
   for (FeatureList::iterator a = m_featureList.begin(); a !=
         m_featureList.end(); ++a) {
      for (FeatureList::iterator b = a+1; b != m_featureList.end(); ++b ) {

         AAL_DEBUG(LM_AFU, "Comparing features at 0x" << std::hex << a->offset <<
                  " and 0x" << b->offset << std::endl);

         if (a->dfh.Feature_ID == b->dfh.Feature_ID) {
            AAL_INFO(LM_AFU, "Features at 0x" << std::hex << a->offset <<
                  " and 0x" << b->offset << " share feature ID " <<
                  std::dec << a->dfh.Feature_ID << std::endl);

            if (a->dfh.Type != b->dfh.Type) {
               AAL_INFO(LM_AFU, "   Features can be disambiguated by type - OK."
                     << std::endl);
            } else {
               if (a->dfh.Type == ALI_DFH_TYPE_BBB) {
                  if (a->guid[0] != b->guid[0] || a->guid[1] != b->guid[1]) {
               AAL_INFO(LM_AFU, "   Features can be disambiguated by BBB GUID - OK."
                     << std::endl);
                  } else {
                     AAL_WARNING(LM_AFU,
                           "   Features have same BBB GUID! This is not recommended,"
                           << std::endl);
                     AAL_WARNING(LM_AFU,
                           "   as it complicates disambiguation." << std::endl);
                     AAL_WARNING(LM_AFU,
                           "   Please consider giving out separate feature IDs for"
                           << std::endl);
                     AAL_WARNING(LM_AFU, "   each." << std::endl);
                  }
               } else {
                  AAL_WARNING(LM_AFU,
                        "   Features have same feature ID and no other standard"
                        << std::endl);
                  AAL_WARNING(LM_AFU,
                        "   mechanism for disambiguation! This is not recommended."
                        << std::endl);
                  AAL_WARNING(LM_AFU,
                        "   Please consider giving out separate feature IDs for"
                        << std::endl);
                  AAL_WARNING(LM_AFU, "   each." << std::endl);
               }
            }
         }
      }
   }
   return true;
}



void CASEALIAFU::_printDFH( const struct CCIP_DFH &dfh )
{
   AAL_DEBUG(LM_AFU, "Type: " << std::hex << std::setw(2)
         << std::setfill('0') << dfh.Type <<
         ", Next DFH offset: " << dfh.next_DFH_offset <<
         ", Feature Rev: " << dfh.Feature_rev <<
         ", Feature ID: " << dfh.Feature_ID <<
         ", eol: " << std::dec << dfh.eol << std::endl);
}


// btBool ASEALIAFU::Release(TransactionID const &TranID, btTime timeout)
// {
//   session_deinit();
//   return ServiceBase::Release(TranID, timeout);
// }


// ---------------------------------------------------------
// MMIO actions
// ---------------------------------------------------------
//
// mmioGetAddress. Return address of MMIO space.
//
btVirtAddr CASEALIAFU::mmioGetAddress( void )
{
  m_MMIORmap =  (btVirtAddr)mmio_afu_vbase;
  return m_MMIORmap;
}

//
// mmioGetLength. Return length of MMIO space.
//
btCSROffset CASEALIAFU::mmioGetLength( void )
{
  m_MMIORsize = (MMIO_LENGTH - MMIO_AFU_OFFSET);
  return m_MMIORsize;
}

//
// mmioRead32. Read 32bit CSR. Offset given in bytes.
//
btBool CASEALIAFU::mmioRead32(const btCSROffset Offset, btUnsigned32bitInt * const pValue)
{
   if ( (NULL == m_MMIORmap) || (Offset > m_MMIORsize) ) {
      return false;
   }

  mmio_read32(Offset, pValue);
  return true;
}

//
// mmioWrite32. Write 32bit CSR. Offset given in bytes.
//
btBool CASEALIAFU::mmioWrite32(const btCSROffset Offset, const btUnsigned32bitInt Value)
{
   if ( (NULL == m_MMIORmap) || (Offset > m_MMIORsize) ) {
      return false;
   }

  mmio_write32(Offset, Value);
  return true;
}

//
// mmioRead64. Read 64bit CSR. Offset given in bytes.
//
btBool CASEALIAFU::mmioRead64(const btCSROffset Offset, btUnsigned64bitInt * const pValue)
{
   if ( (NULL == m_MMIORmap) || (Offset > m_MMIORsize) ) {
      return false;
   }

  mmio_read64(Offset, (uint64_t*)pValue);
  return true;
}

//
// mmioWrite64. Write 64bit CSR. Offset given in bytes.
//
btBool CASEALIAFU::mmioWrite64(const btCSROffset Offset, const btUnsigned64bitInt Value)
{
   if ( (NULL == m_MMIORmap) || (Offset > m_MMIORsize) ) {
      return false;
   }

  mmio_write64(Offset, Value);
  return true;
}

//
// mmioGetFeature. Get pointer to feature's DFH, if found.
//
btBool  CASEALIAFU::mmioGetFeatureAddress( btVirtAddr          *pFeatureAddress,
                                          NamedValueSet const &rInputArgs,
                                          NamedValueSet       &rOutputArgs )
{
   btBool             filterByID;
   btUnsigned64bitInt filterID;
   btBool             filterByType;
   btUnsigned64bitInt filterType;
   btBool             filterByGUID;
   btcString          filterGUID;

   // extract filters
   filterByID = false;
   if (rInputArgs.Has(ALI_GETFEATURE_ID_KEY)) {
      if (ENamedValuesOK != rInputArgs.Get(ALI_GETFEATURE_ID_KEY, &filterID)) {
         AAL_ERR(LM_ALI, "rInputArgs.Get(ALI_GETFEATURE_ID) failed -- " <<
                         "wrong datatype?" << std::endl);
         return false;
      } else {
         filterByID = true;
      }
   }

   filterByType = false;
   if (rInputArgs.Has(ALI_GETFEATURE_TYPE_KEY)) {
      if (ENamedValuesOK != rInputArgs.Get(ALI_GETFEATURE_TYPE_KEY, &filterType)) {
         AAL_ERR(LM_ALI, "rInputArgs.Get(ALI_GETFEATURE_TYPE) failed -- " <<
                         "wrong datatype?" << std::endl);
         return false;
      } else {
         filterByType = true;
      }
   }

   filterByGUID = false;
   if (rInputArgs.Has(ALI_GETFEATURE_GUID_KEY)) {
      if (ENamedValuesOK != rInputArgs.Get(ALI_GETFEATURE_GUID_KEY, &filterGUID)) {
         AAL_ERR(LM_ALI, "rInputArgs.Get(ALI_GETFEATURE_GUID) failed -- " <<
                         "wrong datatype?" << std::endl);
         return false;
      } else {
         filterByGUID = true;
      }
   }

   //
   // Spec and sanity checks
   //

   // Can't search for GUID in private features
   if ((filterByGUID && filterByType && (filterType == ALI_DFH_TYPE_PRIVATE))) {
      AAL_ERR(LM_AFU, "Can't search for GUIDs in private features." << std::endl);
      return false;
   }


   // walk features
   AAL_DEBUG(LM_AFU, "Walking feature list..." << std::endl);
   for ( FeatureList::iterator iter = m_featureList.begin();
         iter != m_featureList.end(); ++iter ) {
      // NOTE: if we had C++1 or later, we coud do:
      // for ( FeatureDefinition feat : m_featureList )
      // and work with "feat" directly.
      FeatureDefinition &feat = *iter;

      // return first matching feature
      if (
            ( !filterByID   || (feat.dfh.Feature_ID == filterID  )              ) &&
            ( !filterByType || (feat.dfh.Type       == filterType)              ) &&
            ( !filterByGUID || ( (feat.dfh.Type != ALI_DFH_TYPE_PRIVATE) &&
                                 ( 0 == strncmp(filterGUID,
                                          GUIDStringFromStruct(
                                            GUIDStructFrom2xU64(
                                              feat.guid[1],
                                              feat.guid[0]
                                            )
                                          ).c_str(),
                                          16
                                        )
                                 )
                               )
            )
         ) {

         AAL_INFO(LM_AFU, "Found matching feature." << std::endl);
         *pFeatureAddress = (btVirtAddr)(m_MMIORmap + feat.offset);   // return pointer to DFH
         // populate output args
         rOutputArgs.Add(ALI_GETFEATURE_ID_KEY, feat.dfh.Feature_ID);
         rOutputArgs.Add(ALI_GETFEATURE_TYPE_KEY, feat.dfh.Type);
         if (feat.dfh.Type != ALI_DFH_TYPE_PRIVATE) {
            rOutputArgs.Add(ALI_GETFEATURE_GUID_KEY, GUIDStringFromStruct(
                                                       GUIDStructFrom2xU64(
                                                         feat.guid[1],
                                                         feat.guid[0]
                                                       )
                                                     ).c_str()
                           );
         }
         return true;
      }
   }

   // if not found, do not modify ppFeature, return false.
   AAL_INFO(LM_AFU, "No matching feature found." << std::endl);
   return false;
}

btBool CASEALIAFU::mmioGetFeatureAddress( btVirtAddr          *pFeatureAddress,
                                         NamedValueSet const &rInputArgs )
{
   NamedValueSet temp;
   return mmioGetFeatureAddress(pFeatureAddress, rInputArgs, temp);
}

btBool CASEALIAFU::mmioGetFeatureOffset( btCSROffset         *pFeatureOffset,
                                        NamedValueSet const &rInputArgs,
                                        NamedValueSet       &rOutputArgs )
{
   btVirtAddr pFeatAddr;
   if (true == mmioGetFeatureAddress(&pFeatAddr, rInputArgs, rOutputArgs)) {
      *pFeatureOffset = pFeatAddr - mmioGetAddress();
      return true;
   }
   return false;
}

// overloaded version without rOutputArgs
btBool  CASEALIAFU::mmioGetFeatureOffset( btCSROffset         *pFeatureOffset,
                                         NamedValueSet const &rInputArgs )
{
   NamedValueSet temp;
   return mmioGetFeatureOffset(pFeatureOffset, rInputArgs, temp);
}



// -----------------------------------------------------
// Buffer allocation API
// -----------------------------------------------------
AAL::ali_errnum_e CASEALIAFU::bufferAllocate( btWSSize             Length,
                                             btVirtAddr          *pBufferptr,
                                             NamedValueSet const &rInputArgs,
                                             NamedValueSet       &rOutputArgs )
{
  struct buffer_t *buf;
  int ret;

  ALI_MMAP_TARGET_VADDR_DATATYPE pTargetVirtAddr;       // requested virtual address for the mapping

  // extract target VA from optArgs
  if ( ENamedValuesOK != rInputArgs.Get(ALI_MMAP_TARGET_VADDR_KEY, &pTargetVirtAddr) ) {
     pTargetVirtAddr = NULL;    // no mapping requested
  }

  buf = (struct buffer_t *) ase_malloc (sizeof(struct buffer_t));
  // memset(buf, 0, sizeof(buffer_t));

  buf->memsize = (uint32_t)Length;

  // Allocate buffer (ASE call)
  allocate_buffer(buf, (uint64_t*)pTargetVirtAddr);

  if ( ( ASE_BUFFER_VALID != buf->valid )   ||
       ( MAP_FAILED == (void *)buf->vbase ) ||
       ( 0 == buf->fake_paddr ) )
    {
      std::cout << "Error Allocating ASE buffer ... EXITING\n";
      return ali_errnumNoMem;
    }

  *pBufferptr = (btVirtAddr)buf->vbase;

  // Add info to Workspace map
  struct aalui_WSMParms wsParms;
  wsParms.wsid = buf->index;
  wsParms.ptr = (btVirtAddr)buf->vbase;
  wsParms.physptr = buf->fake_paddr;
  wsParms.size = buf->memsize;

  m_mapWkSpc[(btVirtAddr)buf->vbase] = wsParms;

  return ali_errnumOK;
}


AAL::ali_errnum_e CASEALIAFU::bufferFree( btVirtAddr Address)
{
  // Find in map and remove
   mapWkSpc_t::iterator i = m_mapWkSpc.find(Address);
   if (i == m_mapWkSpc.end()) {  // not found
      AAL_ERR(LM_ALI, "Tried to free non-existent Buffer");
      return ali_errnumBadParameter;
   }

  struct aalui_WSMParms wsParms;
  wsParms = i->second;

  // Call ase_common:deallocate_buffer_by_index
  deallocate_buffer_by_index((int)wsParms.wsid);

  return ali_errnumOK;
}


// Exactly the same as HWALIAFU::bufferGetIOVA
btPhysAddr CASEALIAFU::bufferGetIOVA( btVirtAddr Address)
{
   mapWkSpc_t::iterator i = m_mapWkSpc.find(Address);
   if (i != m_mapWkSpc.end()) {
      return i->second.physptr;
   }

   for (mapWkSpc_t::iterator i = m_mapWkSpc.begin(); i != m_mapWkSpc.end(); i++)
     {
       if (Address < i->second.ptr + i->second.size) {
         return i->second.physptr + (Address - i->second.ptr);
       }
     }

   // not found
   return 0;
}


// ---------------------------------------------------------------------------
// IALIUMsg interface implementation
// ---------------------------------------------------------------------------

//
// umsgGetNumber. Return number of UMSGs.
//
btUnsignedInt CASEALIAFU::umsgGetNumber( void )
{
  return NUM_UMSG_PER_AFU;
}

//
// umsgGetAddress. Get address of specific UMSG.
//
btVirtAddr CASEALIAFU::umsgGetAddress( const btUnsignedInt UMsgNumber )
{
   // Umsgs are separated by 1 Page + 1 CL
   // Malicious call could overflow and cause wrap to invalid address.
   // TODO: Check if there is any problem with using a different address
   //       in the UMAS range

   // Input check
   if ((UMsgNumber < 0) || (UMsgNumber > NUM_UMSG_PER_AFU))
   {
      return NULL;
   }
   else
   {
     // m_uMSGmap is btVirtAddr is char* so math is in bytes
      btUnsigned32bitInt offset = UMsgNumber * (4096 + 64) ;
      return m_uMSGmap + offset;
   }
}


void CASEALIAFU::umsgTrigger64( const btVirtAddr pUMsg,
                const btUnsigned64bitInt Value )
{

}  // umsgTrigger64


//
// umsgSetAttributes. Set UMSG attributes.
//
bool CASEALIAFU::umsgSetAttributes( NamedValueSet const &nvsArgs)
{
  btUnsigned64bitInt hint_flag;

   if( true != nvsArgs.Has(UMSG_HINT_MASK_KEY)){
      AAL_ERR( LM_All,"Missing Parameter or Key");
      return false;
   }
   eBasicTypes nvsType;
   if(ENamedValuesOK !=  nvsArgs.Type(UMSG_HINT_MASK_KEY, &nvsType)){
      AAL_ERR( LM_All,"Unable to get key value type.");
      return false;
   }

   if(btUnsigned64bitInt_t !=  nvsType){
      AAL_ERR( LM_All,"Bad value type.");
      return false;
   }
   
   if (nvsArgs.Get(UMSG_HINT_MASK_KEY, &hint_flag) != ENamedValuesOK)
     {
       AAL_ERR( LM_All,"Get Failed");
       return false;
     }
   
   // Send UMSG setup (call ASE)
   umsg_set_attribute(hint_flag);

   return true;
}


IALIReset::e_Reset CASEALIAFU::afuQuiesceAndHalt( NamedValueSet const &rInputArgs )
{
   // NOT IMPLEMENTED

   return e_OK;
}

IALIReset::e_Reset CASEALIAFU::afuEnable( NamedValueSet const &rInputArgs)
{
   // DOES NOTHING

   return e_OK;

}

IALIReset::e_Reset CASEALIAFU::afuReset( NamedValueSet const &rInputArgs )
{
   send_swreset();

   return e_OK;
}

/// @} group ALI

END_NAMESPACE(AAL)
