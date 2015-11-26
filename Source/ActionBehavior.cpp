#include "ActionBehavior.h"

using namespace bt;

ActionBehavior::ActionBehavior(Behavior * parent,
            BehaviorMonitor monitor,
            std::unique_ptr<Task>&& action) :
Behavior(parent, monitor) {

}

ActionBehavior::ActionBehavior(ActionBehavior const & other) :
Behavior(other) {

}

Behavior * ActionBehavior::nextBehavior() {
    return nullptr;
}

void ActionBehavior::tick() {

}

void ActionBehavior::executeAction() {

}