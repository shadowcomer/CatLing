#ifndef TASKER_H
#define TASKER_H

#include "../include/TBB/tbb/concurrent_queue.h"

class Tasker
{
public:
	// TODO: Change template type to Task
	Tasker(tbb::concurrent_queue<int>& taskQueue);
	~Tasker();

	bool requestTask();

private:
	tbb::concurrent_queue<int>& m_queue;

};

#endif