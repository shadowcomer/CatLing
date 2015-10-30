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

    MockBehavior() :
        enter_count(0),
        iter_count(0),
        exit_count(0)
    {};

    BT::Parent* t_parent();
    vector<BT::Child>* t_children();

    // #####################################
    // END access functions.
    // #####################################
    void notify(BT::Parent const & who, BT::State newState) { }
    BT::State doIterate() {
        return iter_count >= 1 ?
            BT::State::SUCCESS : BT::State::RUNNING;
    }

    void incEnter(BT::State) {
        enter_count++;
    }

    void incExit(BT::State) {
        exit_count++;
    }

    void incIter(BT::State) {
        iter_count++;
    }

    int enter_count;
    int exit_count;
    int iter_count;

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
    ASSERT_NE(nullptr, behavior.get());
    ASSERT_NE(nullptr, expected_set.get());

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

TEST_F(T_BehaviorBasic, Iterate)
{
    using namespace std::placeholders;
    std::shared_ptr<MockBehavior> behavior =
        make_shared<MockBehavior>();

    BT::EnterHook enter_h =
        std::bind(&MockBehavior::incEnter, behavior, _1);
    BT::IterateHook iterate_h =
        std::bind(&MockBehavior::incIter, behavior, _1);
    BT::ExitHook exit_h =
        std::bind(&MockBehavior::incExit, behavior, _1);

    behavior->setEnterHook(enter_h);
    behavior->setIterationHook(iterate_h);
    behavior->setExitHook(exit_h);

    ASSERT_EQ(0, behavior->enter_count);
    ASSERT_EQ(0, behavior->iter_count);
    ASSERT_EQ(0, behavior->exit_count);

    behavior->iterate();

    EXPECT_EQ(1, behavior->enter_count);
    EXPECT_EQ(1, behavior->iter_count);
    EXPECT_EQ(0, behavior->exit_count);

    behavior->iterate();

    EXPECT_EQ(1, behavior->enter_count);
    EXPECT_EQ(2, behavior->iter_count);
    EXPECT_EQ(1, behavior->exit_count);

    EXPECT_DEATH(behavior->iterate(), "^");
    // From here on, behavior is INVALID.
}

#endif