#include "Commander.h"

using namespace BWAPI;

Commander::Commander(Tasker& tsk) :
Module(tsk)
{

}

Commander::~Commander()
{

}

void Commander::launch()
{
    m_thread = tbb::tbb_thread(&Commander::run, this);
}

bool Commander::shutdownHelper()
{

    return true;
}

void Commander::run(Commander* m)
{
    Unitset units = Broodwar->self()->getUnits();
    for (auto u : units)
    {
        if (u->getType().isResourceDepot())
        {
            m->m_command = u;
            break;
        }
    }

    while (!m->isTerminating())
    {
        // While enough minerals and supply, train a worker.
        if (Broodwar->self()->minerals() >= 50 &&
            ((Broodwar->self()->supplyUsed() + UnitTypes::Terran_SCV.supplyRequired()) <
            Broodwar->self()->supplyTotal()))
        {
            m->m_tasker.requestTask(new TTrain(m->m_command, UnitTypes::Terran_SCV));
        }

        Unitset units = Broodwar->self()->getUnits();
        for (auto u : units)
        {
            if (!u->exists())
                continue;

            // Ignore the unit if it has one of the following status ailments
            if (u->isLockedDown() || u->isMaelstrommed() || u->isStasised())
                continue;

            // Ignore the unit if it is in one of the following states
            if (u->isLoaded() || !u->isPowered() || u->isStuck())
                continue;

            // Ignore the unit if it is incomplete or busy constructing
            if (!u->isCompleted() || u->isConstructing())
                continue;

            UnitType type = u->getType();
            if (type.isWorker())
            {
                if (u->isIdle())
                {
                    Unit closestPatch = u->getClosestUnit(Filter::GetType == UnitTypes::Resource_Mineral_Field);
                    m->m_tasker.requestTask(new TGather(u, closestPatch, false));
                }
            }
        }
        tbb::this_tbb_thread::sleep(tbb::tick_count::interval_t((double)0.25));
    }
}

