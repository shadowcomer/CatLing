#ifndef T_NODE_H
#define T_NODE_H

#include "Node.h"
#include "gtest\gtest.h"

namespace {
    class T_Node : public ::testing::Test
    {
    protected:
        T_Node() {}
        virtual ~T_Node() {}
        virtual void SetUp() {}
        virtual void TearDown() {}
    };

    TEST_F(T_Node, Primer) {
        EXPECT_EQ(1, 0);
    }
}

#endif