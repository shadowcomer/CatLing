#include "Tasker.h"

Tasker::Tasker(TaskManager& mgr):
m_manager(mgr)
{

}

Tasker::~Tasker()
{

}

bool Tasker::requestTask(Task* t)
{
	// TODO: Add safety to this call
	m_manager.addTask(t);
	return true;
}