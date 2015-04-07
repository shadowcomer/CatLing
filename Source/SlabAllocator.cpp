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
{
	return m_slabs.erase(name) == 1;
}