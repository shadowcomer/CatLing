#include "Sequence.h"

using namespace BT;

Sequence::Sequence():
m_currentChildOffset(0)
{

}

Sequence::~Sequence()
{

}

State Sequence::doIterate()
{
    return State::SUCCESS;
}

void Sequence::treatNotification(Behavior const * const who,
    State oldState)
{

}