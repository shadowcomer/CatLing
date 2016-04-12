/*
    This class represents an abstraction over functional modules, allowing for
    general control of setting up and destruction, as well as access to specific
    functionality that is shared by all of them.
*/

#ifndef MODULE_H
#define MODULE_H

#include <BWAPI.h>

#include <mutex>

#include "../include/TBB/tbb/tbb.h"
#include "../include/TBB/tbb/compat/thread"

#include <condition_variable>

#include "Tasker.h"
#include "SlabAllocator.h"

// Possible types. The _END value is there to automatically
// detect the last element and work accordingly.
enum ModuleType
{
    COMMANDER = 0,
    MACROMGR,
    MICROMGR,
    LEARNING,
    _END
};

class Module
{
public:
    Module(Tasker& tsk, Module** modules, SlabAllocator* alloc);
    ~Module();

    /*
    Launches the module and thread.
    Each module must implement it's own launching method.
    As a requirement, m_thread must be set to the thread that is
    finally in control of the module.

    For example,

        m_thread = tbb::tbb_thread(&MacroManager::run, this);

    is a valid launch.
    */
    virtual void launch() = 0;

    /*
    Tells this module to shutdown.
    This should only be called by a different thread than the module's.

    Calling this function will make the caller's thread wait on the
    module to notify back it's termination.
    */
    bool shutdown();

    /*
    Returns whether this module is in a shutting down state.
    */
    bool isShuttingDown();

    /*
    Returns the last frame this module was called to execute.
    */
    int lastExecFrame();

    /*
    Sets the minimum number of frames between which the module should 
    wake after each complete execution.
    */
    void setFrameExecDelta(int delta);

    /*
    Returns the number of frames between which the module should wake
    after each complete execution.
    */
    int getFrameExecDelta();

    /*
    Tells this module to start the next iteration of execution.
    */
    void resumeExecution();

    /*
    Returns the thread that's in control of this module.
    */
    tbb::tbb_thread& getThread();

    /*
    Sets the SlabAllocator that this module has access to.
    */
    bool setAllocator(SlabAllocator* allocator);

    /*
    Notifies anyone who might be waiting on the execution of the
    module to terminate that it has finished shutting down.
    */
    void notifyShutdownCompletion();

protected:
    tbb::tbb_thread m_thread;
    Tasker& m_tasker;
    SlabAllocator* m_allocator;

    Tasker& tasker();

    Module** m_otherModules;

    /*
    Completes the current execution and blocks the thread until it's
    awoken by a call by resumeExecution.
    */
    void sleepExecution();

private:
    long int m_lastExecFrame;
    long int m_frameExecDelta;

    // Mutex and condition variables for controlling execution.
    std::mutex m_workMutex;
    std::condition_variable m_workCond;
    bool m_shouldWake;

    // Mutex and condition variables for shutdown control
    std::mutex m_shutdownMutex;
    std::condition_variable m_shutdownCond;
    bool m_shutdown;
    bool m_shuttingDown;

    /*
    Sets the frame the current execution started on.
    */
    void setLastExecFrame(int frame);

    /*
    Extra shutdown functionality implemented by each specific module.
    */
    virtual bool shutdownHelper() = 0;

};

#endif