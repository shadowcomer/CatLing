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

Behavior * Sequence::nextBehavior() {
    return nullptr;
}

void Sequence::tick() {

}

