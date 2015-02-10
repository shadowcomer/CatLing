#include "Tasker.h"

Tasker::Tasker(tbb::concurrent_queue<int>& taskQueue):
m_queue(taskQueue)
{

}

Tasker::~Tasker()
{

}

bool Tasker::requestTask()
{
	m_queue.push(1);
	return true;
}