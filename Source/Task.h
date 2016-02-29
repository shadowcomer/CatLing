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
#include <vector>

class Tasker;

typedef std::function<BWAPI::Unit(void)> UnitFun;
typedef std::function<BWAPI::Unit(BWAPI::Unit)> OnUnitFun;
typedef std::function<std::vector<BWAPI::Unit>(void)> UnitVecFun;
typedef std::function<BWAPI::UnitType(void)> UnitTypeFun;
typedef std::function<BWAPI::TilePosition(void)> TilePositionFun;
typedef std::function<BWAPI::PositionOrUnit(void)> PositionOrUnitFun;

typedef std::function<bool(void)> DecisionFun;

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
    Sends 'gatherer' to gather 'resource'.
    queue - If true, this action is queued after any other
    already executing actions on the client side of Starcraft's process.
    */
    TGather(UnitFun gatherer, UnitFun resource,
        DecisionFun queue);
    void execute();
    virtual Task* clone() const;

private:
    UnitFun getGatherer;
    UnitFun getResource;
    DecisionFun checkQueue;

};




class TTrain : public CloneableTask
{
public:
    /*
    Task.
    Trains 'unit' from 'builder'.
    */
    TTrain(UnitFun builder, UnitTypeFun unit);
    void execute();
    virtual Task* clone() const;

private:
    UnitFun getBuilder;
    UnitTypeFun getUnitType;

};




class TBuild : public CloneableTask
{
public:
    /*
    Task.
    Sends 'builder' to build 'building' at 'location'.
    */
    TBuild(UnitFun builder, UnitTypeFun building,
        TilePositionFun location);
    void execute();
    virtual Task* clone() const;

private:
    /*
    Verifies whether the given 'builder' is a valid builder for the 'building'.
    */
    bool verifyBuildCapability();

    UnitFun getBuilder;
    UnitTypeFun getBuildingType;
    TilePositionFun getLocation;
};




class TAttack : public CloneableTask
{
public:
    /*
    Task.
    Sends unit 'attacker' to attack 'target'.
    queue - If true, this action is queued after any other
    already executing actions on the client side of Starcraft's process.
    */
    TAttack(UnitFun attacker, PositionOrUnitFun target,
        DecisionFun queue);
    void execute();
    virtual Task* clone() const;

private:
    UnitFun getAttacker;
    PositionOrUnitFun getTarget;
    DecisionFun checkQueue;
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
    TAllGatherMinerals(UnitVecFun gatherers, OnUnitFun resource);

    void execute() override;
    virtual Task* clone() const;

private:
    UnitVecFun getGatherers;
    OnUnitFun getResource;
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