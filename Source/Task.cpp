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

void TaskWrapper::execute() {
    assert(nullptr != sm_tasker);
    sm_tasker->requestTask(m_task->clone());
}

TGather::TGather(Unit unit, Unit target, bool shouldQueue) :
unit(unit),
target(target),
queueCommand(shouldQueue){}

void TGather::execute()
{
    unit->gather(target, queueCommand);
}

Task* TGather::clone() const {
    return new TGather(*this);
}




TTrain::TTrain(Unit builder, BWAPI::UnitType unit) :
builder(builder),
unit(unit){}

void TTrain::execute()
{
    builder->train(unit);
}

Task* TTrain::clone() const {
    return new TTrain(*this);
}




TBuild::TBuild(Slab* storage, BWAPI::UnitType building,
    std::function<BWAPI::TilePosition(void)> locationFun) :
m_storage(storage),
building(building),
m_locationFun(locationFun),
m_builder(nullptr) {
    assert(nullptr != m_storage);
}

void TBuild::execute()
{
    //TODO: Use building verification
    m_builder = getConstructionWorker();
    if (nullptr != m_builder) {
        m_builder->build(building, m_locationFun());
    }
}

bool TBuild::verifyBuildCapability()
{
    return (nullptr != m_builder) &&
        (building.whatBuilds().first == m_builder->getType());
}

Task* TBuild::clone() const {
    return new TBuild(*this);
}

BWAPI::Unit TBuild::getConstructionWorker() {
    auto allWorkers = m_storage->getEntries();
    if (allWorkers.empty()){
        return nullptr;
    }

    m_storage->removeEntry(0);

    return allWorkers[0][0]->toUnit()->value;
}




TAttack::TAttack(Unit origin, PositionOrUnit target, bool shouldQueue) :
origin(origin),
target(target),
queueCommand(shouldQueue){}

void TAttack::execute()
{
    origin->attack(target, queueCommand);
}

Task* TAttack::clone() const {
    return new TAttack(*this);
}




TRetrieveWorkers::TRetrieveWorkers(Slab* storage) :
m_storage(storage) {
    assert(nullptr != m_storage);
}

void TRetrieveWorkers::execute() {
    BWAPI::Unitset units = Broodwar->self()->getUnits();
    for (Unit u : units) {
        if (u->getType() == UnitTypes::Terran_SCV) {
            m_storage->appendEntry(
                Entry{ new SlabTypes::UnitType(u) });
        }
    }

}

Task* TRetrieveWorkers::clone() const {
    return new TRetrieveWorkers(*this);
}




TAllGatherMinerals::TAllGatherMinerals(Slab* storage) :
m_storage(storage) {
    assert(nullptr != m_storage);
}

void TAllGatherMinerals::execute() {
    std::vector<Entry> workers = m_storage->getEntries();
    for (Entry e : workers) {
        SlabTypes::UnitType* worker = e[0]->toUnit();
        assert(nullptr != worker);

        Unit closestPatch = worker->value->
            getClosestUnit(
            Filter::GetType == UnitTypes::Resource_Mineral_Field);

        worker->value->gather(closestPatch);
    }
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

void TSelectBuilder::execute() {
    std::vector<Entry> workers = m_workers->getEntries();
    if (workers.empty()) {
        return;
    }

    Entry selected = workers[0];
    m_workers->removeEntry(0);

    m_builders->appendEntry(selected);
}

Task* TSelectBuilder::clone() const {
    return new TSelectBuilder(*this);
}
