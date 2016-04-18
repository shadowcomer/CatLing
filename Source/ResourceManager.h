#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "ResourceRequest.h"
#include "ResourceRequestAnswer.h"

#include "../include/TBB/tbb/mutex.h"

class ResourceManager {
public:
    ResourceManager();
    ~ResourceManager();

    /*
        Requests the resources contained in 'res' to the manager.
    */
    ResourceRequestAnswer requestResources(
        ResourceRequest const res);

private:
    tbb::mutex m_resourceLock;

    // real*Accum represent internal knowledge of the current
    // *Accum, thus giving the ability to compare if we've
    // harvested new resources or not.
    unsigned int m_realMineralsAccum;
    unsigned int m_realGasAccum;

    // virtual* represent the non-asigned resources that this
    // resourceManager has.
    unsigned int m_virtualMinerals;
    unsigned int m_virtualGas;
};

#endif
