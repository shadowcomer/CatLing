#include "Slab.h"

Slab::Slab(TypeList const fields) :
m_fields(fields)
{

}

Slab::~Slab()
{

}

auto Slab::canInsert(std::vector<TypeObj*> entry)->bool
{
	if (entry.size() != m_entries.size())
	{
		return false;
	}

	for each(auto base in m_entries)
	{

	}

	return true;
}

auto Slab::appendEntry()->bool
{

	return true;
}

auto Slab::discover()->TypeList const
	{ return m_fields; }
