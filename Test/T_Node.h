#ifndef T_NODE_H
#define T_NODE_H

#include "Node.h"
#include "gtest\gtest.h"

class T_Node : public ::testing::Test
{
protected:
    T_Node() {}
    virtual ~T_Node() {}
    virtual void SetUp() {}
    virtual void TearDown() {}
};

TEST(NodeTest, Constructor)
{
    // Check parent initialization
    BT::Node n{};
    EXPECT_EQ(nullptr, n.parent().get());

    // Check children initialization
    BT::Children first = n.children();
    BT::Children end = n.children_end();
    EXPECT_TRUE(first == end);
}


#endif