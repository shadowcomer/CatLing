#include "Task.h"

#include <assert.h>

using namespace BWAPI;

Tasker* TaskWrapper::sm_tasker = nullptr;

void TaskWrapper::InitializeTaskWrapper(Tasker* tasker) {
    assert(nullptr != tasker);
    sm_tasker = tasker;
}

TaskWrapper::TaskWrapper(std::unique_ptr<CloneableTask> t) :
m_task(std::move(t)) {
    assert(nullptr != m_task);
}

bool TaskWrapper::execute() {
    assert(nullptr != sm_tasker);
    return sm_tasker->requestTask(m_task->clone());
}

TGather::TGather(UnitFun gatherer, UnitFun resource,
    DecisionFun queue) :
getGatherer(gatherer),
getResource(resource),
checkQueue(queue){}

bool TGather::execute()
{
    Unit gatherer = getGatherer();
    Unit resource = getResource();
    bool success = false;

    if (gatherer) {
        success = gatherer->gather(resource, checkQueue());
    }

    return success;
}

Task* TGather::clone() const {
    return new TGather(*this);
}




TTrain::TTrain(UnitFun builder, UnitTypeFun unit) :
getBuilder(builder),
getUnitType(unit){}

bool TTrain::execute()
{
    Unit builder = getBuilder();
    UnitType unit = getUnitType();
    bool success = false;

    if (builder) {
        success = builder->train(unit);
    }

    return success;
}

Task* TTrain::clone() const {
    return new TTrain(*this);
}




TBuild::TBuild(UnitFun builder, UnitTypeFun building,
    TilePositionFun location) :
getBuilder(builder),
getBuildingType(building),
getLocation(location) {

}

bool TBuild::execute()
{
    BWAPI::Unit builder = getBuilder();
    BWAPI::UnitType building = getBuildingType();
    BWAPI::TilePosition location = getLocation();
    bool success = false;

    if (builder) {
        success = builder->build(building, location);
    }

    return success;
}

bool TBuild::verifyBuildCapability()
{
    return true; // Inhibit check for build capability
}

Task* TBuild::clone() const {
    return new TBuild(*this);
}





TAttack::TAttack(UnitFun attacker, PositionOrUnitFun target,
    DecisionFun queue) :
getAttacker(attacker),
getTarget(target),
checkQueue(queue){}

bool TAttack::execute()
{
    Unit attacker = getAttacker();
    PositionOrUnit target = getTarget();
    bool success = false;

    if (attacker) {
        success = attacker->attack(target, checkQueue());
    }

    return success;
}

Task* TAttack::clone() const {
    return new TAttack(*this);
}




TRetrieveWorkers::TRetrieveWorkers(Slab* storage) :
m_storage(storage) {
    assert(nullptr != m_storage);
}

bool TRetrieveWorkers::execute() {
    BWAPI::Unitset units = Broodwar->self()->getUnits();
    for (Unit u : units) {
        if (u->getType() == UnitTypes::Terran_SCV) {
            m_storage->appendEntry(
                Entry{ new SlabTypes::UnitType(u) });
        }
    }

    return true;
}

Task* TRetrieveWorkers::clone() const {
    return new TRetrieveWorkers(*this);
}




TAllGatherMinerals::TAllGatherMinerals(UnitVecFun gatherers,
    OnUnitFun resource) :
getGatherers(gatherers),
getResource(resource) {

}

bool TAllGatherMinerals::execute() {
    std::vector<Unit> gatherers = getGatherers();
    Unit patch = nullptr;

    for (auto g : gatherers) {
        if (!g) {
            continue;
        }

        patch = getResource(g);
        if (!patch) {
            continue;
        }

        g->gather(patch);
    }

    return true;
}

Task* TAllGatherMinerals::clone() const {
    return new TAllGatherMinerals(*this);
}




TSelectBuilder::TSelectBuilder(Slab* workers, Slab* builders) :
m_workers(workers),
m_builders(builders) {
    assert(nullptr != m_workers);
    assert(nullptr != m_builders);
}

bool TSelectBuilder::execute() {
    std::vector<Entry> workers = m_workers->getEntries();
    std::vector<Entry> builders = m_builders->getEntries();
    bool acquired = false;

    // Use any builders we might already have
    if (!builders.empty()) {
        return true;
    }

    // Any workers available?
    if (workers.empty()) {
        return false;
    }

    // Find an adequate worker
    for (Entry e : workers) {
        Unit w = e[0]->toUnit()->value;

        if (!w->isConstructing() && !w->isGatheringGas()) {
            acquired = true;
            m_builders->appendEntry(e);
            break;
        }
    }

    return acquired;
}

Task* TSelectBuilder::clone() const {
    return new TSelectBuilder(*this);
}




TRequestResources::TRequestResources(
    ResourceRequest rq, ResourceManager* mgr) :
    m_request(rq),
    m_manager(mgr) {
    assert(nullptr != m_manager);
}

bool TRequestResources::execute() {
    ResourceRequestAnswer answer = 
        m_manager->requestResources(m_request);

    return answer.conceded;
}
