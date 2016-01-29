#ifndef T_BEHAVIORTREE_H
#define T_BEHAVIORTREE_H

#include "gtest/gtest.h"

#include "BehaviorTree.h"

using namespace std;
using namespace bt;

class DummyBehavior : public Behavior {
public:
    DummyBehavior(int id):
    Behavior(nullptr, BehaviorMonitor()),
    id(id) {

    }

    DummyBehavior() = delete;
    ~DummyBehavior() = default;

    Behavior * nextBehavior() override {
        return nullptr;
    }

    void tick() override {
        std::cout << "Behavior id: " << id << std::endl;
    }

    int const id;

private:

};

class T_BehaviorTree : public ::testing::Test {
public:
    T_BehaviorTree() {

    }

    ~T_BehaviorTree() {

    }

    void SetUp() {

    }

    void TearDown() {

    }

private:

};

TEST_F(T_BehaviorTree, example) {
    std::vector<std::unique_ptr<Behavior>> tmpBehaviors;
    tmpBehaviors.push_back(std::make_unique<DummyBehavior>(1));
    tmpBehaviors.push_back(std::make_unique<DummyBehavior>(2));

    std::cout << "Before..." << std::endl;
    for (auto& behavior : tmpBehaviors) {
        behavior->tick();
    }

    BehaviorTree tree{ std::move(tmpBehaviors) };

    std::cout << "After..." << std::endl;
    for (auto& behavior : tmpBehaviors) {
        behavior->tick();
    }

    ASSERT_EQ(1, 2);
}

#endif