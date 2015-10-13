#include "Node.h"

#include <exception>

using namespace BT;

Node::Node()
{
    m_children = std::vector<Child>(0);
    m_parent = Parent();
}

Node::~Node()
{
    throw new std::exception("Not implemented");
}

void Node::addChild(Node* newChild)
{
    throw new std::exception("Not implemented");
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