#ifndef KNOWLEDGEBASE_H
#define KNOWLEDGEBASE_H

#include "IInjector.h"
#include "IResourceEditor.h"
#include "IResourceViewer.h"

#include "../include/TBB/tbb/mutex.h"

class KnowledgeBase : 
    public IInjector, public IResourceEditor,
    public IResourceViewer
{
public:
    /*
    Overrides
    */

    /*************************
    IInjector overrides
    *************************/

    /*
    Updates virtual resources to be consistent with real
    resources.

    Must only be called during onFrame.
    */
    void updateResources() override;

    /*************************
    IResourceEditor overrides
    **************************/
    bool assignUnits(ConstUnitList units, ModuleType newOwner) override;
    bool assignMinerals(unsigned int amount, ModuleType module) override;
    bool assignGas(unsigned int amount, ModuleType module) override;

    ConstUnitList allUnits() override;
    unsigned int totalMinerals() override;
    unsigned int totalGas() override;

    /*************************
    IResourceViewer overrides
    *************************/
    ConstUnitList availableUnits() override;
    unsigned int availableMinerals() override;
    unsigned int availableGas() override;

    unsigned int assignedMinerals(ModuleType module) override;
    unsigned int assignedGas(ModuleType module) override;

    /*
    End overrides
    */

private:
    tbb::mutex m_resourceLock;

    // real*Accum represent internal knowledge of the current
    // *Accum, thus giving the ability to compare if we've
    // harvested new resources or not.
    unsigned int m_realMineralsAccum;
    unsigned int m_realGasAccum;

    // virtual* represent the non-asigned resources that this
    // resourceManager has.
    unsigned int m_freeMinerals;
    unsigned int m_freeGas;
};

#endif
