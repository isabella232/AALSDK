// Copyright(c) 2007-2016, Intel Corporation
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
/// @file AASEventDeliveryService.h
/// @brief Public Interface to Event Delivery Service
/// @ingroup EDS
/// @verbatim
/// Accelerator Abstraction Layer
///
/// AUTHORS: Joseph Grecco, Intel Corporation
///          Henry Mitchel, Intel Corporation
///
/// HISTORY:
/// WHEN:          WHO:     WHAT:
/// 04/23/2007     JG       Initial version started
/// 05/08/2008     HM       Comments & License
/// 01/04/2009     HM       Updated Copyright@endverbatim
//****************************************************************************
#ifndef __AALSDK_EDS_AASEVENTDELIVERYSERVICE_H__
#define __AALSDK_EDS_AASEVENTDELIVERYSERVICE_H__
#include <aalsdk/AALTypes.h>
#include <aalsdk/CAALEvent.h>
#include <aalsdk/osal/IDispatchable.h>

BEGIN_NAMESPACE(AAL)

/// @addtogroup MDS
/// @{

/// Status codes returned by IEventDeliveryService.
typedef enum EDS_Status
{
   EDS_statusOK = 0,          ///< Operation was successful.
   EDS_statusNotAvailable,    ///< Resource not available.
   EDS_statusUnsupportedModel ///< Mode of interaction is not supported.
} EDS_Status;

/// Identifies the Event Dispatcher class.
typedef enum EDSDispatchClass
{
   EDS_dispatcherNormal,      ///< Normal priority dispatcher.
   EDS_dispatcherHighPriority ///< High priority dispatcher.
} EDSDispatchClass;


/// Interface for the Event Dispatcher.
class AALRUNTIME_API IEventDispatcher
{
public:
   // IEventDispatcher Destructor.
   virtual ~IEventDispatcher() {}

   /// Internal state check.
   /// @retval True if the internal state of the Event Dispatcher is OK.
   /// @retval Talse if the internal state of the Event Dispatcher is not OK.
   virtual btBool       IsOK()                      = 0;
   /// Queue an event with event handler to the Dispatcher.
   /// @param[in] pEventHandler A pointer to the destination of the event.
   /// @param[in] pEvent A pointer to the Event.
   /// @retval True if the event was queued.
   /// @retval Talse if the event was not queued.
   virtual btBool QueueEvent(btEventHandler pEventHandler,
                             CAALEvent     *pEvent) = 0;
   /// Queue a functor Event (deprecated)
   /// @param[in] target A pointer to the destination of the event.
   /// @param[in] pEvent A pointer to the Event.
   /// @retval True if the event was queued.
   /// @retval Talse if the event was not queued.
   virtual btBool QueueEvent(btObjectType   target,
                             CAALEvent     *pEvent) = 0;
   /// Queue a generic Dispatchable functor or event.
   /// @param[in] parm A pointer to the destination of the functor or event.
   /// @param[in] pDisp A pointer to the functor or event.
   /// @retval True if the item was queued.
   /// @retval Talse if the item was not queued.
   virtual btBool QueueEvent(btObjectType   parm,
                             IDispatchable *pDisp)  = 0;
   /// Release the resources held by the Event Dispatcher.
   /// @returns void
   virtual void      Release()                      = 0;
};


/// Interface for the Event Delivery System Service.
class AALRUNTIME_API IEventDeliveryService
{
public:
   // IEventDeliveryService Destructor.
   virtual ~IEventDeliveryService() {}

   /// Starts the Event Delivery Service.
   /// @returns void
   virtual void StartEventDelivery()                 = 0;
   /// Stops the Event Delivery Service.
   /// @returns void
   virtual void  StopEventDelivery()                 = 0;
   /// Schedules an Event.
   /// @retval True if the Event was scheduled.
   /// @retval False if the Event was scheduled.
   virtual btBool     scheduleWork(IDispatchable * ) = 0;
};

/// @}

/// Interface for the Event Delivery System Service.
class AALRUNTIME_API IMessageDeliveryService
{
public:
   // IMessageDeliveryService Destructor.
   virtual ~IMessageDeliveryService() {}

   /// Starts the Message Delivery Service.
   /// @returns void
   virtual void StartMessageDelivery()                 = 0;
   /// Stops the Message Delivery Service.
   /// @returns void
   virtual void  StopMessageDelivery()                 = 0;
   /// Schedules a message.
   /// @retval True if the message was scheduled.
   /// @retval False if the message was scheduled.
   virtual btBool    scheduleMessage(IDispatchable * ) = 0;
};

END_NAMESPACE(AAL)

#endif // __AALSDK_EDS_AASEVENTDELIVERYSERVICE_H__


