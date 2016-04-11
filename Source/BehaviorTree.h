#ifndef BEHAVIORTREE_H
#define BEHAVIORTREE_H

#include <vector>
#include <memory>
#include <functional>

#include "Behavior.h"

namespace bt {

    class BehaviorTree;

    typedef std::vector<std::unique_ptr<Behavior>> BehaviorList;

/*********
BehaviorTree
**********/
/*
A BehaviorTree is a high-level container which manages both the
lifetime and the execution of the Behaviors it's constructed with.

BehaviorTrees are ranged for compliant.
*/
    class BehaviorTree {
    public:
        class BTIterator;
/**
The BehaviorTree constructor acquires ownership of the BehaviorList
given. This list is moved into the BehaviorTree, and thus the given
list becomes invalidated.

For this BehaviorTree to be valid, the root must be the first
element of the list.
*/
        BehaviorTree(BehaviorList&& behaviors);
        BehaviorTree(BehaviorTree const & original);

/*
Returns an iterator to the root of the BehaviorTree.
*/
        BTIterator begin();

/*
Returns an iterator to the end of the BehaviorTree.
*/
        BTIterator end();

        /*********
        BTIterator
        **********/

        /*
        The BTIterator is an iterator implementation for a BehaviorTree.
        This iterator begins at the root and ends at an asociated BTIterator
        to the BehaviorTree.

        The end iterator is always a nullptr.
        The consistency of the BehaviorTree, iterated with the BTIterator,
        is dependent on the correct implementation of a Behavior's
        'nextBehavior' virtual method, which is wrapped into BTIterator's
        '++' operator.
        */
        class BTIterator {
        public:
            BTIterator(BehaviorTree const & origin,
                Behavior * currentBehavior);
            BTIterator();
            ~BTIterator();

            Behavior* operator*();
            BTIterator& operator++();
            bool operator!=(BTIterator const & other);
            BTIterator& BTIterator::operator=(BTIterator other);

        private:
            Behavior * m_currentBehavior;
            BehaviorTree const & m_owner;

        }; // END BTIterator

    private:
        BehaviorList m_behaviors;

    }; // END BehaviorTree
}

#endif