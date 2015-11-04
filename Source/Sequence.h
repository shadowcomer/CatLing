#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "Composite.h"

namespace BT {
    class Sequence : public Composite
    {
    public:
        Sequence();
        ~Sequence();

    protected:
        virtual State doIterate();

    private:
        unsigned int m_currentChildOffset;

        /**
        Implementation of what to do when the Behavior
        implementation has been notified through 'notifyParent'.
        */
        virtual void treatNotification(Behavior const * const who,
            State oldState);
    };
};

#endif