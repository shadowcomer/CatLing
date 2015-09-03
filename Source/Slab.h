#ifndef SLAB_H
#define SLAB_H

#include <unordered_map>
#include <memory>

#include "../include/TBB/tbb/mutex.h"
#include "../include/TBB/tbb/concurrent_hash_map.h"

#include "Types.h"

typedef tbb::concurrent_hash_map<std::string, TypeObj const * const> TypeList;
typedef std::vector<TypeObj const * const > TypeVec;
typedef std::vector<TypeObj*> Entry;
typedef std::vector<Entry> EntryList;

class Slab
{
private:
	TypeList const  m_fields;
	TypeVec const m_fieldsVec;

	EntryList m_entries;

	/*
	Generates an easily iterable vector. This is called during construction to
	guarantee that the TypeList is transcribed into something that will retain
	it's topology after every call; a hash map does not have this property.

	Do not call this function during the lifetime of the object.
	*/
	auto generateFieldsVector(TypeList fields)->std::vector<TypeObj const * const>;

	tbb::mutex SYNC_operation;

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