// Copyright (c) 2012-2015, Intel Corporation
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
/// @file AALServiceModule.cpp
/// @brief AALServiceModule implementation.
/// @ingroup Services
/// @verbatim
/// Intel(R) QuickAssist Technology Accelerator Abstraction Layer
///
/// AUTHORS: Joseph Grecco, Intel Corporation
///          Tim Whisonant, Intel Corporation
///
/// HISTORY:
/// WHEN:          WHO:     WHAT:
/// 01/22/2013     TSW      Moving C++ inlined definitions to .cpp file@endverbatim
//****************************************************************************
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif // HAVE_CONFIG_H

#include "aalsdk/aas/AALServiceModule.h"


BEGIN_NAMESPACE(AAL)


IAALTransport::~IAALTransport() {}
IAALMarshalUnMarshallerUtil::~IAALMarshalUnMarshallerUtil() {}
IAALMarshaller::~IAALMarshaller() {}
IAALUnMarshaller::~IAALUnMarshaller() {}

AALServiceModule::AALServiceModule(ISvcsFact &fact) :
   m_SvcsFact(fact),
   m_Runtime(NULL),
   m_RuntimeClient(NULL)
{
   if ( SetSubClassInterface(iidServiceProvider, dynamic_cast<IServiceModule *>(this)) != EObjOK ) {
      m_bIsOK = false;
      return;
   }
}

IBase * AALServiceModule::Construct(IRuntime            *pAALRuntime,
                                    IBase               *Client,
                                    TransactionID const &tranID,
                                    NamedValueSet const &optArgs)
{
   // Add this one to the list of objects this container holds.
   //  It's up to the factory to enforce singletons.
   // Add the Service to the Module List before the Service can start

   AutoLock(this); // Lock to protect the AddtoList. The serviceAllocated can come at any time.

   IBase *pSvc = m_SvcsFact.CreateServiceObject(this, pAALRuntime);

   // Add the service to the list of services the module tracks.
   ASSERT(NULL != pSvc);
   if ( NULL != pSvc ) {
      AddToServiceList(pSvc);

      // Service will issue serviceAllocated now or fail.
      if ( !m_SvcsFact.InitializeService(Client, tranID, optArgs) ) {
         RemovefromServiceList(pSvc);
         pSvc = NULL;
      }
   }

   return pSvc;
}

void AALServiceModule::Destroy()
{
   // Protect the counter calculation
   //  so nothing can be released until
   //  we are waiting
   {
      AutoLock(this);

      list_type::size_type size = m_serviceList.size();
      if ( 0 == size ) {
         return;
      }

      // Initialize the semaphore as a count up by initializing
      //  count to a negative number.
      //  The waiter will block until the semaphore
      //  counts up to zero.
      btBool res =
      m_srvcCount.Create( - static_cast<btInt>(size) );

      ASSERT(res);

      // Loop through all services and shut them down
      SendReleaseToAll();
   }

   // Wait for all to complete. Unlock before waiting.
   m_srvcCount.Wait();
}

void AALServiceModule::setRuntime(IRuntime *pRuntime)
{
   AutoLock(this);
   m_Runtime = pRuntime;
}

IRuntime * AALServiceModule::getRuntime() const
{
   AutoLock(this);
   return m_Runtime;
}

void AALServiceModule::ServiceReleased(IBase *pService)
{
   RemovefromServiceList(pService);
}

void AALServiceModule::setRuntimeClient(IRuntimeClient *pRTC)
{
   AutoLock(this);
   m_RuntimeClient = pRTC;
}

IRuntimeClient * AALServiceModule::getRuntimeClient() const
{
   AutoLock(this);
   return m_RuntimeClient;
}

btBool AALServiceModule::AddToServiceList(IBase *pService)
{
   AutoLock(this);

   if ( ServiceInstanceRegistered(pService) ) {
      return false;
   }

   m_serviceList[pService] = pService;

   return true;
}

btBool AALServiceModule::RemovefromServiceList(IBase *pService)
{
   AutoLock(this);

   if ( !ServiceInstanceRegistered(pService) ) {
      return false;
   }

   m_serviceList.erase(pService);

   // Post to the count up semaphore
   //  in case the service is shutting down
   m_srvcCount.Post(1);

   return true;
}

btBool AALServiceModule::ServiceInstanceRegistered(IBase *pService)
{
   AutoLock(this);
   return m_serviceList.end() != m_serviceList.find(pService);
}

void AALServiceModule::SendReleaseToAll()
{
   AutoLock(this);   // Lock until done issuing releases

   const_iterator iter = m_serviceList.begin();

   while ( m_serviceList.end() != iter ) {

      // Get the IAALService from the IBase
      IAALService *pService = dynamic_ptr<IAALService>(iidService, (*iter).second);

      if ( NULL != pService ) {
         // Release with no event. Service will call Released() as a notification
         pService->Release(AAL_INFINITE_WAIT);
      }

      iter++;
   }
}

END_NAMESPACE(AAL)
