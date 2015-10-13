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
    throw new std::exception("Not implemented");
}

Parent Node::parent()
{
    throw new std::exception("Not implemented");
}

Children Node::children()
{
    throw new std::exception("Not implemented");
}