#include "Loop.h"

using namespace bt;

Loop::Loop(Behavior * parent,
    BehaviorMonitor monitor,
    Behavior * repeatedBehavior,
    int repetitions) :
Behavior(parent, monitor),
m_maxRepetitions(repetitions),
m_currentRepetitions(0) {

}

Loop::Loop(Loop const & other) :
Behavior(other) {

}

void Loop::setRepeatedBehavior(Behavior* newBehavior) {
    m_repeatedBehavior = newBehavior;
}

void Loop::setRepetitions(int newMax) {
    m_maxRepetitions = newMax;
}

Behavior * Loop::nextBehavior() {
    return nullptr;
}

void Loop::tick() {

}
