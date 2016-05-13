#ifndef IINJECTOR_H
#define IINJECTOR_H

#include <memory>

class IInjector
{
public:
    virtual void updateResources() = 0;
};

typedef std::shared_ptr<IInjector> Injector_p;

#endif
