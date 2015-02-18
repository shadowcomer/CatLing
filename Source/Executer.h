#ifndef EXECUTER_H
#define EXECUTER_H

#include "Task.h"
#include "TaskManager.h"

class Executer
{
public:
	friend class TaskManager;
	~Executer();

	void executeTask();
	bool hasRemainingTasks();

protected:
	Executer(TaskManager& mgr);

private:
	TaskManager& m_manager;

};

#endif