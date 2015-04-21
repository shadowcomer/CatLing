#include "Types.h"

 TypeObj::operator IntType()
	{ return dynamic_cast<IntType&>(*this); }

 auto TypeObj::toInt() -> IntType*
	{ return dynamic_cast<IntType*>(this); }


 IntType::IntType(int val) : value(val) {}