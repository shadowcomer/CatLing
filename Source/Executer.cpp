#include "Executer.h"

Executer::Executer(TaskManager& mgr) :
m_manager(mgr)
{

}

Executer::~Executer()
{

}

bool Executer::hasRemainingTasks()
{
	return m_manager.hasRemainingTasks();
}

void Executer::executeTask()
{
	Task* t;
	t = m_manager.removeTask();
	if (nullptr != t)
	{
		t->execute();
	}

}