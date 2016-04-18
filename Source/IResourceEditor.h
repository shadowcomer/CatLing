#ifndef IRESOURCEEDITOR_H
#define IRESOURCEEDITOR_H

#include "ResourceCommon.h"

#include "ModuleType.h"

class IResourceEditor
{
public:
    virtual bool assignUnits(ConstUnitList units, ModuleType newOwner) = 0;
    virtual bool assignMinerals(unsigned int amount, ModuleType module) = 0;
    virtual bool assignGas(unsigned int amount, ModuleType module) = 0;

    virtual ConstUnitList allUnits() = 0;
    virtual unsigned int totalMinerals() = 0;
    virtual unsigned int totalGas() = 0;
};

#endif