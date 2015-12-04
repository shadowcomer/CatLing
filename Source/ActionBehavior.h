#ifndef ACTIONBEHAVIOR_H
#define ACTIONBEHAVIOR_H

#include <memory>

#include "Behavior.h"
#include "Task.h"

namespace bt {

    typedef std::unique_ptr<Task> Action;

    class ActionBehavior : public Behavior {
    public:
        ActionBehavior(Behavior * parent,
            BehaviorMonitor monitor,
            Action&& action);
        ActionBehavior(ActionBehavior const & other);

        Behavior * nextBehavior() override;
        void tick() override;

    private:
        std::unique_ptr<Task> m_action;

        void executeAction();
    };

}
#endif