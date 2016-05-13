#ifndef IRESOURCEVIEWER_H
#define IRESOURCEVIEWER_H

#include <vector>
#include <memory>

#include "ResourceCommon.h"
#include "ModuleType.h"

class IResourceViewer
{
public:
    virtual ConstUnitList availableUnits() = 0;
    virtual unsigned int availableMinerals() = 0;
    virtual unsigned int availableGas() = 0;

    virtual unsigned int assignedMinerals(ModuleType module) = 0;
    virtual unsigned int assignedGas(ModuleType module) = 0;
};

typedef std::shared_ptr<IResourceViewer> ResViewer_p;

#endif
