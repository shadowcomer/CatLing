#include "Types.h"

 TypeObj::operator IntType()
	{ return dynamic_cast<IntType&>(*this); }

 TypeObj::operator BoolType()
	{ return dynamic_cast<BoolType&>(*this); }

 auto TypeObj::toInt() -> IntType*
	{ return dynamic_cast<IntType*>(this); }

 auto TypeObj::toBool() -> BoolType*
	{  return dynamic_cast<BoolType*>(this); }


 IntType::IntType(int val) : value(val) {}
 BoolType::BoolType(bool val) : value(val) {}