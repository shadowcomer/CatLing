#ifndef CONDITIONAL_H
#define CONDITIONAL_H

#include "Behavior.h"

#include <functional>

namespace bt {
    typedef std::function<bool(void)> BehaviorCondition;

    class Conditional : public Behavior {
    public:
        Conditional(Behavior * parent,
            BehaviorMonitor monitor,
            Behavior * optionalBehavior,
            BehaviorCondition condition);

        Conditional(Conditional const & other);

        Behavior * nextBehavior() override;
        void tick() override;

    private:
        Behavior * m_optionalBehavior;
        BehaviorCondition m_condition;
        
    };
}

#endif