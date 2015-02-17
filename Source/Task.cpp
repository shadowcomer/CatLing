#include "Task.h"

using namespace BWAPI;

TTrain::TTrain(Unit builder, UnitType unit) :
m_builder(builder),
m_unit(unit){}

void TTrain::execute()
{
	m_builder->train(m_unit);
}

TBuild::TBuild(Unit builder, UnitType building, TilePosition location) :
m_builder(builder),
m_building(building),
m_location(location){}

void TBuild::execute()
{
	m_builder->build(m_building, m_location);
}



TAttack::TAttack(Unit origin, PositionOrUnit target, bool shouldQueue) :
m_origin(origin),
m_target(target),
m_queueCommand(shouldQueue){}

void TAttack::execute()
{
	m_origin->attack(m_target, m_queueCommand);
}

