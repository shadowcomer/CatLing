#ifndef TASKER_H
#define TASKER_H

#include "../include/TBB/tbb/concurrent_queue.h"

#include "Task.h"

class Tasker
{
public:
	// TODO: Change template type to Task
	Tasker(tbb::concurrent_queue<Task>& taskQueue);
	~Tasker();

	bool requestTask(Task& t);

private:
	tbb::concurrent_queue<Task>& m_queue;

};

#endif