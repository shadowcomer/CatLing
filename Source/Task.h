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

#include <functional>

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
    TaskWrapper(std::unique_ptr<CloneableTask> t);
    static void InitializeTaskWrapper(Tasker* tasker);
private:
    std::unique_ptr<CloneableTask> m_task;
    static Tasker * sm_tasker;

    void execute() override;
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
    TBuild(Slab* storage, BWAPI::UnitType building,
         std::function<BWAPI::TilePosition(void)> locationFun);
    void execute();
    virtual Task* clone() const;

    const BWAPI::UnitType building;
    const std::function<BWAPI::TilePosition(void)> m_locationFun;

private:
    /*
    Verifies whether the given 'builder' is a valid builder for the 'building'.
    */
    bool verifyBuildCapability();

    /*
    Obtain an available worker for construction.
    */
    BWAPI::Unit getConstructionWorker();

    Slab* m_storage;
    BWAPI::Unit m_builder;
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




class TSelectBuilder : public CloneableTask
{
public:
    TSelectBuilder(Slab* workers, Slab* builders);

    void execute() override;
    virtual Task* clone() const;

private:
    Slab* m_workers;
    Slab* m_builders;
};

#endif