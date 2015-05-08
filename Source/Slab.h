#ifndef SLAB_H
#define SLAB_H

#include <unordered_map>
#include <memory>

#include "Types.h"

typedef std::unordered_map<std::string, TypeObj const * const> TypeList;

class Slab
{
private:
	TypeList const  m_fields;
	std::vector<TypeObj const * const > const m_fieldsVec;

	std::vector<std::vector<TypeObj*>> m_entries;

	auto generateFieldsVector(TypeList fields)->std::vector<TypeObj const * const>;

public:
	Slab(TypeList const fields);
	Slab() = delete;
	Slab(Slab&) = delete;

	~Slab();

	auto canInsert(std::vector<TypeObj*> entry)->bool;

	auto appendEntry()->bool;

	auto discover()->TypeList const;
};

#endif