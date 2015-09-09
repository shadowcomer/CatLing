#include "Task.h"

using namespace BWAPI;

TGather::TGather(Unit unit, Unit target, bool shouldQueue) :
unit(unit),
target(target),
queueCommand(shouldQueue){}

void TGather::execute()
{
    unit->gather(target, queueCommand);
}




TTrain::TTrain(Unit builder, UnitType unit) :
builder(builder),
unit(unit){}

void TTrain::execute()
{
    builder->train(unit);
}




TBuild::TBuild(Unit builder, UnitType building, TilePosition location) :
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

