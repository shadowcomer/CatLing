#ifndef MACROPLANNER_H
#define MACROPLANNER_H

#include "IPlanner.h"

#include "SlabAllocator.h"
#include "IResources.h"

#include "Module.h"

class MacroPlanner :
    public IPlanner {
public:
    MacroPlanner(SlabAllocator* alloc, Resources_p resources);
    ~MacroPlanner();

    virtual std::unique_ptr<bt::BehaviorTree> getNextComplexAction();

private:
    SlabAllocator* m_allocator;
    Resources_p m_resourceSource;

    std::unique_ptr<bt::BehaviorTree> planBarracks();

};

#endif

