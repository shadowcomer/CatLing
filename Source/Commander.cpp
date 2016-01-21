#include "Commander.h"

/*
BT Test headers
*/
#include "BehaviorTree.h"
#include "ActionBehavior.h" // At the moment, include each 1 by 1
/*
BT Test headers
*/

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
/*
BehaviorTree implementation test setup
*/
    using namespace bt;

    std::cout << "Building tree..." << std::endl;
    // Create behaviors
    auto simpleMonitor =
        [](Behavior* b) -> void
    { std::cout << "M-Simple" << std::endl; };

    auto simpleTask =
        []() -> void
    { std::cout << "T-Simple" << std::endl; };

    Action simpleAction = std::make_unique<TWildcard>(simpleTask);
    std::unique_ptr<Behavior> simple =
        std::make_unique<ActionBehavior>(nullptr,
        simpleMonitor,
        std::move(simpleAction));

    // Insert into list
    BehaviorList behaviors;
    behaviors.push_back(std::move(simple));

    // Put tree together
    BehaviorTree tree(std::move(behaviors));
    std::cout << "Tree built." << std::endl;
/*
End BT test setup
*/
    std::cout << "Iterating tree..." << std::endl;
    for (auto b : tree) {
        std::cout << "++Begin iteration." << std::endl;
        b->tick();
        std::cout << "--End iteration." << std::endl;
    }
    std::cout << "Tree iterated." << std::endl;

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

        m->terminateThisExecution();
    }
}

