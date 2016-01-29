#include "Conditional.h"

#include <exception>
#include <assert.h>

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

void Conditional::setOptionalBehavior(Behavior* newBehavior) {
    m_optionalBehavior = newBehavior;
}

Behavior * Conditional::nextBehavior() {
    Behavior * nextBehavior = nullptr;
    switch (m_currentState){
    case State::INVALID:
        nextBehavior = m_optionalBehavior;
        m_currentState = State::RUNNING;
    case State::RUNNING:
        nextBehavior = m_parentBehavior;
        m_currentState = State::SUCCESS;
    default:
        assert(false); // Reached unexpected state
    }

    return nextBehavior;
}

void Conditional::tick() {

}

