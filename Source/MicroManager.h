#ifndef MICRO_H
#define MICRO_H

#include "Module.h"
#include "../include/TBB/tbb/tbb.h"
#include "../include/TBB/tbb/compat/thread"
#include "../include/TBB/tbb/concurrent_queue.h"

#include "Tasker.h"

#include <BWAPI.h>

#include <iostream>
class MicroManager : public Module
{
public:
	MicroManager(Tasker& tsk);
	~MicroManager();

	void launch();

private:

	bool shutdownHelper();
	static void run(MicroManager* m);
};
#endif

