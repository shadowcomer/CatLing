#ifndef T_NODE_H
#define T_NODE_H

#include "gtest\gtest.h"

#include "Behavior.h"

class MockBehavior : public BT::Behavior
{
public:
    // #####################################
    // Functions to access protected members
    // of the tested class.
    // #####################################

    BT::Parent* t_parent();
    std::vector<BT::Child>* t_children();

    // #####################################
    // END access functions.
    // #####################################

protected:
    BT::State doIterate() { return BT::State::SUCCESS; }
    void notify(Behavior * who, BT::State newState) { }

};

class T_BehaviorBasic : public ::testing::Test
{
protected:
    T_BehaviorBasic():
        m_node(new MockBehavior),
        m_extra(new MockBehavior)
    {

    }

    virtual ~T_BehaviorBasic()
    {
    }

    virtual void SetUp() {}
    virtual void TearDown() { }

    MockBehavior* m_node;
    MockBehavior* m_extra;
};

BT::Parent* MockBehavior::t_parent() {
    return &m_parent;
}

std::vector<BT::Child>* MockBehavior::t_children() {
    return &m_children;
}


TEST_F(T_BehaviorBasic, Constructor)
{
    BT::Parent* parent = m_node->t_parent();
    std::vector<BT::Child>* children = m_node->t_children();

    EXPECT_EQ(nullptr, parent->get());
    EXPECT_TRUE(children->empty());
}

TEST_F(T_BehaviorBasic, SetParent)
{
    BT::Parent* const parentVar = m_node->t_parent();
    BT::Behavior* setValue = nullptr;
    BT::Behavior* const expected = m_extra;

    m_node->setParent(m_extra);
    setValue = parentVar->get();

    ASSERT_EQ(expected, setValue);

    m_node->setParent(nullptr);
    EXPECT_EQ(nullptr, parentVar->get());
}

TEST_F(T_BehaviorBasic, AddChild)
{
    int const CHILD_COUNT = 4;
    std::vector<BT::Child>* const childrenVar =
        m_node->t_children();

    for (int i = 0; i < CHILD_COUNT; ++i){
        m_node->addChild(m_extra);
    }

    {
        ASSERT_EQ(CHILD_COUNT, childrenVar->size());
        BT::Children children = childrenVar->begin();
        BT::Behavior* expected = m_extra;

        for (int i = 0; i < CHILD_COUNT; ++i){
            BT::Behavior* current_node_ptr = children->get();
            children++;
            EXPECT_EQ(expected, current_node_ptr);
        }
    }
}


#endif