/*
    The Executer class is responsible for executing the tasks
    that have been assigned to the queue controlled by the
    associated TaskManager.

    The Executer should not be built manually, but rather acquired
    through the call to geOutputInterface of TaskManager. This is
    because the TaskManager is responsible for the association
    between its internal queue and the Executer's queue.
*/

#ifndef EXECUTER_H
#define EXECUTER_H

#include "../include/TBB/tbb/concurrent_queue.h"
#include "Task.h"

class Executer
{
public:
    /*
    Execute a single task from the Executer's queue.
    */
    virtual bool executeSingleTask() = 0;

    /*
    Execute every task from the Executer's queue.
    */
    virtual int executeAllTasks() = 0;

    /*
    Returns whether the Executer has remaining tasks in the queue
    waiting for execution.
    */
    virtual bool hasRemainingTasks() = 0;
};

#endif