#include "Tasker.h"

Tasker::Tasker(tbb::concurrent_queue<Task*>& taskQueue):
m_queue(taskQueue)
{

}

Tasker::~Tasker()
{

}

bool Tasker::requestTask(Task* t)
{
	m_queue.push(t);
	return true;
}