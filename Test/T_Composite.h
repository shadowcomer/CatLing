#ifndef T_SEQUENCE_H
#define T_SEQUENCE_H

#include "gtest\gtest.h"

#include "Composite.h"

class MockComposite : BT::Composite
{
public:
    std::vector<BT::Child>* t_children();

protected:

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