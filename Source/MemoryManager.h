#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include <unordered_map>
#include <memory>

#include "Slab.h"

class MemoryManager
{
public:
	MemoryManager();
	~MemoryManager();

	bool createSlab(const std::string name);
	bool destroySlab(const std::string name);

private:
	std::unordered_map<std::string, std::unique_ptr<Slab>> m_slabs;
};

#endif