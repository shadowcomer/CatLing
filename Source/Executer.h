#ifndef EXECUTER_H
#define EXECUTER_H

#include "../include/TBB/tbb/concurrent_queue.h"
#include "Task.h"

class Executer
{
public:
	Executer(tbb::concurrent_queue<Task*>& queue);
	~Executer();

	bool executeSingleTask();
	int executeAllTasks();
	bool hasRemainingTasks();

private:
	tbb::concurrent_queue<Task*>& m_queue;

};

#endif