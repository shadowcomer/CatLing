#include "TaskManager.h"

TaskManager::TaskManager():
m_inputInterface(*this),
m_outputInterface(*this)
{

}

TaskManager::~TaskManager()
{

}

void TaskManager::addTask(Task* t)
{
	m_taskQueue.push(t);
}

bool TaskManager::hasRemainingTasks()
{
	return !m_taskQueue.empty();
}

Task* TaskManager::removeTask()
{
	Task* result;
	m_taskQueue.try_pop(result);
	return result;
}

const Tasker& TaskManager::getInputInterface()
{
	return m_inputInterface;
}