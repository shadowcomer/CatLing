#ifndef SLABALLOCATOR_H
#define SLABALLOCATOR_H

#include <unordered_map>
#include <memory>

#include "../include/TBB/tbb/mutex.h"
#include "../include/TBB/tbb/concurrent_hash_map.h"

#include "Slab.h"

typedef tbb::concurrent_hash_map<std::string, Slab*> SlabMap;

class SlabAllocator
{
private:
	SlabMap m_slabs;
	tbb::mutex m_operationMutex;

public:
	SlabAllocator();
	~SlabAllocator();

	bool createSlab(std::string name, TypeList fields);
	bool destroySlab(const std::string name);

	auto begin()->SlabMap::iterator;
	auto begin() const ->SlabMap::const_iterator;

	auto end()->decltype(m_slabs.end());
	auto end() const->SlabMap::const_iterator;

	auto find(std::string slabName, Slab** result)->bool;
};

#endif