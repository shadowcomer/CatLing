#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "Behavior.h"

#include <vector>

namespace bt {
    class Sequence : public Behavior {
    public:
        Sequence(Behavior * parent,
            BehaviorMonitor monitor,
            std::vector<Behavior *> behaviors);

        Sequence(Sequence const & other);

        Behavior * nextBehavior() override;
        void tick() override;

/**
Set the list of behaviors.
*/
        void setBehaviors(std::vector<Behavior *> newBehaviors);

    private:
        std::vector<Behavior *> m_behaviors;
        int m_currentBehavior;

/**
Checks whether the given behavior succeeded.
*/
        bool behaviorSucceeded(int behavior);

/**
Checks whether there's another behavior after the current one
from the list of children behaviors.
*/
        bool hasNextBehaviorChild();

    };
}

#endif