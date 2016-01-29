#ifndef SELECTOR_H
#define SELECTOR_H

#include "Behavior.h"
#include <vector>

namespace bt {
    class Selector : public Behavior {
    public:
        Selector(Behavior * parent,
            BehaviorMonitor monitor,
            std::vector<Behavior *> behaviors);

        Selector(Selector const & other);

/**
Set the list of behaviors.
*/
        void setBehaviors(std::vector<Behavior *> newBehaviors);

        Behavior * nextBehavior() override;
        void tick() override;

    private:
        std::vector<Behavior *> m_behaviors;
        int m_currentBehavior;
    };
}

#endif