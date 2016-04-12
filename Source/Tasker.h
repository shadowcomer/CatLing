/*
    The Tasker is the Input interface for the TaskManager.

    The Tasker should not be built manually, but rather acquired
    through the call to getInputInterface of TaskManager. This is
    because the TaskManager is responsible for the association
    between its internal queue and the Tasker's queue.
*/

#ifndef TASKER_H
#define TASKER_H

#include "../include/TBB/tbb/concurrent_queue.h"
#include "Task.h"

class Task;

class Tasker
{
public:
    /*
    Appends a Task to the end of the queue of this Tasker.
    */
    virtual bool requestTask(Task* t) = 0;
};

#endif