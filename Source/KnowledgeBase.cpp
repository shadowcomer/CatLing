#include "KnowledgeBase.h"

#include "BWAPI.h"

KnowledgeBase::KnowledgeBase():
    m_resourceLock(tbb::mutex()),
    m_realMineralsAccum(0),
    m_realGasAccum(0),
    m_freeMinerals(m_realMineralsAccum),
    m_freeGas(m_realGasAccum),
    m_units(UnitList()) {

}

KnowledgeBase::~KnowledgeBase() {

}

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
IResourceViewer overrides
*************************/

ConstUnitList KnowledgeBase::availableUnits() {
    // TODO: This should return the really available units, instead
    // of every unit.
    return ConstUnitList{ m_units.begin(), m_units.end() };
}

unsigned int KnowledgeBase::availableMinerals() {
    return m_freeMinerals;
}

unsigned int KnowledgeBase::availableGas() {
    return m_freeGas;
}

unsigned int KnowledgeBase::assignedMinerals(ModuleType module) {
    return 0; // TODO: Implement
}

unsigned int KnowledgeBase::assignedGas(ModuleType module) {
    return 0; // TODO: Implement
}

/*************************
IResources overrides
*************************/

ResourceRequestAnswer KnowledgeBase::requestResources(
    ResourceRequest const res) {
    ResourceRequestAnswer answer{ res };

    {
        tbb::mutex::scoped_lock lock(m_resourceLock);

        // Start as always conceded, so we can easily negate it when it
        // fails in any of the requested resources
        answer.conceded = true;

        // Concede minerals
        if (m_freeMinerals < res.minerals) {
            answer.missingMinerals =
                res.minerals - m_freeMinerals;
            answer.conceded = false;
        }

        // Concede gas
        if (m_freeGas < res.minerals) {
            answer.missingGas = res.gas - m_freeGas;
            answer.conceded = false;
        }

        // Concede units
        // Dummy: check nothing, concede everything, accept request.

        // Commit resources
        if (answer.conceded) {
            m_freeMinerals -= res.minerals;
            m_freeGas -= res.gas;
        }
    }

    return answer;
}

/*
End overrides
*/
