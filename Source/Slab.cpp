#include "Slab.h"

Slab::Slab(const TypeList fields) :
m_fields(fields)
{

}

auto Slab::discover()->decltype(m_fields)
{
	return m_fields;
}
