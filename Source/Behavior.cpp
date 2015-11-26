#include "Behavior.h"

using namespace bt;

Behavior::Behavior(Behavior * parent, BehaviorMonitor monitor) :
m_parentBehavior(parent) {

}

Behavior::~Behavior() {

}

State Behavior::currentState() const {
    return m_currentState;
}

