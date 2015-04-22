#ifndef SLAB_H
#define SLAB_H

#include <unordered_map>
#include <memory>

#include "Types.h"

typedef std::unordered_map<std::string, TypeObj*> TypeList;

class Slab
{
private:
	const TypeList m_fields;

public:
	Slab(const TypeList fields);
	Slab() = delete;
	Slab(Slab&) = delete;

	~Slab(){};

	auto discover()->decltype(m_fields);
};

#endif