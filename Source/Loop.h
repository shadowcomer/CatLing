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

/**
Set the repeated behavior.
*/
        void setRepeatedBehavior(Behavior* newBehavior);

/**
Set the max number of repetitions.
*/
        void setRepetitions(int newMax);

        Behavior * nextBehavior() override;
        void tick() override;

    private:
        Behavior * m_repeatedBehavior;
        int m_maxRepetitions;
        int m_currentRepetitions;
    };
}

#endif