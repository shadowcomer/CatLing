#ifndef COMMANDER_H
#define COMMANDER_H

#include "Module.h"
#include "../include/TBB/tbb/tbb.h"
#include "../include/TBB/tbb/compat/thread"
#include "../include/TBB/tbb/concurrent_queue.h"

#include <BWAPI.h>

#include <iostream>

class Commander : public Module
{
public:
	Commander(Tasker& tsk);
	~Commander();

	void launch();

private:
	BWAPI::Unit m_command;
	bool shutdownHelper();
	static void run(Commander* m);
	mongo::DBClientConnection c;

};
#endif