#ifndef KNOWLEDGEBASE_H
#define KNOWLEDGEBASE_H

#include "IInjector.h"
#include "IResourceEditor.h"
#include "IResourceViewer.h"

class KnowledgeBase : 
    public IInjector, public IResourceEditor,
    public IResourceViewer
{
public:
    /*
    Overrides
    */

    /*
    IInjector overrides
    */
    void updateResources() override;

    /*
    IResourceEditor overrides
    */
    bool assignUnits(ConstUnitList units, ModuleType newOwner) override;
    bool assignMinerals(unsigned int amount, ModuleType module) override;
    bool assignGas(unsigned int amount, ModuleType module) override;

    ConstUnitList allUnits() override;
    unsigned int totalMinerals() override;
    unsigned int totalGas() override;

    /*
    IResourceViewer overrides
    */
    ConstUnitList availableUnits() override;
    unsigned int availableMinerals() override;
    unsigned int availableGas() override;

    unsigned int assignedMinerals(ModuleType module) override;
    unsigned int assignedGas(ModuleType module) override;

    /*
    End overrides
    */

private:

};

#endif
