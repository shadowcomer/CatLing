#ifndef TASK_H
#define TASK_H

#include <BWAPI.h>

class Task
{
public:
	virtual void execute() = 0;
private:

};




class TGather : public Task
{
public:
	TGather(BWAPI::Unit unit, BWAPI::Unit target, bool shouldQueue);
	void execute();

	const BWAPI::Unit m_unit;
	const BWAPI::Unit m_target;
	const bool m_queueCommand;
private:

};




class TTrain : public Task
{
public:
	TTrain(BWAPI::Unit builder, BWAPI::UnitType unit);
	void execute();

	const BWAPI::Unit m_builder;
	const BWAPI::UnitType m_unit;

private:
	

};




class TBuild : public Task
{
public:
	TBuild(BWAPI::Unit builder, BWAPI::UnitType building, BWAPI::TilePosition location);
	void execute();

	const BWAPI::Unit m_builder;
	const BWAPI::UnitType m_building;
	const BWAPI::TilePosition m_location;

private:
	bool verifyBuildCapability();

};




class TAttack : public Task
{
public:
	TAttack(BWAPI::Unit origin, BWAPI::PositionOrUnit target, bool shouldQueue);
	void execute();

	const BWAPI::Unit m_origin;
	const BWAPI::PositionOrUnit m_target;
	const bool m_queueCommand;

private:
	
};

#endif