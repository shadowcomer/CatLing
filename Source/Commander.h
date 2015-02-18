#ifndef COMMANDER_H
#define COMMANDER_H

#include "../include/TBB/tbb/tbb.h"
#include "../include/TBB/tbb/compat/thread"
#include "../include/TBB/tbb/concurrent_queue.h"

#include "Tasker.h"
#include "Module.h"

#include <BWAPI.h>

#include <iostream>


class Commander : public Module
{
public:
	// TODO: Change argument list to Job requests
	Commander(Tasker& tsk);
	~Commander();

	void launch();

	Tasker& tasker();

private:
	Tasker& m_tasker;
	
	bool shutdownHelper();
	static void run(Commander* m);
};
#endif