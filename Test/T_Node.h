#ifndef T_NODE_H
#define T_NODE_H

#include "gtest\gtest.h"

#define private public
#include "Node.h"

class T_NodeBasic : public ::testing::Test
{
protected:
    T_NodeBasic():
        m_node(new BT::Node),
        m_extra(new BT::Node)
    {

    }

    virtual ~T_NodeBasic()
    {
    }

    virtual void SetUp() {}
    virtual void TearDown() { }

    BT::Node* m_node;
    BT::Node* m_extra;
};

TEST_F(T_NodeBasic, Constructor)
{
    EXPECT_EQ(nullptr, m_node->m_parent.get());
    EXPECT_TRUE(m_node->m_children.empty());
}

TEST_F(T_NodeBasic, SetNullParent)
{
    m_node->setParent(nullptr);
    EXPECT_EQ(nullptr, m_node->m_parent.get());
}

TEST_F(T_NodeBasic, SetParent)
{
    m_node->setParent(m_extra);
    ASSERT_EQ(m_node->m_parent.get(), m_extra);
}


#endif