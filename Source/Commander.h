/*
    This class [Commander] is a specification of the Module class.

    The Commander class is responsible for a high-level approach to the control
    of the AI. Thus, it oversees execution and guides the general objective of the
    AI during the game.

    The Commander class is not, however, the final decision-taker; it only spurs
    the general course of action towards it's objective, trying to make the other modules
    work with it.
*/

#ifndef COMMANDER_H
#define COMMANDER_H

#include "../include/TBB/tbb/tbb.h"
#include "../include/TBB/tbb/compat/thread"
#include "../include/TBB/tbb/concurrent_queue.h"

#include "Module.h"

#include <iostream>

class Commander : public Module
{
public:
    Commander(Tasker& tsk);
    ~Commander();

    /*
    Implementation of Module's launch function.
    */
    void launch();

private:
    BWAPI::Unit m_command;

    /*
    Implementation of the Commander's shutdown procedure.
    */
    bool shutdownHelper();

    /*
    Implementation of the Commander's module process.
    */
    static void run(Commander* m);
};
#endif