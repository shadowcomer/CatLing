#include "KnowledgeBase.h"

#include "BWAPI.h"

/*
Overrides
*/

/*************************
IInjector overrides
*************************/

void KnowledgeBase::updateResources() {
    unsigned int mineralsAccum = BWAPI::Broodwar->self()->
        gatheredMinerals();
    unsigned int gasAccum = BWAPI::Broodwar->self()->
        gatheredGas();

    // *Accum is always larger than or equal to real*Accum
    unsigned int newMinerals = mineralsAccum - m_realMineralsAccum;
    unsigned int newGas = gasAccum - m_realGasAccum;

    m_realMineralsAccum += newMinerals;
    m_realGasAccum += newGas;

    {
        tbb::mutex::scoped_lock lock(m_resourceLock);
        m_freeMinerals += newMinerals;
        m_freeGas += newGas;
    }
}


/*************************
IResourceEditor overrides
**************************/

bool KnowledgeBase::assignUnits(
    ConstUnitList units, ModuleType newOwner) {

    return false;
}

bool KnowledgeBase::assignMinerals(
    unsigned int amount, ModuleType module) {
        {
            tbb::mutex::scoped_lock lock(m_resourceLock);

            if (m_freeMinerals < amount) {
                return false;
            }

            m_freeMinerals -= amount;
        }
        return true;
}

bool KnowledgeBase::assignGas(
    unsigned int amount, ModuleType module) {
        {
            tbb::mutex::scoped_lock lock(m_resourceLock);

            if (m_freeGas < amount) {
                return false;
            }

            m_freeGas -= amount;
        }

    return true;
}

ConstUnitList KnowledgeBase::allUnits() {
    return ConstUnitList{ m_units.begin(), m_units.end() };
}

unsigned int KnowledgeBase::totalMinerals() {
    return m_realMineralsAccum;
}

unsigned int KnowledgeBase::totalGas() {
    return m_realGasAccum;
}

/*************************
IResourceViewer overrides
*************************/

/*
End overrides
*/
