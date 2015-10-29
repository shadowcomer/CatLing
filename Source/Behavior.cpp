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
    m_children = std::vector<Child>(0);
    m_parent = Parent();
}

Behavior::~Behavior()
{

}

void Behavior::addChild(Behavior* newChild)
{
    if (!newChild){
        return;
    }

    m_children.push_back(Child(newChild));
}

void Behavior::setParent(Behavior* newParent)
{
    if (!newParent){
        m_parent = Parent();
        return;
    }

    Children child = newParent->children();
    if (child != newParent->m_children.end()){
        // Get the parent of the first child.
        // This forces the smartpointer to use the same
        // reference for memory management, instead of
        // building a new one, which wouldn't be shared.
        Parent p = child->get()->m_parent;
        m_parent = p;
    }
    else {
        // Create its own reference.
        m_parent = Parent(newParent);
    }
}

Parent Behavior::parent()
{
    return m_parent;
}

Children Behavior::children()
{
    return m_children.begin();
}

Children Behavior::children_end()
{
    return m_children.end();
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
    m_enterHook = fn;
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