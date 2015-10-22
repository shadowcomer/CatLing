#include "Node.h"

#include <gtest.h>
#include <exception>

using namespace BT;

Node::Node() :
m_state(State::INVALID),
m_enterHook(nullptr),
m_iterationHook(nullptr),
m_exitHook(nullptr)
{
    m_children = std::vector<Child>(0);
    m_parent = Parent();
}

Node::~Node()
{

}

void Node::addChild(Node* newChild)
{
    if (!newChild){
        return;
    }

    m_children.push_back(Child(newChild));
}

void Node::setParent(Node* newParent)
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

Parent Node::parent()
{
    return m_parent;
}

Children Node::children()
{
    return m_children.begin();
}

Children Node::children_end()
{
    return m_children.end();
}

State Node::state()
{
    return m_state;
}

bool Node::is(State state)
{
    return m_state == state;
}

void Node::setEnterHook(Hook fn)
{
    m_enterHook = fn;
}

void Node::callEnterHook()
{
    if (nullptr != m_enterHook){
        m_enterHook(m_state);
    }
}

void Node::setIterationHook(Hook fn)
{
    m_enterHook = fn;
}

void Node::callIterationHook()
{
    if (nullptr != m_iterationHook){
        m_iterationHook(m_state);
    }
}

void Node::setExitHook(Hook fn)
{
    m_exitHook = fn;
}

void Node::callExitHook()
{
    if (nullptr != m_exitHook){
        m_exitHook(m_state);
    }
}

State Node::iterate()
{
    // Initialization
    if (State::INVALID == m_state){
        callEnterHook();
    }

    // Execution
    m_state = doIterate();

    // Exit
    if (State::RUNNING != m_state){
        callExitHook();
    }

    return m_state;
}
