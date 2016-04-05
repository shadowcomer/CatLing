#ifndef MACROPLANNER_H
#define MACROPLANNER_H

#include "IPlanner.h"

#include "SlabAllocator.h"

class MacroPlanner :
    public IPlanner {
public:
    MacroPlanner(SlabAllocator* alloc);
    ~MacroPlanner();

    virtual std::unique_ptr<bt::BehaviorTree> getNextComplexAction();

private:
    SlabAllocator* m_allocator;

    std::unique_ptr<bt::BehaviorTree> planBarracks();

};

#endif

