#include "ActionBehavior.h"

#include <exception>

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

Behavior * ActionBehavior::nextBehavior() {
    return nullptr;
}

void ActionBehavior::tick() {

}

void ActionBehavior::executeAction() {

}