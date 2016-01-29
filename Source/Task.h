/*
    This file contains the Task abstract class and specific
    implementations of it.

    A Task is a piece of code to be executed, accompanied by its
    execution parameters.

    This allows for deferred execution of a Task.

    Tasks are scheduled for execution inside a TaskManager's queue.
*/

#ifndef TASK_H
#define TASK_H

#include <BWAPI.h>

class Task
{
public:
    /*
    Execution function to be implemented by each specific task separately.
    */
    virtual void execute() = 0;
private:

};




class TGather : public Task
{
public:
    /*
    Task.
    Sends 'unit' to gather 'target'.
    shouldQueue - If true, this action is queued after any other
    already executing actions on the client side of Starcraft's process.
    */
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
    /*
    Task.
    Trains 'unit' from 'builder'.
    */
    TTrain(BWAPI::Unit builder, BWAPI::UnitType unit);
    void execute();

    const BWAPI::Unit builder;
    const BWAPI::UnitType unit;

private:
    

};




class TBuild : public Task
{
public:
    /*
    Task.
    Sends 'builder' to build 'building' at 'location'.
    */
    TBuild(BWAPI::Unit builder, BWAPI::UnitType building, BWAPI::TilePosition location);
    void execute();

    const BWAPI::Unit builder;
    const BWAPI::UnitType building;
    const BWAPI::TilePosition location;

private:
    /*
    Verifies whether the given 'builder' is a valid builder for the 'building'.
    */
    bool verifyBuildCapability();

};




class TAttack : public Task
{
public:
    /*
    Task.
    Sends unit 'origin' to attack 'target'.
    shouldQueue - If true, this action is queued after any other
    already executing actions on the client side of Starcraft's process.
    */
    TAttack(BWAPI::Unit origin, BWAPI::PositionOrUnit target, bool shouldQueue);
    void execute();

    const BWAPI::Unit origin;
    const BWAPI::PositionOrUnit target;
    const bool queueCommand;

private:
    
};




class TWildcard : public Task
{
public:
    TWildcard(std::function<void(void)> action) :
        m_action(action) {}

    void execute() override {
        m_action();
    }

    std::function<void(void)> m_action;
};

#endif