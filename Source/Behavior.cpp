#include "Behavior.h"

#include <gtest.h>
#include <exception>

using namespace BT;

Behavior::Behavior() :
m_state(State::INITIAL),
m_iterations(0),
m_enterHook(nullptr),
m_iterationHook(nullptr),
m_exitHook(nullptr)
{
    m_parent = Parent();
}

Behavior::~Behavior()
{

}

void Behavior::setParent(Parent const & newParent)
{
    m_parent = newParent;
}

Parent Behavior::parent()
{
    return m_parent;
}

State Behavior::state() const
{
    return m_state;
}

bool Behavior::is(State state) const
{
    return m_state == state;
}

void Behavior::setEnterHook(EnterHook fn)
{
    m_enterHook = fn;
}

void Behavior::callEnterHook()
{
    if (nullptr != m_enterHook){
        m_enterHook(m_state);
    }
}

void Behavior::setIterationHook(IterateHook fn)
{
    m_iterationHook = fn;
}

void Behavior::callIterationHook()
{
    if (nullptr != m_iterationHook){
        m_iterationHook(m_state);
    }
}

void Behavior::setExitHook(ExitHook fn)
{
    m_exitHook = fn;
}

void Behavior::callExitHook()
{
    if (nullptr != m_exitHook){
        m_exitHook(m_state);
    }
}

State Behavior::iterate()
{
    assert(!terminated());

    // Initialization
    if (State::INITIAL == m_state){
        callEnterHook();
    }

    // Execution
    m_iterations++;
    m_state = doIterate();
    callIterationHook();

    // Exit
    if (State::RUNNING != m_state){
        callExitHook();
    }

    return m_state;
}

int Behavior::iterations() const
{
    return m_iterations;
}

bool Behavior::terminated() const
{
    return !((m_state == State::INITIAL) ||
        (m_state == State::RUNNING));
}