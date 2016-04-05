#include "MacroPlanner.h"

#include "BehaviorTree.h"
#include "Sequence.h"
#include "ActionBehavior.h"

#include <BWAPI.h>

#include <assert.h>

using namespace BWAPI;

MacroPlanner::MacroPlanner(SlabAllocator* alloc) :
    m_allocator(alloc) {
    assert(nullptr != m_allocator);
}

MacroPlanner::~MacroPlanner() {

}

std::unique_ptr<bt::BehaviorTree>
    MacroPlanner::getNextComplexAction() {
    std::unique_ptr<bt::BehaviorTree> plan;

    plan = planBarracks();

    return std::move(plan);
}

std::unique_ptr <bt::BehaviorTree> MacroPlanner::planBarracks() {
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

    bt::BehaviorList behaviors;
    behaviors.push_back(std::move(buildBarracksB));

    return std::move(
        std::make_unique<bt::BehaviorTree>(std::move(behaviors)));
}

