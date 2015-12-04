#include "Conditional.h"

#include <exception>

using namespace bt;

Conditional::Conditional(Behavior * parent,
    BehaviorMonitor monitor,
    Behavior * optionalBehavior,
    BehaviorCondition condition) :
Behavior(parent, monitor),
m_optionalBehavior(optionalBehavior),
m_condition(condition) {

}

Conditional::Conditional(Conditional const & other) :
Behavior(other) {
    throw new std::exception("Copy constructor not implemented.");
}

Behavior * Conditional::nextBehavior() {
    return nullptr;
}

void Conditional::tick() {

}

