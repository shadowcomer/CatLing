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

	const BWAPI::Unit unit;
	const BWAPI::Unit target;
	const bool queueCommand;
private:

};




class TTrain : public Task
{
public:
	TTrain(BWAPI::Unit builder, BWAPI::UnitType unit);
	void execute();

	const BWAPI::Unit builder;
	const BWAPI::UnitType unit;

private:
	

};




class TBuild : public Task
{
public:
	TBuild(BWAPI::Unit builder, BWAPI::UnitType building, BWAPI::TilePosition location);
	void execute();

	const BWAPI::Unit builder;
	const BWAPI::UnitType building;
	const BWAPI::TilePosition location;

private:
	bool verifyBuildCapability();

};




class TAttack : public Task
{
public:
	TAttack(BWAPI::Unit origin, BWAPI::PositionOrUnit target, bool shouldQueue);
	void execute();

	const BWAPI::Unit origin;
	const BWAPI::PositionOrUnit target;
	const bool queueCommand;

private:
	
};

#endif