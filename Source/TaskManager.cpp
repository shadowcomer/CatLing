#include "TaskManager.h"

TaskManager::TaskManager():
m_inputInterface(m_taskQueue),
m_outputInterface(m_taskQueue)
{

}

TaskManager::~TaskManager()
{

}

bool TaskManager::hasRemainingTasks()
{
	return !m_taskQueue.empty();
}

Tasker& TaskManager::getInputInterface()
{
	return m_inputInterface;
}

Executer& TaskManager::getOutputInterface()
{
	return m_outputInterface;
}