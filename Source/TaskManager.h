/*
    The TaskManager is in control of the task queue which contains
    every action to be sent to the Starcraft process.

    To be able to add or remove elements from this queue, an interface
    has to be used: InputInterface or OutputInterface.

    The TaskManager manages Tasks, which are safe implementations of calls
    to the possible actions of the Starcraft process.
*/

#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include "Tasker.h"
#include "Executer.h"

class TaskManager : public Tasker, public Executer
{
public:
    TaskManager();
    ~TaskManager();

    /*
    Returns whether the TaskManager has remaining tasks in the queue
    waiting for execution.
    */
    bool hasRemainingTasks() override;

    /*
    Execute a single task from the Executer's queue.
    */
    bool executeSingleTask() override;

    /*
    Execute every task from the Executer's queue.
    */
    int executeAllTasks() override;

    /*
    Appends a Task to the end of the queue of this Tasker.
    */
    bool requestTask(Task* t) override;

private:
    tbb::concurrent_queue<Task*> m_taskQueue;

};

#endif
