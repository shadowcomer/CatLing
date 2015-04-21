#include "Types.h"
#include <assert.h>

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
	assert(typeid(IntType*) == typeid(this));
	return dynamic_cast<IntType*>(this);
 }

 auto TypeObj::toBool() -> BoolType*
 {
	assert(typeid(BoolType*) == typeid(this));
	return dynamic_cast<BoolType*>(this);
 }

auto TypeObj::toFloat() -> FloatType*
{
	assert(typeid(FloatType*) == typeid(this));
	return dynamic_cast<FloatType*>(this);
}


 auto TypeObj::toString() -> StringType*
{ 
	assert(typeid(StringType*) == typeid(this));
	return dynamic_cast<StringType*>(this);
}

 // Constructors

 IntType::IntType(int val) : value(val) {}
 BoolType::BoolType(bool val) : value(val) {}
 FloatType::FloatType(float val) : value(val) {}
 StringType::StringType(std::string val) : value(val) {}
