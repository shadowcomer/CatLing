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

    };
}

#endif