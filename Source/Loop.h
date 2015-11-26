#ifndef LOOP_H
#define LOOP_H

#include "Behavior.h"

namespace bt {
    class Loop : public Behavior {
    public:
        Loop(Behavior * parent,
            BehaviorMonitor monitor,
            Behavior * repeatedBehavior,
            int repetitions);

        Loop(Loop const & other);

        Behavior * nextBehavior() override;
        void tick() override;

    private:
        Behavior * m_repeatedBehavior;
        int m_maxRepetitions;
        int m_currentRepetitions;
    };
}

#endif