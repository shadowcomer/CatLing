#include "Commander.h"

#include <assert.h>

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

    UnitVecFun gathererFun = [workers]() -> std::vector<Unit> {
        auto entries = workers->getEntries();
        std::vector<BWAPI::Unit> gatherers;
        for (auto e : entries) {
            gatherers.push_back(e[0]->toUnit()->value);
        }
        return gatherers;
    };

    OnUnitFun resourceFun = [](Unit u) -> Unit {
        return u->getClosestUnit(
            Filter::GetType == UnitTypes::Resource_Mineral_Field);
    };

    std::unique_ptr<bt::Behavior> sendRetrieveMineralsB =
        std::make_unique<bt::ActionBehavior>(
        nullptr,
        [](bt::Behavior* b) { std::cout << "In TAllGatherMinerals" <<
        std::endl; },
        std::make_unique<TaskWrapper>(
        std::make_unique<TAllGatherMinerals>(gathererFun,
        resourceFun)));

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

void Commander::allocateInitialBudget() {
    Slab* resources = nullptr;
    bool found = m_allocator->find("resources", &resources);
    assert(found);

    // Insert resource allocations in order of module apparition
    // The last element of the slab corresponds to the virtual
    // image of unassigned resources.
    for (size_t i = 0; i <= ModuleType::_END; i++) {
        Entry e;
        e.push_back(new SlabTypes::IntType(0));
        e.push_back(new SlabTypes::IntType(0));
        resources->appendEntry(e);
    }

    std::function<void(void)> initFun = [resources, this]() -> void {
        // Create resources
        std::unique_ptr<SlabTypes::IntType> minerals =
            std::make_unique<SlabTypes::IntType>(0);
        std::unique_ptr<SlabTypes::IntType> gas =
            std::make_unique<SlabTypes::IntType>(0);

        // Get resources
        minerals->value = BWAPI::Broodwar->self()->minerals();
        gas->value = BWAPI::Broodwar->self()->gas();

        // Update slab
        resources->modifyEntry(ModuleType::_END, 0, minerals.get());
        resources->modifyEntry(ModuleType::_END, 1, gas.get());

        m_virtAccumMinerals = minerals->value;
        m_virtAccumGas = gas->value;
    };

    // Create and query a task for virtual resource initialization
    std::unique_ptr<bt::Behavior> initResourcesB =
        std::make_unique<bt::ActionBehavior>(
        nullptr,
        [](bt::Behavior* b) { std::cout << "In TWildCard: BudgetInit" <<
        std::endl; },
        std::make_unique<TaskWrapper>(
        std::make_unique<TWildcard>(initFun)));

    bt::BehaviorList behaviors;
    behaviors.push_back(std::move(initResourcesB));

    bt::BehaviorTree initTree(std::move(behaviors));

    for (auto b : initTree) {
        b->tick();
    }
}

void Commander::updateBudget() {
    std::unique_ptr<bt::Behavior> updateB =
        std::make_unique<bt::ActionBehavior>(
        nullptr,
        [](bt::Behavior* b) {},
        std::make_unique<TaskWrapper>(
        std::make_unique<TWildcard>(
        std::bind(&Commander::updateBudgetHelper, this))));

    bt::BehaviorList behaviors;
    behaviors.push_back(std::move(updateB));
    bt::BehaviorTree updateTree(std::move(behaviors));

    for (auto b : updateTree) {
        b->tick();
    }
}

void Commander::updateBudgetHelper() {
    Slab* resources = nullptr;
    bool found = m_allocator->find("resources", &resources);
    assert(found);

    // Get accumulated resources for comparison
    int realAccumMins = BWAPI::Broodwar->self()->gatheredMinerals();
    int realAccumGas = BWAPI::Broodwar->self()->gatheredGas();

    // Calculate the difference
    int newMinerals = realAccumMins - m_virtAccumMinerals;
    int newGas = realAccumGas - m_virtAccumGas;
    assert(newMinerals >= 0 && newGas >= 0);

    // Obtain the old virtual resources so we can update them
    Entry oldVirtual;
    resources->getEntry(ModuleType::_END, oldVirtual);
    oldVirtual[0]->toInt()->value += newMinerals;
    oldVirtual[1]->toInt()->value += newGas;

    m_virtAccumMinerals = realAccumMins;
    m_virtAccumGas = realAccumGas;
}

void Commander::run(Commander* m)
{
    m->allocateInitialBudget();

    Unitset units = Broodwar->self()->getUnits();
    for (auto u : units)
    {
        if (u->getType().isResourceDepot())
        {
            m->m_command = u;
            break;
        }
    }

    std::unique_ptr<bt::BehaviorTree> gather =
        m->buildGatherMinerals();

    std::cout << "Starting gather ticking" << std::endl;
    for (auto b : *gather.get()) {
        b->tick();
    }

    std::cout << "Completed gather ticking" << std::endl;

    m->setFrameExecDelta(3);

    while (!m->isShuttingDown())
    {
        m->updateBudget();
        m->sleepExecution();
    }

    m->notifyShutdownCompletion();
}

