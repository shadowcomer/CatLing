#include "BehaviorTree.h"

#include <exception>

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

BehaviorTree::BehaviorTree(BehaviorList&& behaviors):
m_behaviors(std::move(behaviors)) {

}

BehaviorTree::BehaviorTree(BehaviorTree const & original) {
    throw new std::exception("Copy Constructor not implemented.");
}

BehaviorTree::BTIterator BehaviorTree::begin() {
    return BTIterator(*this, nullptr);
}

BehaviorTree::BTIterator BehaviorTree::end() {
    return BTIterator(*this, nullptr);
}

