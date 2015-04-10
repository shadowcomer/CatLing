#include "Tasker.h"

Tasker::Tasker(tbb::concurrent_queue<Task*>& queue) :
m_queue(queue)
{

}

Tasker::~Tasker()
{

}

bool Tasker::requestTask(Task* t)
{
	// TODO: Add safety to this call
	m_queue.push(t);
	return true;
}