#include "Composite.h"

using namespace BT;

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

State Composite::doIterate()
{
    return State::SUCCESS;
}

void Composite::notify(Parent const &who, State newState)
{

}