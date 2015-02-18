#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include "Tasker.h"
#include "Executer.h"

class TaskManager
{
public:
	TaskManager();
	~TaskManager();

	const Tasker& getInputInterface();
	const Executer& getOutputInterface();

	bool hasRemainingTasks();

	friend class Tasker;
	friend class Executer;

protected:
	void addTask(Task* t);
	Task* removeTask();

private:
	Tasker m_inputInterface;
	Executer m_outputInterface;

	tbb::concurrent_queue<Task*> m_taskQueue;

};

#endif