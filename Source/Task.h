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

private:
	BWAPI::Unit m_unit;
	BWAPI::Unit m_target;
	bool m_queueCommand;

};




class TTrain : public Task
{
public:
	TTrain(BWAPI::Unit builder, BWAPI::UnitType unit);
	void execute();

private:
	BWAPI::Unit m_builder;
	BWAPI::UnitType m_unit;

};




class TBuild : public Task
{
public:
	TBuild(BWAPI::Unit builder, BWAPI::UnitType building, BWAPI::TilePosition location);
	void execute();

private:
	BWAPI::Unit m_builder;
	BWAPI::UnitType m_building;
	BWAPI::TilePosition m_location;

	bool verifyBuildCapability();

};




class TAttack : public Task
{
public:
	TAttack(BWAPI::Unit origin, BWAPI::PositionOrUnit target, bool shouldQueue);
	void execute();

private:
	BWAPI::Unit m_origin;
	BWAPI::PositionOrUnit m_target;
	bool m_queueCommand;
};

#endif