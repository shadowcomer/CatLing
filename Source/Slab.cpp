#include "Slab.h"

Slab::Slab(TypeList const fields) :
m_fields(fields),
m_fieldsVec(generateFieldsVector(m_fields))
{

}

Slab::~Slab()
{

}

auto Slab::generateFieldsVector(TypeList fields)->std::vector<TypeObj const * const>
{
	std::vector < TypeObj const * const> vec;
	for each(auto elem in m_fields)
	{
		vec.push_back(elem.second);
	}

	return vec;
}

auto Slab::canInsert(std::vector<TypeObj*> entry)->bool
{
	if (entry.size() != m_entries.size())
	{
		return false;
	}

	for (int i = 0; i < m_entries.size(); i++)
	{
		if (!isSameType(entry[i], m_fieldsVec[i]))
			return false;
	}

	return true;
}

auto Slab::appendEntry()->bool
{

	return true;
}

auto Slab::discover()->TypeList const
	{ return m_fields; }
