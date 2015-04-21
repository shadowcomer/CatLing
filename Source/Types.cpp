#include "Types.h"

// Implicit conversion operators

 TypeObj::operator IntType()
	{ return dynamic_cast<IntType&>(*this); }

 TypeObj::operator BoolType()
	{ return dynamic_cast<BoolType&>(*this); }

 TypeObj::operator FloatType()
	{ return dynamic_cast<FloatType&>(*this); }

 // Special conversion functions

 auto TypeObj::toInt() -> IntType*
	{ return dynamic_cast<IntType*>(this); }

 auto TypeObj::toBool() -> BoolType*
	{  return dynamic_cast<BoolType*>(this); }

 auto TypeObj::toFloat() -> FloatType*
	{ return dynamic_cast<FloatType*>(this); }

 // Constructors

 IntType::IntType(int val) : value(val) {}
 BoolType::BoolType(bool val) : value(val) {}
 FloatType::FloatType(float val) : value(val) {}