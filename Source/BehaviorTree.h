#ifndef BEHAVIORTREE_H
#define BEHAVIORTREE_H

#include <vector>
#include <memory>
#include <functional>

#include "Behavior.h"

namespace bt {
    class BehaviorTree;

    typedef std::function<void(BehaviorTree const &)> TreeMonitor;
    typedef std::vector<std::unique_ptr<Behavior>> BehaviorList;

    class BehaviorTree {
    private: // This is to keep the interface at the top
        class BTIterator;
    public:
        BehaviorTree(BehaviorList&& behaviors);
        BehaviorTree(BehaviorTree const & original);

        BTIterator begin();
        BTIterator end();

    private:
        BehaviorList m_behaviors;

        class BTIterator {
        public:
            BTIterator(BehaviorTree const & origin,
                Behavior * currentBehavior);
            ~BTIterator();

            Behavior* operator*();
            BTIterator& operator++();
            bool operator!=(BTIterator const & other);

        private:
            Behavior * m_currentBehavior;
            BehaviorTree const & m_owner;

        }; // END BTIterator
    }; // END BehaviorTree
}

#endif