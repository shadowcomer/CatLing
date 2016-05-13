#include "MacroManager.h"
#include "Commander.h"

#include <assert.h>

using namespace BWAPI;

MacroManager::MacroManager(Tasker& tsk,
    std::shared_ptr<Module>* modules,
    SlabAllocator* alloc,
    Resources_p requester,
    ResViewer_p viewer) :
Module(tsk, modules, alloc) {
    m_planner = std::make_shared<MacroPlanner>(
        m_allocator, requester);
}

MacroManager::~MacroManager()
{

}

void MacroManager::launch()
{
    m_thread = tbb::tbb_thread(&MacroManager::run, this);
}

bool MacroManager::shutdownHelper()
{

    return true;
}

void MacroManager::run(MacroManager* m)
{
    Unitset units = Broodwar->self()->getUnits();
    Slab* builderSlab = nullptr;

    {
        bool e = m->m_allocator->find("builders", &builderSlab);
        assert(e); // Make sure the slab exists
    }

    for (auto u : units)
    {
        if (u->getType().isResourceDepot())
        {
            m->m_command = u;
            break;
        }
    }

    m->setFrameExecDelta(48);

    std::unique_ptr<bt::BehaviorTree> currentPlan;
    bt::BehaviorTree::BTIterator currentBehavior;

    while(!m->isShuttingDown())
    {
        if (!currentPlan) {
            currentPlan = m->m_planner->getNextComplexAction();
            currentBehavior = currentPlan->begin();
        }

        if (currentBehavior != currentPlan->end()) {
            (*currentBehavior)->tick();
            ++currentBehavior;
        }
        else {
            currentPlan.release();
        }

        m->sleepExecution();
    }

    m->notifyShutdownCompletion();
}
