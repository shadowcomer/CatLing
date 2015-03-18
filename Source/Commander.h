#ifndef COMMANDER_H
#define COMMANDER_H

#include "../include/TBB/tbb/tbb.h"
#include "../include/TBB/tbb/compat/thread"
#include "../include/TBB/tbb/concurrent_queue.h"

#include "Module.h"

#include <BWAPI.h>

#include <iostream>

class Commander : public Module
{
public:
	Commander(Tasker& tsk);
	~Commander();

	void launch();

private:
	
	bool shutdownHelper();
	static void run(Commander* m);
};
#endif