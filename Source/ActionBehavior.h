#ifndef ACTIONBEHAVIOR_H
#define ACTIONBEHAVIOR_H

#include <memory>

#include "Behavior.h"
#include "Task.h"

namespace bt {

    typedef std::unique_ptr<Task> Action;

/*
An ActionBehavior is a Behavior which is a terminal node, or leaf,
of a BehaviorTree. As such, it always terminates with a success.

Thus, execution control differs from traditional BehaviorTrees in
that branch selection does not depend on whether the Behavior was
executed correctly or not. Execution logic is separated from the
tree itself, leaving successes and failures for the evaluation of
the correctness of BehaviorTree.

In this manner, a BehaviorTree is more akin to a computer program
than it is to an utility for a computer programmer to create better
AI.
*/
    class ActionBehavior : public Behavior {
    public:
        ActionBehavior(Behavior * parent,
            BehaviorMonitor monitor,
            Action&& action);
        ActionBehavior(ActionBehavior const & other);

/**
Change the Task managed by this ActionBehavior. The old Task, if
any, is deleted.
*/
        void setAction(Action&& newAction);

        Behavior * nextBehavior() override;
        void tick() override;

    private:
        std::unique_ptr<Task> m_action;

        void executeAction();
    };

}
#endif