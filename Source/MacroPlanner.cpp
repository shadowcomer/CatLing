#include "MacroPlanner.h"

#include "BehaviorTree.h"
#include "Sequence.h"
#include "ActionBehavior.h"

#include <BWAPI.h>

#include <assert.h>

using namespace BWAPI;

MacroPlanner::MacroPlanner(SlabAllocator* alloc,
    Resources_p resources) :
    m_allocator(alloc) {
    assert(nullptr != resources);
    m_resourceSource = resources;
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
    Slab* builders = nullptr;

    if (!m_allocator->find("workers", &workers)) {
        std::cout << "Couldn't find the 'workers' slab." <<
            std::endl;
        return nullptr;
    }

    if (!m_allocator->find("builders", &builders)) {
        std::cout << "Couldn't find the 'builders' slab." <<
            std::endl;
        return nullptr;
    }

    //
    // Builder Selection
    //
    std::unique_ptr<bt::Behavior> selectBuilderB =
        std::make_unique<bt::ActionBehavior>(
        nullptr,
        [](bt::Behavior* b) {},
        std::make_unique<TaskWrapper>(
        std::make_unique<TSelectBuilder>(
        workers, builders)));


    //
    // Resource Request
    //

    // WARNING: Does not ask for required unit. That is not
    // implemented yet.
    ResourceRequest requiredResources;
    requiredResources.minerals =
        BWAPI::UnitTypes::Terran_Barracks.mineralPrice();
    requiredResources.gas =
        BWAPI::UnitTypes::Terran_Barracks.gasPrice();

    std::unique_ptr<bt::Behavior> resourcesB =
        std::make_unique<bt::ActionBehavior>(
        nullptr,
        [](bt::Behavior* b) {},
        std::make_unique<TRequestResources>(
        requiredResources,
        m_resourceSource));

    //
    // Building Construction
    //

    UnitTypeFun buildingFun = []() -> BWAPI::UnitType {
        return BWAPI::UnitTypes::Terran_Barracks;
    };

    UnitFun builderFun = [builders]() -> BWAPI::Unit {
        Entry builderE;
        bool acquired = builders->getAndRemoveEntry(0, builderE);

        return acquired ? builderE[0]->toUnit()->value :
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

    //
    // Sequence: Select -> Request -> Build
    //

    std::vector<bt::Behavior*> childrenBehaviors{
        selectBuilderB.get(),
        resourcesB.get(),
        buildBarracksB.get() };

    std::unique_ptr<bt::Behavior> seq = std::make_unique<bt::Sequence>
        (nullptr,
        [](bt::Behavior* b) {},
        childrenBehaviors);

    selectBuilderB->setParent(seq.get());
    resourcesB->setParent(seq.get());
    buildBarracksB->setParent(seq.get());

    //
    // Tree
    //

    bt::BehaviorList behaviors;
    behaviors.push_back(std::move(seq));
    behaviors.push_back(std::move(selectBuilderB));
    behaviors.push_back(std::move(resourcesB));
    behaviors.push_back(std::move(buildBarracksB));

    return std::move(
        std::make_unique<bt::BehaviorTree>(std::move(behaviors)));
}

