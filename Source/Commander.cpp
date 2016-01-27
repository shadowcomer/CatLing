#include "Commander.h"

/*
BT Test headers
*/
#include "Measure.h"

#include "BehaviorTree.h"
#include "ActionBehavior.h" // At the moment, include each 1 by 1
#include "Sequence.h"
/*
BT Test headers
*/

/*
BT test
*/
void testTree() {
    /*
BehaviorTree implementation test setup
*/
    using namespace bt;

    std::cout << "Building tree..." << std::endl;
    // Create behaviors
    //
    // ActionBehavior
    // First layer simples
    //
    const int LAYER_SIZE = 10000;

    BehaviorMonitor f_monitors[LAYER_SIZE];
    for (size_t i = 0; i < LAYER_SIZE; i++) {
        f_monitors[i] = [i](Behavior* b) -> void
        {};
    }

    std::function<void(void)> f_tasks[LAYER_SIZE];
    for (size_t i = 0; i < LAYER_SIZE; i++) {
        f_tasks[i] = [i]() -> void
        {};
    }

    Action f_actions[LAYER_SIZE];
    for (size_t i = 0; i < LAYER_SIZE; i++) {
        f_actions[i] = std::make_unique<TWildcard>(f_tasks[i]);
    }

    std::unique_ptr<Behavior> f_simples[LAYER_SIZE];
    for (size_t i = 0; i < LAYER_SIZE; i++) {
        f_simples[i] = std::make_unique<ActionBehavior>(nullptr,
            f_monitors[i],
            std::move(f_actions[i]));
    }

    //
    // Second layer simples
    //
    BehaviorMonitor s_monitors[LAYER_SIZE];
    for (size_t i = 0; i < LAYER_SIZE; i++) {
        s_monitors[i] = [i](Behavior* b) -> void
        {};
    }

    std::function<void(void)> s_tasks[LAYER_SIZE];
    for (size_t i = 0; i < LAYER_SIZE; i++) {
        s_tasks[i] = [i]() -> void
        {};
    }

    Action s_actions[LAYER_SIZE];
    for (size_t i = 0; i < LAYER_SIZE; i++) {
        s_actions[i] = std::make_unique<TWildcard>(s_tasks[i]);
    }

    std::unique_ptr<Behavior> s_simples[LAYER_SIZE];
    for (size_t i = 0; i < LAYER_SIZE; i++) {
        s_simples[i] = std::make_unique<ActionBehavior>(nullptr,
            s_monitors[i],
            std::move(s_actions[i]));
    }

    //
    // Second layer Sequence
    //
    BehaviorMonitor s_sequenceMonitor =
        [](Behavior* b) -> void
    {};

    std::function<void(void)> s_sequenceTask =
        []() -> void
    {};

    std::vector<Behavior*> s_sequenceBehaviors;
    for (size_t i = 0; i < LAYER_SIZE; i++) {
        s_sequenceBehaviors.push_back(s_simples[i].get());
    }

    std::unique_ptr<Behavior> s_sequence =
        std::make_unique<Sequence>(nullptr,
        s_sequenceMonitor,
        s_sequenceBehaviors);

    // Reconfigure the children to point to the sequence
    for (size_t i = 0; i < LAYER_SIZE; i++) {
        s_simples[i]->setParent(s_sequence.get());
    }

    //
    // First layer Sequence
    //
    BehaviorMonitor f_sequenceMonitor =
        [](Behavior* b) -> void
    {};

    std::function<void(void)> f_sequenceTask =
        []() -> void
    {};

    std::vector<Behavior*> f_sequenceBehaviors;
    for (size_t i = 0; i < LAYER_SIZE - 2; i++) {
        f_sequenceBehaviors.push_back(f_simples[i].get());
    }
    f_sequenceBehaviors.push_back(s_sequence.get());
    for (size_t i = LAYER_SIZE - 2; i < LAYER_SIZE; i++) {
        f_sequenceBehaviors.push_back(f_simples[i].get());
    }


    std::unique_ptr<Behavior> f_sequence =
        std::make_unique<Sequence>(nullptr,
        f_sequenceMonitor,
        f_sequenceBehaviors);

    // Reconfigure the children to point to the sequence
    for (size_t i = 0; i < LAYER_SIZE; i++) {
        f_simples[i]->setParent(f_sequence.get());
    }
    s_sequence->setParent(f_sequence.get());

    // Insert into list
    BehaviorList behaviors;
    behaviors.push_back(std::move(f_sequence));
    behaviors.push_back(std::move(s_sequence));
    for (size_t i = 0; i < LAYER_SIZE; i++) {
        behaviors.push_back(std::move(s_simples[i]));
    }
    for (size_t i = 0; i < LAYER_SIZE; i++) {
        behaviors.push_back(std::move(f_simples[i]));
    }

    // Put tree together
    BehaviorTree tree(std::move(behaviors));
    std::cout << "Tree built." << std::endl;

    /*
    End BT test setup
    */

    /*
    Begin BT iteration test
    */
    std::cout << "Ticking..." << std::endl;
    for (auto b : tree) {
        b->tick();
    }
    std::cout << std::endl << "Ticked." << std::endl;
    /*
    End BT iteration test
    */

/*
BT test end
*/

}

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
    std::cout << "Tree test duration: " <<
        Measure<>::execution(testTree) <<
        std::endl;

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

