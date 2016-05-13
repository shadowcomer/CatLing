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

#include "BehaviorTree.h"
#include "Sequence.h"
#include "ActionBehavior.h"

#include <iostream>

#include "IResourceViewer.h"
#include "IResources.h"

class Commander : public Module
{
public:
    Commander(Tasker& tsk,
        std::shared_ptr<Module>* modules,
        SlabAllocator* alloc,
        ResViewer_p viewer,
        Resources_p editor);
    ~Commander();

    /*
    Implementation of Module's launch function.
    */
    void launch();

private:
    BWAPI::Unit m_command;

    ResViewer_p m_resViewer;
    Resources_p m_resMgr;

    int m_virtAccumMinerals;
    int m_virtAccumGas;

    /*
    Builds a BehaviorTree for mineral gathering.
    */
    std::unique_ptr<bt::BehaviorTree> buildGatherMinerals();

    /*
    Implementation of the Commander's shutdown procedure.
    */
    bool shutdownHelper();

    /*
    Retrieves the current virtual minerals available for
    budgeting.
    */
    int availableMinerals();

    /*
    Retrieves the current virtual gas available for budgeting.
    */
    int availableGas();

    /*
    Implementation of the Commander's module process.
    */
    static void run(Commander* m);
};
#endif