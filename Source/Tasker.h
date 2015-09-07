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

class Tasker
{
public:
    /*
    Do not call this constructor manually. The TaskManager is responsible
    for building this object, using for its queue parameter its own queue.
    */
	Tasker(tbb::concurrent_queue<Task*>& queue);
	~Tasker();

    /*
    Appends a Task to the end of the queue of this Tasker.
    */
	bool requestTask(Task* t);

private:
	tbb::concurrent_queue<Task*>& m_queue;

};

#endif