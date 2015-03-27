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

	bool createSlab(const std::string name, const TypeList& fields);
	bool destroySlab(const std::string name);

private:
	std::unordered_map<std::string, Slab*> m_slabs;
};

#endif