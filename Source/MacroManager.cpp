#include "MacroManager.h"

#include <assert.h>

using namespace BWAPI;

MacroManager::MacroManager(Tasker& tsk) :
Module(tsk)
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
        // When we have plenty of minerals, build a barracks
        if(Broodwar->self()->minerals() > 200)
        {
            // Find a builder
            Unitset set = Broodwar->getUnitsInRadius(
                (Position)Broodwar->self()->getStartLocation() / 32,
                20000,
                Filter::GetType == UnitTypes::Terran_SCV &&
                Filter::IsIdle || Filter::IsGatheringMinerals);

            if (!set.empty())
            {
                Unit builder = *set.begin();
                TilePosition location = 
                    Broodwar->getBuildLocation(UnitTypes::Terran_Barracks,
                    Broodwar->self()->getStartLocation(),
                    100);
                m->tasker().requestTask(
                    new TBuild(builderSlab,
                        UnitTypes::Terran_Barracks,
                        location));
            }
        }

        m->sleepExecution();
    }

    m->notifyShutdownCompletion();
}
