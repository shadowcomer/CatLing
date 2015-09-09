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

class TaskManager
{
public:
    TaskManager();
    ~TaskManager();

    /*
    Returns a reference to the input interface.
    */
    Tasker& getInputInterface();

    /*
    Returns a reference to the output interface.
    */
    Executer& getOutputInterface();

    /*
    Returns whether the TaskManager has remaining tasks in the queue
    waiting for execution.
    */
    bool hasRemainingTasks();

private:
    Tasker m_inputInterface;
    Executer m_outputInterface;

    tbb::concurrent_queue<Task*> m_taskQueue;

};

#endif