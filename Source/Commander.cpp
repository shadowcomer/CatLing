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

std::unique_ptr<bt::BehaviorTree> Commander::buildGatherMinerals() {
    Slab* workers;
    if (!m_allocator->find("workers", &workers)){
        std::cout << "Couldn't find the 'workers' slab." << std::endl;
        return nullptr;
    }

    std::unique_ptr<bt::Behavior> sendRetrieveMineralsB =
        std::make_unique<bt::ActionBehavior>(
        nullptr,
        [](bt::Behavior* b) { std::cout << "In TAllGatherMinerals" <<
        std::endl; },
        std::make_unique<TaskWrapper>(
        std::make_unique<TAllGatherMinerals>(workers)));

    std::unique_ptr<bt::Behavior> getWorkersB =
        std::make_unique<bt::ActionBehavior>(
        nullptr,
        [](bt::Behavior* b) { std::cout << "In TRetrieveWorkers" <<
        std::endl; },
        std::make_unique<TaskWrapper>(
        std::make_unique<TRetrieveWorkers>(workers)));

    std::vector<bt::Behavior*> childrenBehaviors{ getWorkersB.get(),
        sendRetrieveMineralsB.get() };

    std::unique_ptr<bt::Behavior> seq = std::make_unique<bt::Sequence>
        (nullptr,
        [](bt::Behavior* b) { std::cout << "In Sequence" <<
        std::endl; },
        childrenBehaviors);

    getWorkersB->setParent(seq.get());
    sendRetrieveMineralsB->setParent(seq.get());

    bt::BehaviorList behaviors;
    behaviors.push_back(std::move(seq));
    behaviors.push_back(std::move(getWorkersB));
    behaviors.push_back(std::move(sendRetrieveMineralsB));

    return std::move(
        std::make_unique<bt::BehaviorTree>(std::move(behaviors)));
}

void Commander::run(Commander* m)
{
    // Create Slab for worker units
    TypeList types;
    std::shared_ptr<SlabTypes::UnitType> unitType;
    types.insert(std::make_pair("unit", unitType.get()));
    m->m_allocator->createSlab("workers", types);

    Unitset units = Broodwar->self()->getUnits();
    for (auto u : units)
    {
        if (u->getType().isResourceDepot())
        {
            m->m_command = u;
            break;
        }
    }

    m->setFrameExecDelta(3);

    while (!m->isShuttingDown())
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

            BWAPI::UnitType type = u->getType();
            if (type.isWorker())
            {
                if (u->isIdle())
                {
                    Unit closestPatch = u->getClosestUnit(Filter::GetType == UnitTypes::Resource_Mineral_Field);
                    m->m_tasker.requestTask(new TGather(u, closestPatch, false));
                }
            }
        }

        m->sleepExecution();
    }

    m->notifyShutdownCompletion();
}

