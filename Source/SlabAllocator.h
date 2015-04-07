#ifndef SLABALLOCATOR_H
#define SLABALLOCATOR_H

#include <unordered_map>
#include <memory>

#include "Slab.h"

class SlabAllocator
{
public:
	SlabAllocator();
	~SlabAllocator();

	bool createSlab(const std::string name, const TypeList& fields);
	bool destroySlab(const std::string name);

private:
	std::unordered_map<std::string, Slab*> m_slabs;
};

#endif