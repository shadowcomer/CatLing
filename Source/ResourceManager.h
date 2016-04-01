#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "ResourceRequest.h"
#include "ResourceRequestAnswer.h"

class ResourceManager {
public:
    ResourceManager();
    ~ResourceManager();

    /*
        Requests the resources contained in 'res' to the manager.
    */
    ResourceRequestAnswer requestResources(
        ResourceRequest const res);

    /*
        Updates virtual resources to be consistent with real
        resources.

        Must only be called during onFrame.
    */
    void updateResources();

private:
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
