#include "Node.h"

#include <gtest.h>
#include <exception>

using namespace BT;

Node::Node()
{
    m_children = std::vector<Child>(0);
    m_parent = Parent();
}

Node::~Node()
{

}

void Node::addChild(Node* newChild)
{
    ASSERT_NE(nullptr, newChild);
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