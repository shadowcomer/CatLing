#ifndef MACROPLANNER_H
#define MACROPLANNER_H

#include "IPlanner.h"
class MacroPlanner :
    public IPlanner {
public:
    MacroPlanner();
    ~MacroPlanner();

    virtual std::unique_ptr<bt::BehaviorTree> getNextComplexAction();
};

#endif

