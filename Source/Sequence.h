#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "Behavior.h"

#include <vector>

namespace BT {

    typedef std::shared_ptr<Behavior> Child;
    typedef std::vector<Child>::iterator Children;

    class Sequence : public Behavior
    {
    public:

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

        /**
        Notifies this Sequence of the given State change.
        */
        virtual void notify(Parent const &who, State newState);

    protected:
        std::vector<Child> m_children;

    };
};

#endif