#include "Task.h"

#include <assert.h>

using namespace BWAPI;

TGather::TGather(Unit unit, Unit target, bool shouldQueue) :
unit(unit),
target(target),
queueCommand(shouldQueue){}

void TGather::execute()
{
    unit->gather(target, queueCommand);
}




TTrain::TTrain(Unit builder, BWAPI::UnitType unit) :
builder(builder),
unit(unit){}

void TTrain::execute()
{
    builder->train(unit);
}




TBuild::TBuild(Unit builder, BWAPI::UnitType building, TilePosition location) :
builder(builder),
building(building),
location(location){}

void TBuild::execute()
{
    //TODO: Use building verification
    builder->build(building, location);
}

bool TBuild::verifyBuildCapability()
{
    return (nullptr != builder) && (building.whatBuilds().first == builder->getType());
}




TAttack::TAttack(Unit origin, PositionOrUnit target, bool shouldQueue) :
origin(origin),
target(target),
queueCommand(shouldQueue){}

void TAttack::execute()
{
    origin->attack(target, queueCommand);
}




TRetrieveWorkers::TRetrieveWorkers(Slab* storage) :
m_storage(storage) {
    assert(nullptr != m_storage);
}

void TRetrieveWorkers::execute() {
    BWAPI::Unitset units = Broodwar->self()->getUnits();
    for (Unit u : units) {
        if (u->getType() == UnitTypes::Terran_SCV) {
            Entry e;
            SlabTypes::UnitType worker(u);
            e.push_back(&worker);
            m_storage->appendEntry(e);
        }
    }

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
