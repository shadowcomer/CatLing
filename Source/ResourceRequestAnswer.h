#ifndef RESOURCEREQUESTANSWER_H
#define RESOURCEREQUESTANSWER_H

#include <vector>
#include "ResourceRequest.h"

class ResourceRequestAnswer
{
public:
    ResourceRequestAnswer(ResourceRequest original);
    ~ResourceRequestAnswer();

    ResourceRequest originalRequest;

    bool conceded;

    unsigned int missingMinerals;
    unsigned int missingGas;

    std::vector<UnitResource> missingUnits;
};

#endif
