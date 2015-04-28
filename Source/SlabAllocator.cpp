#include "SlabAllocator.h"

SlabAllocator::SlabAllocator()
{
	
}

SlabAllocator::~SlabAllocator()
{

}

bool SlabAllocator::createSlab(const std::string name, const TypeList& fields)
{
	auto newSlab = new Slab(fields);
	auto res = m_slabs.emplace(std::pair<std::string, Slab*>(name, newSlab));
	return res.second;
}

bool SlabAllocator::destroySlab(const std::string name)
	{ return m_slabs.erase(name) == 1; }

auto SlabAllocator::begin()->std::unordered_map<std::string, Slab*>::iterator
	{ return m_slabs.begin(); }

auto SlabAllocator::begin() const ->std::unordered_map<std::string, Slab*>::const_iterator
	{ return m_slabs.begin(); }

auto SlabAllocator::end() ->std::unordered_map<std::string, Slab*>::iterator
	{ return m_slabs.end(); }

auto SlabAllocator::end() const->std::unordered_map<std::string, Slab*>::const_iterator
	{ return m_slabs.end(); }

auto SlabAllocator::find(std::string slabName)->std::unordered_map<std::string, Slab*>::iterator
	{ return m_slabs.find(slabName); }

auto SlabAllocator::find(std::string slabName) const->std::unordered_map<std::string, Slab*>::const_iterator
	{ return m_slabs.find(slabName); }