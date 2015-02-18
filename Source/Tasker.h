#ifndef TASKER_H
#define TASKER_H

#include "../include/TBB/tbb/concurrent_queue.h"

#include "Task.h"
#include "TaskManager.h"

class Tasker
{
public:
	friend class TaskManager;
	~Tasker();

	bool requestTask(Task* t);
protected:
	Tasker(TaskManager& mgr);

private:
	TaskManager& m_manager;

};

#endif