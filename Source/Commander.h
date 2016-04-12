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

#include "ResourceManager.h"

class Commander : public Module
{
public:
    Commander(Tasker& tsk, Module** modules, SlabAllocator* alloc);
    ~Commander();

    /*
    Implementation of Module's launch function.
    */
    void launch();

    /*
    Updates the resource budget for each module. This is for use
    on each frame.
    */
    void updateBudget();

    ResourceManager* getResourceManager();

private:
    BWAPI::Unit m_command;

    std::unique_ptr<ResourceManager> m_resources;

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