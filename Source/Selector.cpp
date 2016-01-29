#include "Selector.h"

using namespace bt;

Selector::Selector(Behavior * parent,
    BehaviorMonitor monitor,
    std::vector<Behavior *> behaviors) :
Behavior(parent, monitor) {

}

Selector::Selector(Selector const & other) :
Behavior(other) {

}

void Selector::setBehaviors(std::vector<Behavior *> newBehaviors) {
    m_behaviors = newBehaviors;
}

Behavior * Selector::nextBehavior() {
    return nullptr;
}

void Selector::tick() {

}






