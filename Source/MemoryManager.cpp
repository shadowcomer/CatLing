#include "MemoryManager.h"

MemoryManager::MemoryManager()
{
	
}

MemoryManager::~MemoryManager()
{

}

bool MemoryManager::createSlab(const std::string name)
{
	auto newSlab = std::make_unique<Slab>();
	auto res = m_slabs.emplace(std::pair<std::string, std::unique_ptr<Slab>>(name, std::move(newSlab)));
	return res.second;
}

bool MemoryManager::destroySlab(const std::string name)
{
	return m_slabs.erase(name) == 1;
}