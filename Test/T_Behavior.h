#ifndef T_NODE_H
#define T_NODE_H

#include "gtest\gtest.h"

#include "Behavior.h"

using namespace std;

class MockBehavior : public BT::Behavior
{
public:
    // #####################################
    // Functions to access protected members
    // of the tested class.
    // #####################################

    BT::Parent* t_parent();
    vector<BT::Child>* t_children();

    // #####################################
    // END access functions.
    // #####################################
    void notify(BT::Parent const & who, BT::State newState) { 3 + 3; }
    BT::State doIterate() { return BT::State::SUCCESS; }

protected:
    

};

class T_BehaviorBasic : public ::testing::Test
{
protected:
    T_BehaviorBasic()
    {

    }

    virtual ~T_BehaviorBasic()
    {
    }

    virtual void SetUp() {}
    virtual void TearDown() { }
};

BT::Parent* MockBehavior::t_parent() {
    return &m_parent;
}

vector<BT::Child>* MockBehavior::t_children() {
    return &m_children;
}


TEST_F(T_BehaviorBasic, Constructor)
{
    shared_ptr<MockBehavior> behavior = 
        make_shared<MockBehavior>();
    ASSERT_NE(nullptr, behavior);

    BT::Parent* parent = behavior->t_parent();
    vector<BT::Child>* children = behavior->t_children();

    EXPECT_EQ(nullptr, parent->get());
    EXPECT_TRUE(children->empty());
}

TEST_F(T_BehaviorBasic, SetParent)
{
    shared_ptr<MockBehavior> behavior = 
        make_shared<MockBehavior>();
    shared_ptr<MockBehavior> expected_set =
        make_shared<MockBehavior>();
    shared_ptr<MockBehavior> expected_clear = nullptr;
    ASSERT_NE(nullptr, behavior);
    ASSERT_NE(nullptr, expected_set);

    BT::Parent* const parentVar = behavior->t_parent();
    BT::Behavior* current_setValue = nullptr;

    // Check whether we can set a new Parent
    // OPERATION
    behavior->setParent(expected_set);

    // CHECK
    current_setValue = parentVar->get();
    ASSERT_EQ(expected_set.get(), current_setValue);

    // Check whether we can remove the current Parent
    // OPERATION
    behavior->setParent(nullptr);

    // CHECK
    current_setValue = parentVar->get();
    EXPECT_EQ(expected_clear.get(), parentVar->get());
}

#endif