//============================================================================
// Name        : main.cpp
// Author      : Artak Avetyan
// Version     :
// Copyright   : (c) 2021-2023 Aregtech UG.All rights reserved.
// Description : This project demonstrates the use of synchronization objects,
//               including waiting for multiple synchronization objects such 
//               as mutex and synchronization events, that differ by types 
//               such as automatic or manual reset, or initially locked or 
//               unlocked, etc, and how they may be combined in one waiting 
//               scope. For example, a thread can wait for one, many or a
//               mixture of synchronization objects such as Mutex or 
//               synchronization events.
//
//               The synchronization events are not part of POSIX standard, 
//               but exists in Windows. By business logic they have similarity
//               with POSIX signals of conditional variables. Synchronization
//               events are powerful and useful objects.
//============================================================================

#include "areg/base/GEGlobal.h"

#include "areg/base/DateTime.hpp"
#include "areg/base/IEThreadConsumer.hpp"
#include "areg/base/Thread.hpp"

#include "areg/component/DispatcherThread.hpp"
#include "areg/component/Event.hpp"

#include "areg/logging/GELog.h"

#include <chrono>

#ifdef  _WIN32
    // link with areg library, valid only for MSVC
    #pragma comment(lib, "areg.lib")
#endif // _WIN32

class HelloThread;
class GoodbyeThread;

//////////////////////////////////////////////////////////////////////////
// Synchronization objects
//////////////////////////////////////////////////////////////////////////

static SynchEvent  gEventExit(true, false);    //!< Non-signaled, manual reset event
static SynchEvent  gEventRun(true, true);      //!< Non-signaled, auto-reset event
static Mutex       gMutexWait(false);          //!< Not locked mutex
static Mutex       gMutexDummy(false);         //!< Not locked mutex

//!< HelloThread class declaration, simple thread.
class HelloThread   : public    Thread
                    , protected IEThreadConsumer
{
public:
    HelloThread( void );

    virtual ~HelloThread( void ) = default;

protected:
/************************************************************************/
// IEThreadConsumer interface overrides
/************************************************************************/
    /**
     * \brief   The callback is called from Thread, when it is running and fully operable.
     **/
    virtual void onThreadRuns( void ) override;

private:
    inline HelloThread & self( void )
    {
        return (*this);
    }

public:
    SynchEvent  mQuit;  //!< Event, signaled when thread completes job.
};

//!< GoodbyeThread class declaration, simple thread.
class GoodbyeThread : public    Thread
                    , protected IEThreadConsumer
{
public:
    GoodbyeThread( void );
    virtual ~GoodbyeThread( void ) = default;

protected:
/************************************************************************/
// IEThreadConsumer interface overrides
/************************************************************************/
    /**
     * \brief   This callback function is called from Thread object, when it is
     *          running and fully operable.
     **/
    virtual void onThreadRuns( void ) override;

//////////////////////////////////////////////////////////////////////////
// Hidden calls
//////////////////////////////////////////////////////////////////////////
private:
    inline GoodbyeThread & self( void )
    {
        return (*this);
    }

public:
    SynchEvent  mQuit;  //!< Event, signaled when thread completes job.
};

DEF_LOG_SCOPE(main_HelloThread_HelloThread);
DEF_LOG_SCOPE(main_HelloThread_onThreadRuns);

HelloThread::HelloThread( void )
    : Thread( self( ), "HelloThread" )
    , IEThreadConsumer  ( )
    , mQuit             (true, true)
{
    LOG_SCOPE(main_HelloThread_HelloThread);
    LOG_DBG( "Initialized thread [ %s ]", getName().getString() );
}

void HelloThread::onThreadRuns( void )
{
    LOG_SCOPE(main_HelloThread_onThreadRuns);

    LOG_INFO("The thread [ %s ] runs, going to output message", getName().getString());
    LOG_INFO("!!!Hello World!!! from thread [ %s ]", getName( ).getString( ) );

    // reset events
    mQuit.resetEvent();
    gMutexDummy.lock( );

    // lock, to wait auto-reset event
    gEventRun.lock(NECommon::WAIT_INFINITE);
    LOG_INFO("Auto-reset event \'gEventRun\' is signaled, locking again");

    // This multi-lock uses 3 synchronization events and one mutex
    IESynchObject * synchObjects[]  = {&gEventExit, &gMutexWait, &gEventRun};
    MultiLock multiLock(synchObjects, MACRO_ARRAYLEN( synchObjects ), false);

    do 
    {
        constexpr unsigned int waitTimeout{ NECommon::WAIT_1_MILLISECOND * 150 };

        // Wait until either all objects are signaled or until timeout expires.
        // If all events are signaled, exit thread.
        // If timeout is expired, make some job and wait again.
        int waitResult = multiLock.lock( waitTimeout, true, false);
        
        if (waitResult == MultiLock::LOCK_INDEX_ALL)
        {
            std::cout << "All synchronization objects are signaled, exiting thread." << std::endl;
            LOG_DBG("All waiting objects of thread [ %s ] are signaled, exit the job.", getName( ).getString( ) );
            break;  // exit loop
        }
        else if ( waitResult == MultiLock::LOCK_INDEX_TIMEOUT )
        {
            Lock lock( gMutexDummy );

            std::cout << "Wait multi-lock timeout expired, continue the job." << std::endl;
            LOG_DBG("Thread [ %s ] waiting timeout expired, continuing the job", getName( ).getString( ) );
            Thread::sleep(waitTimeout);
            LOG_DBG("Thread [ %s ] continues to wait", getName( ).getString( ) );
        }
        else
        {
            std::cerr << "Unexpected waiting result, breaking the loop" << std::endl;
            LOG_ERR("Unexpected waiting result, breaking the loop");
        }

    } while (true);

    Thread::sleep( NECommon::WAIT_100_MILLISECONDS );
    gMutexDummy.unlock();

    // set event to inform the thread completed job
    mQuit.setEvent();
}

//////////////////////////////////////////////////////////////////////////
// GoodbyeThread implementation
//////////////////////////////////////////////////////////////////////////

DEF_LOG_SCOPE(main_GoodbyeThread_GoodbyeThread);
DEF_LOG_SCOPE(main_GoodbyeThread_onThreadRuns);

GoodbyeThread::GoodbyeThread( void )
    : Thread( self( ), "GoodbyeThread" )
    , IEThreadConsumer  ( )
    , mQuit             (false, true)
{
    LOG_SCOPE(main_GoodbyeThread_GoodbyeThread);
    LOG_DBG("Initialized thread [ %s ]", getName( ).getString( ) );
}

void GoodbyeThread::onThreadRuns( void )
{
    LOG_SCOPE(main_GoodbyeThread_onThreadRuns);

    LOG_INFO("The thread [ %s ] runs, going to output message", getName().getString());
    LOG_INFO("!!!Hello World!!! from thread [ %s ]", getName( ).getString( ) );

    mQuit.resetEvent();

    // Initialize multi-lock object
    IESynchObject * synchObjects[]  = {&gEventExit, &gMutexDummy};
    MultiLock multiLock(synchObjects, MACRO_ARRAYLEN(synchObjects), false);

    // This multi-lock uses 1 synchronization event and 1 mutex, waits for any is signaled.
    int waitResult = multiLock.lock( NECommon::WAIT_INFINITE, false, false );
    std::cout << "Multi-lock is signaled the elem " << waitResult << " is unlocked" << std::endl;
    LOG_DBG( "Lock finished with result [ %d ]", waitResult );

    multiLock.unlock( waitResult );
    Thread::sleep( NECommon::WAIT_500_MILLISECONDS );

    // set event to inform the thread completed
    mQuit.setEvent();
}

DEF_LOG_SCOPE(main_main);
//! \brief   A Demo of synchronization objects, use of various synchronization objects in multi-lock.
int main()
{
    std::cout << "A Demo of synchronization objects, use of various synchronization objects in multi-lock ..." << std::endl;

    // Force to start logging. See outputs log files in appropriate "logs" sub-folder.
    LOGGING_CONFIGURE_AND_START( nullptr );

    do
    {
        // set this part of code in a block (for example, 'do-while' block).
        // otherwise, the logs will not be visible, since in the time when
        // scope is initialized, the logging is not active yet.
        LOG_SCOPE(main_main);

        // initialize synchronization object states
        gMutexWait.lock();
        gEventRun.resetEvent();
        gEventExit.resetEvent();

        // Hello thread.
        LOG_DBG("Starting Hello Thread");
        HelloThread helloThread;
        helloThread.createThread( NECommon::DO_NOT_WAIT);

        LOG_INFO( "Sleep main thread for [ %d ] ms, to signal \'gEventRun\' auto-reset event.", NECommon::WAIT_500_MILLISECONDS );
        Thread::sleep( NECommon::WAIT_500_MILLISECONDS);
        gEventRun.setEvent();                           // signal auto-reset event to let hello thread to run

        Thread::sleep( NECommon::WAIT_500_MILLISECONDS);// sleep to check whether auto-reset is working
        gMutexWait.unlock();                            // release mutex to let other thread to take ownership
        Thread::sleep( NECommon::WAIT_1_SECOND);        // sleep for no reason

        // Goodbye thread
        LOG_DBG("Starting Goodbye Thread");
        GoodbyeThread goodbyeThread;
        goodbyeThread.createThread(NECommon::WAIT_INFINITE);

        Thread::sleep( NECommon::WAIT_1_SECOND);        // sleep for no reason

        // Initialize multi-lock object, wait for all events
        IESynchObject * synchObjects[] = {&helloThread.mQuit, &goodbyeThread.mQuit, &gMutexDummy};
        gEventExit.setEvent();
        gEventRun.setEvent();

        MultiLock multiLock(synchObjects, MACRO_ARRAYLEN(synchObjects), true);

        std::cout << "All synchronization objects are unlocked. Completing all threads." << std::endl;

        // stop and destroy thread, clean resources. Wait until thread ends.
        LOG_INFO("The threads completed jobs, wait threads to shutdown to exit application");
        helloThread.shutdownThread(NECommon::WAIT_INFINITE);
        goodbyeThread.shutdownThread(NECommon::WAIT_INFINITE);

        constexpr uint32_t eventTimeout{ 1000 };
        LOG_INFO("Testing event synchronization object with event waiting timeout  [ %u ] ms", eventTimeout);
        std::cout << "Testing event synchronization object with event waiting timeout " << eventTimeout << " ms" << std::endl;

        DateTime start{ DateTime::getNow() };
        std::cout << "Start at: " << start.formatTime().getData() << std::endl;

        SynchEvent localEvent(false, false);
        localEvent.resetEvent();
        localEvent.lock(eventTimeout); // lock with timeout

        DateTime end{ DateTime::getNow() };
        std::chrono::nanoseconds ns{ end.getTime() - start.getTime() };
        std::chrono::microseconds ms{ std::chrono::duration_cast<std::chrono::microseconds>(ns)};
        std::cout << "End at: " << end.formatTime().getData() << std::endl;
        LOG_INFO("The lock timeout it [ %llu ]", ms.count());
        std::cout << "The event was locked for " << ms.count() << " ms" << std::endl;
        ASSERT(eventTimeout <= ms.count());
    } while (false);

    // Stop logging.
    LOGGING_STOP();

    std::cout << "Exit application, check the logs for details!" << std::endl;
    return 0;
}
