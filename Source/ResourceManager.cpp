#include "ResourceManager.h"

#include "BWAPI.h"

ResourceManager::ResourceManager() {

}

ResourceManager::~ResourceManager() {

}

ResourceRequestAnswer ResourceManager::requestResources(
    ResourceRequest const res) {
    ResourceRequestAnswer answer;



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

