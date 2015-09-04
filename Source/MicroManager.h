/*
    This class [MicroManager] is a specification of the Module class.

    The MicroManager is responsible for the control of individual units.
    The MicroManager can have multiple simultaneous objectives, which it
    can personalize and adapt accordingly, depending on the units it controls.

    For example, simultaneously the MicroManager may want to explore the map
    and attack the enemy. It may then assign tasks to different units to
    simultaneously accomplish both objectives, taking into consideration the
    importance of the objectives and the resources (units) it has control over.
*/

#ifndef MICRO_H
#define MICRO_H

#include "../include/TBB/tbb/tbb.h"
#include "../include/TBB/tbb/compat/thread"
#include "../include/TBB/tbb/concurrent_queue.h"

#include "Tasker.h"
#include "Module.h"

#include <BWAPI.h>

#include <iostream>
class MicroManager : public Module
{
public:
	MicroManager(Tasker& tsk);
	~MicroManager();

    /*
    Implementation of Module's launch function.
    */
	void launch();

private:

    /*
    Implementation of the MicroManager's shutdown procedure.
    */
	bool shutdownHelper();

    /*
    Implementation of the MicroManager's module process.
    */
	static void run(MicroManager* m);
};
#endif

