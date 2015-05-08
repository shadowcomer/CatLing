#ifndef MACROMANAGER_H
#define MACROMANAGER_H

#include "Module.h"
#include "../include/TBB/tbb/tbb.h"
#include "../include/TBB/tbb/compat/thread"
#include "../include/TBB/tbb/concurrent_queue.h"

#include <BWAPI.h>

#include <iostream>

class MacroManager : public Module
{
public:
	MacroManager(Tasker& tsk);
	~MacroManager();

	void launch();

private:
	BWAPI::Unit m_command;

	bool shutdownHelper();
	static void run(MacroManager* m);

};

#endif