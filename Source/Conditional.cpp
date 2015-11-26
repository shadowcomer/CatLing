#include "Conditional.h"

using namespace bt;

Conditional::Conditional(Behavior * parent,
    BehaviorMonitor monitor,
    Behavior * optionalBehavior,
    BehaviorCondition condition) :
Behavior(parent, monitor) {
    
}

Conditional::Conditional(Conditional const & other) :
Behavior(other) {

}

Behavior * Conditional::nextBehavior() {
    return nullptr;
}

void Conditional::tick() {

}

