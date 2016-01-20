#include "Behavior.h"

using namespace bt;

Behavior::Behavior(Behavior * parent, BehaviorMonitor monitor) :
m_parentBehavior(parent),
m_monitor(monitor) {

}

Behavior::~Behavior() {

}

void Behavior::setParent(Behavior* newParent) {
    m_parentBehavior = newParent;
}

void Behavior::setMonitor(BehaviorMonitor newMonitor) {
    m_monitor = newMonitor;
}

State Behavior::currentState() const {
    return m_currentState;
}

