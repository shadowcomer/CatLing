#include "BehaviorTree.h"

#include <exception>
#include <assert.h>

#include <utility>

using namespace bt;

/************************
BTIterator
*************************/

BehaviorTree::BTIterator::BTIterator(BehaviorTree const & origin,
    Behavior * currentBehavior) :
m_currentBehavior(currentBehavior),
m_owner(origin) {

}

BehaviorTree::BTIterator::BTIterator() :
m_owner(std::move(BehaviorTree(BehaviorList()))),
m_currentBehavior(nullptr) {

}

BehaviorTree::BTIterator::~BTIterator() {

}

Behavior * BehaviorTree::BTIterator::operator*() {
    return m_currentBehavior;
}

BehaviorTree::BTIterator& BehaviorTree::BTIterator::operator++() {
    assert(nullptr != m_currentBehavior);
    m_currentBehavior = m_currentBehavior->nextBehavior();
    return *this;
}

bool BehaviorTree::BTIterator::operator!=(BTIterator const & other) {
    assert(&m_owner == &(other.m_owner));
    return m_currentBehavior != other.m_currentBehavior;
}

BehaviorTree::BTIterator&
    BehaviorTree::BTIterator::operator=(BTIterator other) {
    std::swap(*this, other);
    return *this;
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
    Behavior * head = m_behaviors.empty() ?
        nullptr : m_behaviors[0].get();
    return BTIterator(*this, head);
}

BehaviorTree::BTIterator BehaviorTree::end() {
    return BTIterator(*this, nullptr);
}

