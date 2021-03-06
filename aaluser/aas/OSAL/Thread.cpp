// Copyright(c) 2003-2016, Intel Corporation
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
/// @file Thread.cpp
/// @brief Implementation of the OSLThread class.
/// @ingroup OSAL
/// @verbatim
/// Accelerator Abstraction Layer
///
/// AUTHORS: Charlie Lasswell, Intel Corporation
///          Joseph Grecco,    Intel Corporation
///          Henry Mitchel,    Intel Corporation
///          Tim Whisonant,    Intel Corporation
///
/// HISTORY:
/// WHEN:          WHO:     WHAT:
/// 12/16/2007     JG       Changed include path to expose aas/
/// 04/04/2008     JG       Added IsOK method to OLThread object
///                         Fixed bug in thread destructor that caused a
///                         resource leak if in Linux you do not do a Join()
/// 05/08/2008     HM       Cleaned up windows includes
///                         Fixed bug in OSLThread constructor
/// 05/08/2008     HM       Comments & License
/// 01/04/2009     HM       Updated Copyright
/// 07/14/2009     HM/JG    Explicit initialization of all fields in OSLThread
///                         Added m_Joined. If the thread has been Joined, then
///                            it is already clean and should not be cancelled &
///                            detached in the dtor.
/// 06/14/2012     HM       Added Cancel()@endverbatim
//****************************************************************************
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif // HAVE_CONFIG_H

#include "aalsdk/osal/Thread.h"
#include <aalsdk/AALTypes.h>

#if   defined( __AAL_WINDOWS__ )
# include <stdlib.h> // errno_t rand_s(unsigned int *RandomValue);
# include <process.h>
#elif defined( __AAL_LINUX__ )
# include <cstdlib>  // int rand_r(unsigned int *seed);
# include <signal.h>
# include <unistd.h>
#endif // OS

BEGIN_NAMESPACE(AAL)

const btInt OSLThread::sm_PriorityTranslationTable[(btInt)THREADPRIORITY_COUNT] =
{
#if   defined( __AAL_WINDOWS__ )
   THREAD_PRIORITY_TIME_CRITICAL,
   THREAD_PRIORITY_HIGHEST,
   THREAD_PRIORITY_ABOVE_NORMAL,
   THREAD_PRIORITY_NORMAL,
   THREAD_PRIORITY_BELOW_NORMAL
#elif defined( __AAL_LINUX__ )
   5,
   4,
   3,
   2,
   1
#endif // OS
};

const btInt OSLThread::sm_DefaultPriority =
               OSLThread::sm_PriorityTranslationTable[THREADPRIORITY_NORMAL];


//=============================================================================
// Name: OSLThread
// Description: Thread abstraction
// Interface: public
// Inputs: pProc - Pointer to procedure to execute
//         nPriority - OSAL thread priority
//         pContext - Context
//         ThisThread - btBool indicating if proc should run in this thread
// Outputs: none.
// Comments:
//=============================================================================
OSLThread::OSLThread(ThreadProc                     pProc,
                     OSLThread::ThreadPriority      nPriority,
                     void                          *pContext,
                     btBool                         ThisThread) :
#if   defined( __AAL_WINDOWS__ )
   m_hThread(NULL),
#elif defined( __AAL_LINUX__ )
   m_Thread(),
#endif // OS
   m_tid(),
   m_pProc(pProc),
   m_nPriority(THREADPRIORITY_INVALID),
   m_pContext(pContext),
   m_State(0)
{
   ASSERT(NULL != pProc);

   if ( ( nPriority >= 0 ) &&
        ( (unsigned)nPriority < (sizeof(OSLThread::sm_PriorityTranslationTable) / sizeof(OSLThread::sm_PriorityTranslationTable[0])) ) ) {
      m_nPriority = OSLThread::sm_PriorityTranslationTable[(btInt)nPriority];
   } else {
      m_nPriority = OSLThread::sm_DefaultPriority;
   }

   if ( !m_Semaphore.Create(0, INT_MAX) ) { // (Without setting THR_ST_OK.)
      return;
   }

   if ( NULL == pProc ) { // (Without setting THR_ST_OK.)
      return;
   }

   if ( ThisThread ) {
      // Run the thread function locally in this thread.

      flag_setf(m_State, THR_ST_OK|THR_ST_LOCAL);
      OSLThread::StartThread(this);

   } else {

      flag_setf(m_State, THR_ST_OK);

#if   defined( __AAL_WINDOWS__ )
      // Create a new thread to run the thread function.

      m_hThread = CreateThread(NULL,
                               0,
                               OSLThread::StartThread,
                               this,
                               0,
                               NULL);

      if ( NULL == m_hThread ) {
         flag_clrf(m_State, THR_ST_OK);
      }

#elif defined( __AAL_LINUX__ )

      int res = pthread_create(&m_Thread, NULL, OSLThread::StartThread, this);
      if ( 0 != res ) {
         flag_clrf(m_State, THR_ST_OK);
      }

#endif // OS

   }
}

//=============================================================================
// Name: StartThread
// Description: Thread entry point for newly-spawned and local thread cases.
// Interface: private
// Inputs:
// Outputs: none.
// Comments:
//=============================================================================

#ifdef DBG_OSLTHREAD
# include "dbg_oslthread_0.cpp"
#endif // DBG_OSLTHREAD

#if   defined( __AAL_WINDOWS__ )
DWORD WINAPI OSLThread::StartThread(LPVOID p)
#elif defined( __AAL_LINUX__ )
void * OSLThread::StartThread(void *p)
#endif // OS
{
   OSLThread *pThread = reinterpret_cast<OSLThread *>(p);

   if ( NULL == pThread ) {
#if   defined( __AAL_WINDOWS__ )
      return (DWORD)-1;
#elif defined( __AAL_LINUX__ )
      return (void *)-1;
#endif // OS
   }

   if ( OSLThread::sm_DefaultPriority != pThread->m_nPriority ) {

#if   defined( __AAL_WINDOWS__ )

      SetThreadPriority(GetCurrentThread(), pThread->m_nPriority);

#elif defined( __AAL_LINUX__ )

      struct sched_param sp;
      memset(&sp, 0, sizeof(struct sched_param));


      sp.sched_priority = pThread->m_nPriority;
      pthread_setschedparam(pthread_self(), SCHED_RR, &sp);

#endif // OS

   }

   pThread->m_tid = CurrentThreadID();

   ThreadProc fn = pThread->m_pProc;

   if ( NULL != fn ) {
#ifdef DBG_OSLTHREAD
# include "dbg_oslthread_1.cpp"
#endif // DBG_OSLTHREAD

      fn(pThread, pThread->m_pContext);

#ifdef DBG_OSLTHREAD
# include "dbg_oslthread_2.cpp"
#endif // DBG_OSLTHREAD
   }

#if   defined( __AAL_WINDOWS__ )
   return 0;
#elif defined( __AAL_LINUX__ )
   return NULL;
#endif // OS
}

//=============================================================================
// Name: ~OSLThread
// Description: Destructor
// Interface: public
// Inputs:
// Outputs: none.
// Comments:
//=============================================================================
OSLThread::~OSLThread()
{
   // The thread is exiting. Post to the internal semaphore so that all waiters can wake (Wait() / Signal()).
   btInt CurrentCount = 0;
   btInt MaxCount     = 0;

   m_Semaphore.CurrCounts(CurrentCount, MaxCount);
   m_Semaphore.Post(INT_MAX - CurrentCount);

   {
      AutoLock(this);

      if ( flag_is_clr(m_State, THR_ST_OK) ||
           flag_is_set(m_State, THR_ST_LOCAL|THR_ST_JOINED) ) {
         // If we didn't construct successfully, or if we're a "local" thread, or if we've already
         //  been Join()'ed, then there's nothing left to do.
         return;
      }

      Detach();  // Detach it to free resources - this thread won't be Join()'ed.
   }

   if ( IsThisThread( GetThreadID() ) ) {
      // When self-destructing, allow the thread to return from the d'tor so that
      // the C++ runtime releases the memory allocated for this.
      ;
   } else {

      Cancel();  // Mark the thread for termination at the next cancellation point.
                 // TODO: refer to man pthread_setcanceltype
                 //       the default cancellation type for threads is PTHREAD_CANCEL_DEFERRED,
                 //       which means that cancellation requests are deferred until the target
                 //       thread next calls a cancellation point.
                 //       Use pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL) to cancel
                 //       a thread asap.
   }

#ifdef __AAL_WINDOWS__
   CloseHandle(m_hThread);
#endif // __AAL_WINDOWS__
}

//=============================================================================
// Name: Unblock
// Description: Send a kill signal to a thread to unblock a system call.
// Interface: public
// Inputs:
// Outputs: none.
// Comments: signum defaults to SIGIO from header file
//=============================================================================
void OSLThread::Unblock()
{
   {
      AutoLock(this);

      // We can only manipulate m_Thread when OK and !Local.
      if ( flag_is_clr(m_State, THR_ST_OK) ||
           flag_is_set(m_State, THR_ST_LOCAL) ) {

         if ( flag_is_set(m_State, THR_ST_LOCAL) ) {
            flag_setf(m_State, THR_ST_UNBLOCKED);
         }
         // Nothing else to do.
         return;
      }

      flag_setf(m_State, THR_ST_UNBLOCKED);
   }

#if   defined( __AAL_WINDOWS__ )

   //TODO
//   SetEvent(m_hEvent);

#elif defined( __AAL_LINUX__ )

   // Mark the thread for termination
   pthread_kill(m_Thread, SIGIO);

#endif // OS
}

//=============================================================================
// Name: Signal
// Description: Signal the event on thread local sync object
// Interface: public
// Inputs:
// Outputs: none.
// Comments:
//=============================================================================
void OSLThread::Signal()
{
   m_Semaphore.Post(1);
}

//=============================================================================
// Name: Wait
// Description: Thread local sync object (timed)
// Interface: public
// Inputs:
// Outputs: none.
// Comments:
//=============================================================================
void OSLThread::Wait(btTime ulMilliseconds)
{
   m_Semaphore.Wait(ulMilliseconds);
}

//=============================================================================
// Name: Wait
// Description: Thread local sync object
// Interface: public
// Inputs:
// Outputs: none.
// Comments:
//=============================================================================
void OSLThread::Wait()
{
   m_Semaphore.Wait();
}

//=============================================================================
// Name: Join
// Description: Wait for thread to exit
// Interface: public
// Inputs:
// Outputs: none.
// Comments:
//=============================================================================
void OSLThread::Join()
{
   {
      AutoLock(this);

      if ( flag_is_clr(m_State, THR_ST_OK) ||
           flag_is_set(m_State, THR_ST_LOCAL|THR_ST_JOINED|THR_ST_DETACHED) ) {

         if ( flag_is_set(m_State, THR_ST_LOCAL) ) {
            flag_setf(m_State, THR_ST_JOINED);
         }
#if ENABLE_ASSERT
         else {
            const btBool OSLThread_attempt_to_Join_when_already_Joined_or_Detached = false;
            ASSERT(OSLThread_attempt_to_Join_when_already_Joined_or_Detached);
         }
#endif // ENABLE_ASSERT

         // Nothing else to do.
         return;
      }

      flag_setf(m_State, THR_ST_JOINED);
   }

   // Don't join while locked.

#if   defined( __AAL_WINDOWS__ )

   WaitForSingleObject(m_hThread, INFINITE);
   CloseHandle(m_hThread);

#elif defined( __AAL_LINUX__ )

   void *ret;
   ::pthread_join(m_Thread, &ret);

#endif // OS
}

// The underlying thread resource will never be join()'ed.
void OSLThread::Detach()
{
   {
      AutoLock(this);

      if ( flag_is_clr(m_State, THR_ST_OK) ||
           flag_is_set(m_State, THR_ST_LOCAL|THR_ST_JOINED|THR_ST_DETACHED) ) {

         if ( flag_is_set(m_State, THR_ST_LOCAL) ) {
            flag_setf(m_State, THR_ST_DETACHED);
         }
#if ENABLE_ASSERT
         else {
            const btBool OSLThread_attempt_to_Detach_when_already_Joined_or_Detached = false;
            ASSERT(OSLThread_attempt_to_Detach_when_already_Joined_or_Detached);
         }
#endif // ENABLE_ASSERT

         // Nothing else to do.
         return;
      }

      flag_setf(m_State, THR_ST_DETACHED);
   }

#if defined( __AAL_LINUX__ )
   // Detach it to free resources.
   ::pthread_detach(m_Thread);
#endif // OS
}

//=============================================================================
// Name: Cancel
// Description: Do a pthread cancel
// Interface: public
// Inputs:
// Outputs: none.
// Comments:
//=============================================================================
void OSLThread::Cancel()
{
   {
      AutoLock(this);

      if ( flag_is_clr(m_State, THR_ST_OK) ||
           flag_is_set(m_State, THR_ST_LOCAL|THR_ST_JOINED) ) {

         if ( flag_is_set(m_State, THR_ST_LOCAL) ) {
            flag_setf(m_State, THR_ST_CANCELED);
         }
#if ENABLE_ASSERT
         else {
            const btBool OSLThread_attempt_to_Cancel_when_already_Joined = false;
            ASSERT(OSLThread_attempt_to_Cancel_when_already_Joined);
         }
#endif // ENABLE_ASSERT

         // Nothing to do.
         return;
      }

      flag_setf(m_State, THR_ST_CANCELED);
   }

#if   defined( __AAL_WINDOWS__ )

   TerminateThread(m_hThread, (DWORD)-1);

#elif defined( __AAL_LINUX__ )

   // Mark the thread for termination
   ::pthread_cancel(m_Thread);

#endif // OS
}

btBool OSLThread::IsThisThread(btTID id) const
{
#if   defined( __AAL_WINDOWS__ )
   return id == m_tid;
#elif defined( __AAL_LINUX__ )
   return 0 != pthread_equal(id, m_tid);
#endif // OS
}

//=============================================================================
// Name: tid
// Description: return the OS threadID
// Interface: public
// Inputs:
// Outputs: none.
// Comments:
//=============================================================================
btTID OSLThread::tid()
{
   return m_tid;
}

/*
//=============================================================================
// Name: SetThreadPriority
// Description:
// Interface: public
// Inputs:
// Outputs: none.
// Comments:
//=============================================================================
OSAL_API void SetThreadPriority(OSLThread::ThreadPriority nPriority)
{
   if ( ( nPriority >= 0 ) &&
        ( (btUnsignedInt)nPriority < (sizeof(OSLThread::sm_PriorityTranslationTable) / sizeof(OSLThread::sm_PriorityTranslationTable[0])) ) ) {

      btInt pri = OSLThread::sm_PriorityTranslationTable[nPriority];

#if   defined( __AAL_WINDOWS__ )

      ::SetThreadPriority(GetCurrentThread(), pri);

#elif defined( __AAL_LINUX__ )

      struct sched_param sp;
      memset(&sp, 0, sizeof(struct sched_param));

      sp.sched_priority = pri;
      pthread_setschedparam(pthread_self(), SCHED_RR, &sp);

#endif // OS

   }
}
*/

//=============================================================================
// Name: GetProcessID
// Description: Return processID
// Interface: public
// Inputs:
// Outputs: none.
// Comments:
//=============================================================================
btPID GetProcessID()
{
#if   defined( __AAL_WINDOWS__ )

   return (btPID) ::GetCurrentProcessId();

#elif defined( __AAL_LINUX__ )

   return (btPID) ::getpid();

#endif // OS
}

//=============================================================================
// Name: GetThreadID
// Description: Return threadID
// Interface: public
// Inputs:
// Outputs: none.
// Comments:
//=============================================================================
btTID GetThreadID()
{
#if   defined( __AAL_WINDOWS__ )

   return (btTID) ::GetCurrentThreadId();

#elif defined( __AAL_LINUX__ )

   return (btTID) ::pthread_self();

#endif // OS
}

btBool ThreadIDEqual(btTID x, btTID y)
{
#if   defined( __AAL_WINDOWS__ )

   return x == y;

#elif defined( __AAL_LINUX__ )

   return 0 != ::pthread_equal(x, y);

#endif // OS
}

void ExitCurrentThread(btUIntPtr ExitStatus)
{
#if   defined( __AAL_WINDOWS__ )
# ifdef DBG_OSLTHREAD
#    include "dbg_oslthread_2.cpp"
# endif // DBG_OSLTHREAD

   ::ExitThread((DWORD)ExitStatus);

#elif defined( __AAL_LINUX__ )

   ::pthread_exit((void *)ExitStatus);

#endif // OS
}

/*
//=============================================================================
// Name: GetNumProcessors
// Description:
// Interface: public
// Inputs:
// Outputs: none.
// Comments:
//=============================================================================
OSAL_API btInt GetNumProcessors()
{
#if   defined( __AAL_WINDOWS__ )

   return 1;   // TODO GetNumProcessors() fixed for now
   // SYSTEM_INFO si;
   // GetSystemInfo(&si);
   // return (int) si.dwNumberOfProcessors;

#elif defined( __AAL_LINUX__ )

   return 1;   // TODO GetNumProcessors() fixed for now
   // return sysconf(_SC_NPROCESSORS_ONLN);

#endif // OS
}
*/

OSAL_API btUnsigned32bitInt GetRand(btUnsigned32bitInt *storage)
{
   ASSERT(NULL != storage);

   unsigned int seed = 0;
   // if the storage is NULL, use zero as the seed and call this
   // function recursively (ensuring not to pass NULL as the storage)
   if (NULL == storage){
      return GetRand(&seed);
   }

#if   defined( __AAL_WINDOWS__ )
   seed = (unsigned int)*storage;
   ::rand_s(&seed);
   return (*storage = (btUnsigned32bitInt)seed);
#elif defined( __AAL_LINUX__ )
   seed = (unsigned int)*storage;
   btUnsigned32bitInt val  = (btUnsigned32bitInt) ::rand_r(&seed);
   *storage = (btUnsigned32bitInt)seed;
   return val;
#endif // OS
}

END_NAMESPACE(AAL)

