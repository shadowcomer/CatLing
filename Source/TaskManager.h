#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include "Tasker.h"
#include "Executer.h"

class TaskManager
{
public:
	TaskManager();
	~TaskManager();

	Tasker& getInputInterface();
	Executer& getOutputInterface();

	bool hasRemainingTasks();

private:
	Tasker m_inputInterface;
	Executer m_outputInterface;

	tbb::concurrent_queue<Task*> m_taskQueue;

};

#endif