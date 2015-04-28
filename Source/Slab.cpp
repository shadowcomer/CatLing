#include "Slab.h"

Slab::Slab(TypeList const fields) :
m_fields(fields)
{

}

auto Slab::discover()->TypeList const
	{ return m_fields; }
