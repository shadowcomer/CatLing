#ifndef MODULE_H
#define MODULE_H

#include <WinSock2.h>
#include <Windows.h>
#include "../include/TBB/tbb/tbb.h"
#include "../include/TBB/tbb/compat/thread"
#include <mongo\bson\bson.h>
#include <mongo\client\dbclient.h>
#include <mongo\client\init.h>

#include "Tasker.h"

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

	// Launches the module and thread
	virtual void launch() = 0;
	bool shutdown();
	bool isTerminating();
	int getFramesToWake();

	tbb::tbb_thread& getThread();

protected:
	tbb::tbb_thread m_thread;
	Tasker& m_tasker;
	Tasker& tasker();

private:
	int framesToWake;
	bool m_shuttingDown;
	virtual bool shutdownHelper() = 0;

};

#endif