#ifndef IRESOURCES_H
#define IRESOURCES_H

#include <memory>

#include "ResourceRequestAnswer.h"
#include "ResourceRequest.h"

class IResources {
public:
    virtual ResourceRequestAnswer requestResources(
        ResourceRequest const res) = 0;
};

typedef std::shared_ptr<IResources> Resources_p;

#endif
