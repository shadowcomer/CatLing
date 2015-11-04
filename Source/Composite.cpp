#include "Composite.h"

using namespace BT;

Composite::Composite():
m_children(std::vector<Child>())
{

}

Composite::~Composite()
{

}

void Composite::addChild(Child const & newChild)
{
    if (nullptr == newChild.get()){
        return;
    }

    m_children.push_back(newChild);
}

Children Composite::children()
{
    return m_children.begin();
}

Children Composite::children_end()
{
    return m_children.end();
}

