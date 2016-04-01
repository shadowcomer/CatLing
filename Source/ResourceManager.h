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
    ResourceRequestAnswer requestResources(ResourceRequest res);

    /* 
        Updates virtual resources to be consistent with real
        resources.

        Must only be called during onFrame.
    */
    void updateResources();

private:

};

#endif
