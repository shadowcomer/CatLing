#include "Types.h"

// Implicit conversion operators

 TypeObj::operator IntType()
	{ return dynamic_cast<IntType&>(*this); }

 TypeObj::operator BoolType()
	{ return dynamic_cast<BoolType&>(*this); }

 TypeObj::operator FloatType()
	{ return dynamic_cast<FloatType&>(*this); }

 TypeObj::operator StringType()
	{ return dynamic_cast<StringType&>(*this); }

 // Special conversion functions

 auto TypeObj::toInt() -> IntType*
 {
	return typeid(IntType*) == typeid(this) ? dynamic_cast<IntType*>(this) : nullptr;
 }

 auto TypeObj::toBool() -> BoolType*
 {
	return typeid(BoolType*) == typeid(this) ? dynamic_cast<BoolType*>(this) : nullptr;
 }

auto TypeObj::toFloat() -> FloatType*
{
	return typeid(FloatType*) == typeid(this) ? dynamic_cast<FloatType*>(this) : nullptr;
}


 auto TypeObj::toString() -> StringType*
{ 
	return typeid(StringType*) == typeid(this) ? dynamic_cast<StringType*>(this) : nullptr;
}

 // Constructors

 IntType::IntType(int val) : value(val) {}
 BoolType::BoolType(bool val) : value(val) {}
 FloatType::FloatType(float val) : value(val) {}
 StringType::StringType(std::string val) : value(val) {}