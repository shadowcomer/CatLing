#include "Commander.h"

#include <assert.h>

using namespace BWAPI;

Commander::Commander(Tasker& tsk, std::shared_ptr<Module>* modules,
    SlabAllocator* alloc) :
Module(tsk, modules, alloc),
m_resources(std::make_unique<ResourceManager>())
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

ResourceManager* Commander::getResourceManager() {
    return m_resources.get();
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

void Commander::updateBudget() {
    m_resources->updateResources();
}

int Commander::availableMinerals() {
    Slab* res = nullptr;
    bool found = m_allocator->find("resources", &res);
    assert(found);

    Entry e;
    res->getEntry(ModuleType::_END, e);
    return e[0]->toInt()->value;
}

int Commander::availableGas() {
    Slab* res = nullptr;
    bool found = m_allocator->find("resources", &res);
    assert(found);

    Entry e;
    res->getEntry(ModuleType::_END, e);
    return e[1]->toInt()->value;
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
        m->sleepExecution();
    }

    m->notifyShutdownCompletion();
}

