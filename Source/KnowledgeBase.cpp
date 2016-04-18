#include "KnowledgeBase.h"

#include "BWAPI.h"

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

