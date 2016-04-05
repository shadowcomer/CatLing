#include "MacroManager.h"

#include <assert.h>

using namespace BWAPI;

MacroManager::MacroManager(Tasker& tsk) :
Module(tsk),
m_planner(std::make_unique<MacroPlanner>(m_allocator))
{
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

void MacroManager::commitResources(BWAPI::UnitType unit) {
    Slab* res = nullptr;
    bool found = m_allocator->find("resources", &res);
    assert(found);

    int mPrice = unit.mineralPrice();
    int gPrice = unit.gasPrice();

    Entry e;
    res->getEntry(ModuleType::MACROMGR, e);

    int availableMinerals = e[0]->toInt()->value;
    int availableGas = e[1]->toInt()->value;

    assert(availableMinerals >= mPrice &&
        availableGas >= gPrice);

    std::unique_ptr<SlabTypes::IntType> newMinerals =
        std::make_unique<SlabTypes::IntType>(
        availableMinerals - mPrice);

    std::unique_ptr<SlabTypes::IntType> newGas =
        std::make_unique<SlabTypes::IntType>(
        availableGas - gPrice);

    res->modifyEntry(ModuleType::MACROMGR, 0, newMinerals.get());
    res->modifyEntry(ModuleType::MACROMGR, 1, newGas.get());
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

    while(!m->isShuttingDown())
    {

        m->sleepExecution();
    }

    m->notifyShutdownCompletion();
}
