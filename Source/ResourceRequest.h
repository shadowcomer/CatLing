#ifndef RESOURCEREQUEST_H
#define RESOURCEREQUEST_H

#include <vector>
#include "BWAPI\UnitType.h"

typedef std::pair<BWAPI::UnitType, unsigned int> UnitResource;

class ResourceRequest
{
public:
    ResourceRequest();
    ~ResourceRequest();

    unsigned int minerals;
    unsigned int gas;
    std::vector<UnitResource> units;
};

#endif
