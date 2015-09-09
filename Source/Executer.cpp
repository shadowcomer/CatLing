#include "Executer.h"

Executer::Executer(tbb::concurrent_queue<Task*>& queue) :
m_queue(queue)
{

}

Executer::~Executer()
{

}

bool Executer::hasRemainingTasks()
{
    return !m_queue.empty();
}

bool Executer::executeSingleTask()
{
    Task* t;
    m_queue.try_pop(t);
    if (nullptr != t)
    {
        t->execute();
        delete t;
        return true;
    }

    return false;
}

int Executer::executeAllTasks()
{
    int executedTasks = 0;
    while (hasRemainingTasks())
    {
        executeSingleTask();
        executedTasks++;
    }

    return executedTasks;
}