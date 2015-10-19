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

TEST_F(T_NodeBasic, SetNullParent)
{
    BT::Parent* parent = m_node->t_parent();
    m_node->setParent(nullptr);
    EXPECT_EQ(nullptr, parent->get());
}

TEST_F(T_NodeBasic, SetParent)
{
    BT::Parent* const parentVar = m_node->t_parent();
    BT::Node* setValue = nullptr;
    BT::Node* const expected = m_extra;

    m_node->setParent(m_extra);
    setValue = parentVar->get();

    ASSERT_EQ(expected, setValue);
}

TEST_F(T_NodeBasic, Children)
{
    std::vector<BT::Child>* const childrenVar =
        m_node->t_children();

    BT::Children expected_ch = childrenVar->begin();
    BT::Children ch = m_node->children();
    EXPECT_TRUE(expected_ch == ch);

    BT::Children expected_ch_end = childrenVar->end();
    BT::Children ch_end = m_node->children_end();
    EXPECT_TRUE(expected_ch_end == ch_end);

}

TEST_F(T_NodeBasic, Parent)
{
    BT::Parent* const parentVar =
        m_node->t_parent();

    {
        BT::Node* expected_ptr = parentVar->get();
        BT::Node* actual_ptr = m_node->parent().get();
        EXPECT_EQ(expected_ptr, actual_ptr);
    }

    {
        *parentVar = BT::Parent(m_extra);
        BT::Node* expected_ptr = parentVar->get();
        BT::Node* actual_ptr = m_node->parent().get();
        EXPECT_EQ(expected_ptr, actual_ptr);
    }
    
}


#endif