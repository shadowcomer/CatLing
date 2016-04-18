#include "ResourceManager.h"

#include "BWAPI.h"

ResourceManager::ResourceManager() {

}

ResourceManager::~ResourceManager() {

}

ResourceRequestAnswer ResourceManager::requestResources(
    ResourceRequest const res) {
    ResourceRequestAnswer answer{ res };

    {
        tbb::mutex::scoped_lock lock(m_resourceLock);

        // Start as always conceded, so we can easily negate it when it
        // fails in any of the requested resources
        answer.conceded = true;

        // Concede minerals
        if (m_virtualMinerals < res.minerals) {
            answer.missingMinerals =
                res.minerals - m_virtualMinerals;
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
    }

    return answer;
}

