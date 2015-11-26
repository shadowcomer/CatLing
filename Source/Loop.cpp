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

Behavior * Loop::nextBehavior() {
    return nullptr;
}

void Loop::tick() {

}
