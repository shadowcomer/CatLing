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

#include "Slab.h"
#include "Tasker.h"

class Tasker;

class Task
{
public:
    /*
    Execution function to be implemented by each specific task separately.
    */
    virtual void execute() = 0;

private:

};

class CloneableTask : public Task
{
public:
    virtual Task* clone() const = 0;
};

class TaskWrapper : public Task
{
public:
    TaskWrapper(CloneableTask* t);

private:
    CloneableTask * m_task;
    static Tasker * sm_tasker;

    void execute() override;

    static void InitializeTaskWrapper(Tasker* tasker);
};




class TGather : public CloneableTask
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
    virtual Task* clone() const;

    const BWAPI::Unit unit;
    const BWAPI::Unit target;
    const bool queueCommand;
private:

};




class TTrain : public CloneableTask
{
public:
    /*
    Task.
    Trains 'unit' from 'builder'.
    */
    TTrain(BWAPI::Unit builder, BWAPI::UnitType unit);
    void execute();
    virtual Task* clone() const;

    const BWAPI::Unit builder;
    const BWAPI::UnitType unit;

private:
    

};




class TBuild : public CloneableTask
{
public:
    /*
    Task.
    Sends 'builder' to build 'building' at 'location'.
    */
    TBuild(BWAPI::Unit builder, BWAPI::UnitType building, BWAPI::TilePosition location);
    void execute();
    virtual Task* clone() const;

    const BWAPI::Unit builder;
    const BWAPI::UnitType building;
    const BWAPI::TilePosition location;

private:
    /*
    Verifies whether the given 'builder' is a valid builder for the 'building'.
    */
    bool verifyBuildCapability();

};




class TAttack : public CloneableTask
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
    virtual Task* clone() const;

    const BWAPI::Unit origin;
    const BWAPI::PositionOrUnit target;
    const bool queueCommand;

private:
    
};




class TWildcard : public CloneableTask
{
public:
    TWildcard(std::function<void(void)> action) :
        m_action(action) {}

    void execute() override {
        m_action();
    }

    virtual Task* clone() const {
        return new ::TWildcard(*this);
    }

    std::function<void(void)> m_action;
};




class TRetrieveWorkers : public CloneableTask
{
public:
    TRetrieveWorkers(Slab* storage);

    void execute() override;
    virtual Task* clone() const;

private:
    Slab* m_storage;
};




class TAllGatherMinerals : public CloneableTask
{
public:
    TAllGatherMinerals(Slab* storage);

    void execute() override;
    virtual Task* clone() const;

private:
    Slab* m_storage;
};

#endif