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
    ASSERT_NE(nullptr, newParent);

    // Copy construct the shared_ptr from a sibling, when it exists.
    Children child = newParent->children();
    if (child != newParent->m_children.end()){
        Parent p = child->get()->m_parent;
        m_parent = p;
    }
    else {
        m_parent = Parent(newParent);
    }
}

Parent Node::parent()
{
    throw new std::exception("Not implemented");
}

Children Node::children()
{
    throw new std::exception("Not implemented");
}