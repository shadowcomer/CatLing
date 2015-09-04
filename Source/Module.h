/*
    This class represents an abstraction over functional modules, allowing for
    general control of setting up and destruction, as well as access to specific
    functionality that is shared by all of them.
*/

#ifndef MODULE_H
#define MODULE_H

#include "../include/TBB/tbb/tbb.h"
#include "../include/TBB/tbb/compat/thread"

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
	Module(Tasker& tsk);
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
    */
	bool shutdown();

    /*
    Returns whether this module is in a shutting down state.
    */
	bool isTerminating();

    /*
    Returns the number of frames that should pass inbetween wakes to
    this module.
    */
	int getFramesToWake();

    /*
    Returns the thread that's in control of this module.
    */
	tbb::tbb_thread& getThread();

    /*
    Sets the SlabAllocator that this module has access to.
    */
	bool setAllocator(SlabAllocator* allocator);

protected:
	tbb::tbb_thread m_thread;
	Tasker& m_tasker;
	SlabAllocator* m_allocator;

	Tasker& tasker();

private:
	int framesToWake;
	bool m_shuttingDown;

    /*
    Extra shutdown functionality implemented by each specific module.
    */
	virtual bool shutdownHelper() = 0;

};

#endif