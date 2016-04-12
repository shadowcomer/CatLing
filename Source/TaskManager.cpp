#include "TaskManager.h"

TaskManager::TaskManager() {

}

TaskManager::~TaskManager()
{

}

bool TaskManager::hasRemainingTasks()
{
    return !m_taskQueue.empty();
}

bool TaskManager::hasRemainingTasks() {
    return !m_taskQueue.empty();
}

bool TaskManager::executeSingleTask() {
    Task* t;
    m_taskQueue.try_pop(t);
    if (nullptr != t) {
        t->execute();
        delete t;
        return true;
    }

    return false;
}

int TaskManager::executeAllTasks() {
    int executedTasks = 0;
    while (hasRemainingTasks()) {
        executeSingleTask();
        executedTasks++;
    }

    return executedTasks;
}

bool TaskManager::requestTask(Task* t) {
    m_taskQueue.push(t);
    return true;
}

