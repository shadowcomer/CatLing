#include "MemoryManager.h"

MemoryManager::MemoryManager()
{
	
}

MemoryManager::~MemoryManager()
{

}

bool MemoryManager::createSlab(const std::string name, const TypeList& fields)
{
	auto newSlab = new Slab(fields);
	auto res = m_slabs.emplace(std::pair<std::string, Slab*>(name, newSlab));
	return res.second;
}

bool MemoryManager::destroySlab(const std::string name)
{
	return m_slabs.erase(name) == 1;
}