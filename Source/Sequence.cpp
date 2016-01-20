#include "Sequence.h"

#include <assert.h>

using namespace bt;

Sequence::Sequence(Behavior * parent,
    BehaviorMonitor monitor,
    std::vector<Behavior *> behaviors) :
    Behavior(parent, monitor),
    m_currentBehavior(0) {

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
    switch (m_currentState) {
    case State::RUNNING:
        if (hasNextBehaviorChild() &&
            behaviorSucceeded(m_currentBehavior)) {
            return
                m_behaviors[++m_currentBehavior]->nextBehavior();
        }
        else {
            // We're leaving the sequence, so we must update the
            // state.
            m_currentState =
                m_behaviors[m_currentBehavior]->currentState();

            return m_parentBehavior ?
                m_parentBehavior->nextBehavior() :
                nullptr;
        }

    case State::INVALID:
        m_currentState = State::RUNNING;
        assert(0 == m_currentBehavior);
        return m_behaviors[m_currentBehavior]->nextBehavior();

        /*
        A Sequence should never be ticked, thus it should never
        be able to encounter the following states.
        */
    case State::FAILURE: // Cascade
    case State::ABORTED: // Cascade
    case State::SUCCESS: // Cascade
    default:
        throw new std::exception("Unexpected state.");
    }

}

void Sequence::tick() {

}

