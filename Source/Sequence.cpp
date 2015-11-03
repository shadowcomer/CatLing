#include "Sequence.h"

using namespace BT;

void Sequence::addChild(Child const & newChild)
{
    if (nullptr == newChild.get()){
        return;
    }

    m_children.push_back(newChild);
}

Children Sequence::children()
{
    return m_children.begin();
}

Children Sequence::children_end()
{
    return m_children.end();
}

void Sequence::notify(Parent const &who, State newState)
{

}