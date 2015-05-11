#include "SlabAllocator.h"

SlabAllocator::SlabAllocator()
{
	
}

SlabAllocator::~SlabAllocator()
{

}

bool SlabAllocator::createSlab(const std::string name, const TypeList& fields)
{
	// Emplace guarantees uniqueness.
	Slab* s = new Slab(fields);
	bool result = false;
	{
		tbb::mutex::scoped_lock lock(m_operationMutex);
		auto item = m_slabs.emplace(name, s);
		result = item.second;
	}
	return result;
}

bool SlabAllocator::destroySlab(const std::string name)
{
	int erasedVals = 0;
	{
		tbb::mutex::scoped_lock lock(m_operationMutex);
		erasedVals = m_slabs.erase(name);
	}
	return erasedVals == 1;
}

auto SlabAllocator::begin()->std::unordered_map<std::string, Slab*>::iterator
	{ return m_slabs.begin(); }

auto SlabAllocator::begin() const ->std::unordered_map<std::string, Slab*>::const_iterator
	{ return m_slabs.begin(); }

auto SlabAllocator::end() ->std::unordered_map<std::string, Slab*>::iterator
	{ return m_slabs.end(); }

auto SlabAllocator::end() const->std::unordered_map<std::string, Slab*>::const_iterator
	{ return m_slabs.end(); }

auto SlabAllocator::find(std::string slabName)->std::unordered_map<std::string, Slab*>::iterator
{
	return m_slabs.find(slabName);
}

auto SlabAllocator::find(std::string slabName) const->std::unordered_map<std::string, Slab*>::const_iterator
	{ return m_slabs.find(slabName); }