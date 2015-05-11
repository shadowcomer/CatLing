#include "Slab.h"
#include <assert.h>

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

auto Slab::canInsert(Entry entry)->bool
{
	if (entry.size() != m_fieldsVec.size())
	{
		return false;
	}

	for (int i = 0; i < m_fieldsVec.size(); i++)
	{
		if (!isSameType(entry[i], m_fieldsVec[i]))
			return false;
	}

	return true;
}

auto Slab::appendEntry(Entry entry)->bool
{
	if (!canInsert(entry))
	{
		return false;
	}

	{
		tbb::mutex::scoped_lock lock(m_operationMutex);
		m_entries.push_back(entry);
	}
	return true;
}

auto Slab::removeEntry(int i)->bool
{
	if (m_entries.empty() || i < 0 || i >(m_entries.size() - 1))
		return false;

	{
		tbb::mutex::scoped_lock lock(m_operationMutex);
		auto it = m_entries.begin();
		std::advance(it, i);
		m_entries.erase(it);
	}
	return true;
}

auto Slab::modifyEntry(int i, int j, TypeObj* val)->bool
{
	if (val == nullptr)
		return false;

	// Check if 'j' is a valid field offset
	if (m_fieldsVec.empty() || j < 0 || j >(m_fieldsVec.size() - 1))
		return false;

	// Make sure it's the correct type
	if (val->type != m_fieldsVec[j]->type)
		return false;

	{
		tbb::mutex::scoped_lock lock(m_operationMutex);

		// Check if 'i' is a valid entry offset
		if (m_entries.empty() || i < 0 || i >(m_entries.size() - 1))
			return false;

		auto entry = m_entries.begin();
		std::advance(entry, i);
		auto entryField = (*entry)[j];

		switch (val->type)
		{
		case Type::INT:
			auto iParamObj = val->toInt();
			auto iTargetObj = entryField->toInt();

			assert(iParamObj != nullptr);
			assert(iTargetObj != nullptr);

			iTargetObj->value = iParamObj->value;
			break;

		case Type::BOOL:
			auto bParamObj = val->toBool();
			auto bTargetObj = entryField->toBool();

			assert(bParamObj != nullptr);
			assert(bTargetObj != nullptr);

			bTargetObj->value = iParamObj->value;
			break;

		case Type::FLOAT:
			auto fParamObj = val->toFloat();
			auto fTargetObj = entryField->toFloat();

			assert(fParamObj != nullptr);
			assert(fTargetObj != nullptr);

			fTargetObj->value = fParamObj->value;
			break;

		case Type::STRING:
			auto sParamObj = val->toString();
			auto sTargetObj = entryField->toString();

			assert(sParamObj != nullptr);
			assert(sTargetObj != nullptr);

			sTargetObj->value = sParamObj->value;
			break;

		default:
			assert(false);
			return false;
		}
	}
	return true;
}

auto Slab::discover()->TypeList const
	{ return m_fields; }
