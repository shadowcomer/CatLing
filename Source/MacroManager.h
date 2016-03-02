/*
    This class [MacroManager] is a specification of the Module class.

    The MacroManager class is responsible for large scale, long term control
    of resources and buildings (Infrastructure).

    Other modules depend on the MacroManager because it's the starting point
    of any possible plan; without infrastructure, no long term progress can be achieved.

    The MacroManager's objectives are influenced by the necessities and requirements
    of the other modules. It is never self-imposed, because there's no primary, urgent,
    need for it.

    Because of this, the MacroManager can be considered a planner / optimizer, though it
    doesn't come up with its own objectives.
*/

#ifndef MACROMANAGER_H
#define MACROMANAGER_H

#include "../include/TBB/tbb/tbb.h"
#include "../include/TBB/tbb/compat/thread"
#include "../include/TBB/tbb/concurrent_queue.h"

#include "Module.h"

#include "BehaviorTree.h"
#include "Sequence.h"
#include "ActionBehavior.h"

#include <BWAPI.h>

#include <iostream>

class MacroManager : public Module
{
public:
    MacroManager(Tasker& tsk);
    ~MacroManager();

    /*
    Implementation of Module's launch function.
    */
    void launch();

private:
    BWAPI::Unit m_command;

    /*
    Implementation of the MacroManager's shutdown procedure.
    */
    bool shutdownHelper();

    /*
    Builds a BehaviorTree for building a barracks.
    */
    std::unique_ptr<bt::BehaviorTree> buildBarracksTree();

    /*
    Checks if the macromanager has enough resources to build a barracks.
    */
    bool canBuildBarracks();

    /*
    Commits the resources for the given unit, removing them
    from its virtual budget.
    */
    void commitResources(BWAPI::UnitType unit);

    /*
    Implementation of the MacroManager's module process.
    */
    static void run(MacroManager* m);

};

#endif