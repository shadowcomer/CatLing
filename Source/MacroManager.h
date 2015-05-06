#ifndef MACRO_H
#define MACRO_H

#include "../include/TBB/tbb/tbb.h"
#include "../include/TBB/tbb/compat/thread"
#include "../include/TBB/tbb/concurrent_queue.h"

#include "Tasker.h"
#include "Module.h"

#include <BWAPI.h>

#include <iostream>

class MacroManager : public Module
{
public:
	// TODO: Change argument list to Job requests
	MacroManager(Tasker& tsk);
	~MacroManager();

	void launch();

	Tasker& tasker();

private:
	Tasker& m_tasker;

	bool shutdownHelper();
	static void run(MacroManager* m);
};
#endif