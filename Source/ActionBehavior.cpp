#include "ActionBehavior.h"

#include <exception>
#include <assert.h>

using namespace bt;

ActionBehavior::ActionBehavior(Behavior * parent,
            BehaviorMonitor monitor,
            Action&& action) :
Behavior(parent, monitor),
m_action(std::move(action)) {

}

ActionBehavior::ActionBehavior(ActionBehavior const & other) :
Behavior(other) {
    throw new std::exception("Copy constructor not implemented.");
}

void ActionBehavior::setAction(Action&& newAction) {
    m_action = std::move(newAction);
}

Behavior * ActionBehavior::nextBehavior() {
    switch (m_currentState){
        case State::INVALID: // Cascade
        case State::RUNNING:
            return this;
        case State::ABORTED: // Cascade
        case State::FAILURE: // Cascade
        case State::SUCCESS:
            return !m_parentBehavior ? nullptr :
                m_parentBehavior->nextBehavior();
        default:
            throw new std::exception("Unexpected State.");
    }
}

void ActionBehavior::tick() {
    // Pre-tick monitorization
    m_monitor(this);
    executeAction();
}

void ActionBehavior::executeAction() {
    // TODO: At the moment, execute does not return a termination
    // result, and thus we can't assess whether it was a success
    // or a failure. This has to be changed in the Task
    // implementation, and then here.
    m_action->execute();
    m_currentState = State::SUCCESS;
}