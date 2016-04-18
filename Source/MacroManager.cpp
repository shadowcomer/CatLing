#include "MacroManager.h"
#include "Commander.h"

#include <assert.h>

using namespace BWAPI;

MacroManager::MacroManager(Tasker& tsk, std::shared_ptr<Module>* modules,
    SlabAllocator* alloc) :
Module(tsk, modules, alloc) {
    ResourceManager* resManager =
        ((Commander*)modules[ModuleType::COMMANDER].get())->
        getResourceManager();
    m_planner = std::make_shared<MacroPlanner>(
        m_allocator, resManager);
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

bool MacroManager::canBuildBarracks() {
    Slab* res = nullptr;
    bool found = m_allocator->find("resources", &res);
    assert(found);

    Entry budget;
    res->getEntry(ModuleType::MACROMGR, budget);

    int availableMinerals = budget[0]->toInt()->value;

    return availableMinerals >= UnitTypes::Terran_Barracks;
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
