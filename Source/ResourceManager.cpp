#include "ResourceManager.h"

#include "BWAPI.h"

ResourceManager::ResourceManager() {

}

ResourceManager::~ResourceManager() {

}

ResourceRequestAnswer ResourceManager::requestResources(
    ResourceRequest const res) {
    ResourceRequestAnswer answer{res};

    // Start as always conceded, so we can easily negate it when it
    // fails in any of the requested resources
    answer.conceded = true;

    // Concede minerals
    if (m_virtualMinerals < res.minerals) {
        answer.missingMinerals = res.minerals - m_virtualMinerals;
        answer.conceded = false;
    }

    // Concede gas
    if (m_virtualGas < res.minerals) {
        answer.missingGas = res.gas - m_virtualGas;
        answer.conceded = false;
    }

    // Concede units
    // Dummy: check nothing, concede nothing, accept request.

    // Commit resources
    if (answer.conceded) {
        m_virtualMinerals -= res.minerals;
        m_virtualGas -= res.gas;
    }

    return answer;
}

void ResourceManager::updateResources() {
    unsigned int mineralsAccum = BWAPI::Broodwar->self()->
        gatheredMinerals();
    unsigned int gasAccum = BWAPI::Broodwar->self()->
        gatheredGas();

    // *Accum is always larger than or equal to real*Accum
    unsigned int newMinerals = mineralsAccum - m_realMineralsAccum;
    unsigned int newGas = gasAccum - m_realGasAccum;

    m_realMineralsAccum += newMinerals;
    m_realGasAccum += newGas;

    m_virtualMinerals += newMinerals;
    m_virtualGas += newGas;
}

