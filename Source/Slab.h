#ifndef SLAB_H
#define SLAB_H

#include <unordered_map>
#include <memory>

#include "../include/TBB/tbb/mutex.h"

#include "Types.h"

typedef std::unordered_map<std::string, TypeObj const * const> TypeList;
typedef std::vector<TypeObj*> Entry;

class Slab
{
private:
	TypeList const  m_fields;
	std::vector<TypeObj const * const > const m_fieldsVec;

	std::vector<std::vector<TypeObj*>> m_entries;

	auto generateFieldsVector(TypeList fields)->std::vector<TypeObj const * const>;

	tbb::mutex m_operationMutex;

public:
	Slab(TypeList const fields);
	Slab() = delete;
	Slab(Slab&) = delete;

	~Slab();

	auto canInsert(Entry entry)->bool;

	auto appendEntry(Entry entry)->bool;
	auto removeEntry(int i)->bool;
	auto modifyEntry(int i, int j, TypeObj* val)->bool;

	auto discover()->TypeList const;
};

#endif