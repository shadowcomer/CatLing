#ifndef MODULE_H
#define MODULE_H

#include "../include/TBB/tbb/tbb.h"
#include "../include/TBB/tbb/compat/thread"

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
	Module();
	~Module();

	// Launches the module and thread
	virtual void launch() = 0;
	bool shutdown();
	bool isTerminating();

	tbb::tbb_thread& getThread();

protected:
	tbb::tbb_thread m_thread;

private:
	bool m_shuttingDown;
	virtual bool shutdownHelper() = 0;

	// What the thread does
	virtual void run() = 0;
};

#endif