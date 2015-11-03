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
        Notifies this Composite of the given State change.
        */
        virtual void notify(Parent const &who, State newState) = 0;

    protected:
        std::vector<Child> m_children;

        virtual State doIterate() = 0;

    };
};

#endif