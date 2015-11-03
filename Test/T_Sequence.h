#ifndef T_SEQUENCE_H
#define T_SEQUENCE_H

#include "gtest\gtest.h"

#include "Sequence.h"

class MockSequence : BT::Sequence
{
public:
    std::vector<BT::Child>* t_children();

protected:

};

std::vector<BT::Child>* MockSequence::t_children() {
    return &m_children;
}

class T_SequenceBasic : public ::testing::Test
{
protected:

    T_SequenceBasic() {}

    virtual ~T_SequenceBasic()
    {
    }

    virtual void SetUp() { }
    virtual void TearDown() { }

};

#endif