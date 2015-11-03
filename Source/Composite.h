/**
    A Composite is an abstract class that inherits from Behavior.
    Expanding on a Behavior, the purpose of a Composite is to
    give the ability to have multiple Behaviors related to it.

    Composites support adding new children to the hierarchy, and
    iteration over them.
*/

#ifndef COMPOSITE_H
#define COMPOSITE_H

#include "Behavior.h"

#include <vector>

namespace BT {

    typedef std::shared_ptr<Behavior> Child;
    typedef std::vector<Child>::iterator Children;

    class Composite : public Behavior
    {
    public:

        Composite();
        ~Composite();

        /**
        Adds a new Child to the end of the list of Children.

        newChild: The Behavior to add to the list of Children.
        If it's a nullptr, then no action is taken.
        */
        virtual void addChild(Child const & newChild);

        /**
        Retrieves an iterator to the first Child.
        */
        Children children();

        /**
        Retrieves an iterator to the end of the list of Children.
        */
        Children children_end();

    protected:
        std::vector<Child> m_children;

        virtual State doIterate() = 0;

    private:

        /**
        Implementation of what to do when the Behavior
        implementation has been notified through 'notifyParent'.
        */
        virtual void treatNotification(Behavior const * const who,
            State oldState) = 0;

    };
};

#endif