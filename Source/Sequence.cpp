#include "Sequence.h"

#include <assert.h>

using namespace bt;

Sequence::Sequence(Behavior * parent,
    BehaviorMonitor monitor,
    std::vector<Behavior *> behaviors) :
    Behavior(parent, monitor),
    m_currentBehavior(0),
    m_behaviors(behaviors) {

}

Sequence::Sequence(Sequence const & other) :
Behavior(other) {

}

void Sequence::setBehaviors(std::vector<Behavior *> newBehaviors) {
    m_behaviors = newBehaviors;
}

bool Sequence::behaviorSucceeded(int behavior) {
    return m_behaviors[behavior]->currentState() == State::SUCCESS;
}

bool Sequence::hasNextBehaviorChild() {
    return m_currentBehavior < (m_behaviors.size() - 1);
}

Behavior * Sequence::nextBehavior() {
/*
A Sequence should never be queried for the next behavior by the
iterator when it's in an invalid state.
*/

    assert(State::INVALID == m_currentState);
    switch (m_currentState) {
    case State::RUNNING:
        return hasNextBehaviorChild() &&
            behaviorSucceeded(m_currentBehavior) ?
            m_behaviors[m_currentBehavior + 1] :
            m_parentBehavior;

    case State::SUCCESS: // Cascade
    case State::FAILURE: // Cascade
    case State::ABORTED: // Cascade. TODO: Abort should have special
                         // treatment.
        return m_parentBehavior;

    default:
        throw new std::exception("Unexpected state.");
    }

}

void Sequence::tick() {
    throw new std::exception("Unexpected tick.");
}

