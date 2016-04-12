#ifndef MACROPLANNER_H
#define MACROPLANNER_H

#include "IPlanner.h"

#include "SlabAllocator.h"

#include "Module.h"

class MacroPlanner :
    public IPlanner {
public:
    MacroPlanner(SlabAllocator* alloc, ResourceManager* resources);
    ~MacroPlanner();

    virtual std::unique_ptr<bt::BehaviorTree> getNextComplexAction();

private:
    SlabAllocator* m_allocator;
    ResourceManager* m_resourceSource;

    std::unique_ptr<bt::BehaviorTree> planBarracks();

};

#endif

