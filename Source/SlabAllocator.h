#ifndef SLABALLOCATOR_H
#define SLABALLOCATOR_H

#include <unordered_map>
#include <memory>

#include "../include/TBB/tbb/mutex.h"

#include "Slab.h"

class SlabAllocator
{
public:
	SlabAllocator();
	~SlabAllocator();

	bool createSlab(const std::string name, const TypeList& fields);
	bool destroySlab(const std::string name);

	auto begin()->std::unordered_map<std::string, Slab*>::iterator;
	auto begin() const ->std::unordered_map<std::string, Slab*>::const_iterator;

	auto end()->std::unordered_map<std::string, Slab*>::iterator;
	auto end() const->std::unordered_map<std::string, Slab*>::const_iterator;

	auto find(std::string slabName)->std::unordered_map<std::string, Slab*>::iterator;
	auto find(std::string slabName) const->std::unordered_map<std::string, Slab*>::const_iterator;

private:
	std::unordered_map<std::string, Slab*> m_slabs;
	tbb::mutex m_operationMutex;
};

#endif