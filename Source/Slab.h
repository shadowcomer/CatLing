#ifndef SLAB_H
#define SLAB_H

#include <vector>
#include <memory>

#include "Types.h"

typedef std::vector<TypeObj*> TypeList;

class Slab
{
public:
	Slab(const TypeList& fields);
	Slab(){};
	~Slab(){};

private:
	TypeList m_fields;

};

#endif