#include "BehaviorTree.h"

using namespace bt;

/************************
BTIterator
*************************/

BehaviorTree::BTIterator::BTIterator(BehaviorTree const & origin,
    Behavior * currentBehavior) :
m_currentBehavior(currentBehavior),
m_owner(origin) {

}

BehaviorTree::BTIterator::~BTIterator() {

}

Behavior * BehaviorTree::BTIterator::operator*() {
    return m_currentBehavior;
}

BehaviorTree::BTIterator& BehaviorTree::BTIterator::operator++() {

    return *this;
}

bool BehaviorTree::BTIterator::operator!=(BTIterator const & other) {
    return true;
}

/************************
BehaviorTree
*************************/

BehaviorTree::BehaviorTree(BehaviorList&& behaviors) {

}

BehaviorTree::BehaviorTree(BehaviorTree const & original) {

}

BehaviorTree::BTIterator BehaviorTree::begin() {
    return BTIterator(*this, nullptr);
}

BehaviorTree::BTIterator BehaviorTree::end() {
    return BTIterator(*this, nullptr);
}

