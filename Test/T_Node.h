#ifndef T_NODE_H
#define T_NODE_H

#include "gtest\gtest.h"

#include "Node.h"

class MockNode : public BT::Node
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

};

class T_NodeBasic : public ::testing::Test
{
protected:
    T_NodeBasic():
        m_node(new MockNode),
        m_extra(new MockNode)
    {

    }

    virtual ~T_NodeBasic()
    {
    }

    virtual void SetUp() {}
    virtual void TearDown() { }

    MockNode* m_node;
    MockNode* m_extra;
};

BT::Parent* MockNode::t_parent() {
    return &m_parent;
}

std::vector<BT::Child>* MockNode::t_children() {
    return &m_children;
}


TEST_F(T_NodeBasic, Constructor)
{
    BT::Parent* parent = m_node->t_parent();
    std::vector<BT::Child>* children = m_node->t_children();

    EXPECT_EQ(nullptr, parent->get());
    EXPECT_TRUE(children->empty());
}

TEST_F(T_NodeBasic, SetParent)
{
    BT::Parent* const parentVar = m_node->t_parent();
    BT::Node* setValue = nullptr;
    BT::Node* const expected = m_extra;

    m_node->setParent(m_extra);
    setValue = parentVar->get();

    ASSERT_EQ(expected, setValue);

    m_node->setParent(nullptr);
    EXPECT_EQ(nullptr, parentVar->get());
}

TEST_F(T_NodeBasic, AddChild)
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
        BT::Node* expected = m_extra;

        for (int i = 0; i < CHILD_COUNT; ++i){
            BT::Node* current_node_ptr = children->get();
            children++;
            EXPECT_EQ(expected, current_node_ptr);
        }
    }
}


#endif