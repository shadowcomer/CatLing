#ifndef IPLANNER_H
#define IPLANNER_H

#include "BehaviorTree.h"

class IPlanner {
public:
    virtual std::unique_ptr<bt::BehaviorTree>
        getNextComplexAction() = 0;

};

#endif