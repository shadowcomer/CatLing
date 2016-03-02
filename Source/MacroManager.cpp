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

std::unique_ptr<bt::BehaviorTree> MacroManager::buildBarracksTree(){
    Slab* workers = nullptr;

    if (!m_allocator->find("workers", &workers)) {
        std::cout << "Couldn't find the 'workers' slab." << std::endl;
        return nullptr;
    }

    UnitTypeFun buildingFun = []() -> BWAPI::UnitType {
        return BWAPI::UnitTypes::Terran_Barracks;
    };

    UnitFun builderFun = [workers]() -> BWAPI::Unit {
        Entry workerE;
        bool acquired = workers->getAndRemoveEntry(0, workerE);

        return acquired ? workerE[0]->toUnit()->value :
            nullptr;
    };

    TilePositionFun locationFun =
        [](void) -> TilePosition {
        return Broodwar->getBuildLocation(UnitTypes::Terran_Barracks,
            Broodwar->self()->getStartLocation(),
            100);
    };

    std::unique_ptr<bt::Behavior> buildBarracksB =
        std::make_unique<bt::ActionBehavior>(
        nullptr,
        [](bt::Behavior* b) { std::cout << "In TBuildBarracks" <<
        std::endl; },
        std::make_unique<TaskWrapper>(
        std::make_unique<TBuild>(builderFun,
                                 buildingFun,
                                 locationFun)));

    std::vector<bt::Behavior*> childrenBehaviors{
        buildBarracksB.get()
    };

    std::unique_ptr<bt::Behavior> seq = std::make_unique<bt::Sequence>
        (nullptr,
        [](bt::Behavior* b) { std::cout << "In Sequence" <<
        std::endl; },
        childrenBehaviors);

    buildBarracksB->setParent(seq.get());

    bt::BehaviorList behaviors;
    behaviors.push_back(std::move(seq));
    behaviors.push_back(std::move(buildBarracksB));

    return std::move(
        std::make_unique<bt::BehaviorTree>(std::move(behaviors)));
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

    bool modified = false;
    modified = res->modifyEntry(ModuleType::MACROMGR, 0, newMinerals.get());
    assert(modified);
    modified = res->modifyEntry(ModuleType::MACROMGR, 1, newGas.get());
    assert(modified);
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
        if(m->canBuildBarracks())
        {
            m->commitResources(UnitTypes::Terran_Barracks);
            std::unique_ptr<bt::BehaviorTree> buildBarracks =
                m->buildBarracksTree();

            for (auto b : *buildBarracks) {
                b->tick();
            }
        }

        m->sleepExecution();
    }

    m->notifyShutdownCompletion();
}
