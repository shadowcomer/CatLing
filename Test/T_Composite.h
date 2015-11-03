#ifndef T_COMPOSITE_H
#define T_COMPOSITE_H

#include "gtest\gtest.h"

#include "Composite.h"

class MockComposite : BT::Composite
{
public:
    std::vector<BT::Child>* t_children();

    void notify(BT::Parent const &who,
        BT::State newState) {}

protected:
    BT::State doIterate() {}

};

std::vector<BT::Child>* MockComposite::t_children() {
    return &m_children;
}

class T_CompositeBasic : public ::testing::Test
{
protected:

    T_CompositeBasic() {}

    virtual ~T_CompositeBasic()
    {
    }

    virtual void SetUp() { }
    virtual void TearDown() { }

};

#endif