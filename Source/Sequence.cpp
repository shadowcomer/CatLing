#include "Sequence.h"

using namespace bt;

Sequence::Sequence(Behavior * parent,
    BehaviorMonitor monitor,
    std::vector<Behavior *> behaviors):
Behavior(parent, monitor) {

}

Sequence::Sequence(Sequence const & other):
Behavior(other) {

}

void Sequence::setBehaviors(std::vector<Behavior *> newBehaviors) {
    m_behaviors = newBehaviors;
}

Behavior * Sequence::nextBehavior() {
    return nullptr;
}

void Sequence::tick() {

}

