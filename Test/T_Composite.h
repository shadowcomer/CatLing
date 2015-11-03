#ifndef T_COMPOSITE_H
#define T_COMPOSITE_H

#include "gtest\gtest.h"

#include "Composite.h"

class MockComposite : public BT::Composite
{
public:
    std::vector<BT::Child>* t_children();

    void notify(BT::Parent const &who,
        BT::State newState) {}

protected:
    BT::State doIterate() { return BT::State::SUCCESS; }

};

std::vector<BT::Child>* MockComposite::t_children() {
    return &m_children;
}

class T_CompositeBasic : public ::testing::Test
{
protected:

    T_CompositeBasic():
        composite(std::make_shared<MockComposite>())
    {}

    virtual ~T_CompositeBasic()
    {
    }

    virtual void SetUp() { }
    virtual void TearDown() { }

    std::shared_ptr<MockComposite> composite;
};

TEST_F(T_CompositeBasic, Constructor)
{
    std::vector<BT::Child>* children = composite->t_children();
    EXPECT_EQ(0, children->size());
}

TEST_F(T_CompositeBasic, AddChildren)
{
    int const expected_children_count = 4;

    // Initialize
    std::vector<BT::Child>* children = composite->t_children();
    std::vector<BT::Child> expected;
    BT::Child not_expected;

    for (int i = 0; i < expected_children_count; i++){
        BT::Child tmp = std::make_shared<MockComposite>();
        expected.push_back(tmp);
    }

    // Test

    // Check the number of references prior to the test
    for (int i = 0; i < expected_children_count; i++){
        ASSERT_EQ(1, expected[0].use_count());
    }

    // Add 2 expected children
    composite->addChild(expected[0]);
    composite->addChild(expected[1]);
    ASSERT_EQ(2, children->size());
    EXPECT_EQ(expected[0].get(), (*children)[0].get());
    EXPECT_EQ(expected[1].get(), (*children)[1].get());

    // Add unexpected child
    composite->addChild(not_expected);
    ASSERT_EQ(2, children->size());
    EXPECT_EQ(expected[0].get(), (*children)[0].get());
    EXPECT_EQ(expected[1].get(), (*children)[1].get());

    // Add the rest of the children
    composite->addChild(expected[2]);
    composite->addChild(expected[3]);
    ASSERT_EQ(expected_children_count, children->size());
    EXPECT_EQ(expected[0].get(), (*children)[0].get());
    EXPECT_EQ(expected[1].get(), (*children)[1].get());
    EXPECT_EQ(expected[2].get(), (*children)[2].get());
    EXPECT_EQ(expected[3].get(), (*children)[3].get());

    // Check the number of references
    for (int i = 0; i < expected_children_count; i++){
        EXPECT_EQ(2, expected[0].use_count());
    }
}

#endif