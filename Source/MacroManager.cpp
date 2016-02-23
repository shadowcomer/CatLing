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
    Slab* builders = nullptr;
    if (!m_allocator->find("workers", &workers)) {
        std::cout << "Couldn't find the 'workers' slab." << std::endl;
        return nullptr;
    }

    if (!m_allocator->find("builders", &builders)) {
        std::cout << "Couldn't find the 'builders' slab." << std::endl;
        return nullptr;
    }

    std::unique_ptr<bt::Behavior> selectBuilderB =
        std::make_unique<bt::ActionBehavior>(
        nullptr,
        [](bt::Behavior* b) { std::cout << "In TSelectBuilder" <<
        std::endl; },
        std::make_unique<TaskWrapper>(
        std::make_unique<TSelectBuilder>(workers, builders)));

    std::function<TilePosition(void)> locationFun =
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
        std::make_unique<TBuild>(builders,
                                 UnitTypes::Terran_Barracks,
                                 locationFun)));

    std::vector<bt::Behavior*> childrenBehaviors{
        selectBuilderB.get(),
        buildBarracksB.get()
    };

    std::unique_ptr<bt::Behavior> seq = std::make_unique<bt::Sequence>
        (nullptr,
        [](bt::Behavior* b) { std::cout << "In Sequence" <<
        std::endl; },
        childrenBehaviors);

    selectBuilderB->setParent(seq.get());
    buildBarracksB->setParent(seq.get());

    bt::BehaviorList behaviors;
    behaviors.push_back(std::move(seq));
    behaviors.push_back(std::move(selectBuilderB));
    behaviors.push_back(std::move(buildBarracksB));

    return std::move(
        std::make_unique<bt::BehaviorTree>(std::move(behaviors)));
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
                std::function<TilePosition(void)> location =
                    [](void) -> TilePosition {
                    return Broodwar->getBuildLocation(
                        UnitTypes::Terran_Barracks,
                        Broodwar->self()->getStartLocation(),
                        100);
                };

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
