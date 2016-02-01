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

 TypeObj::operator UnitType()
    { return dynamic_cast<UnitType&>(*this); }

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

auto TypeObj::toUnit() -> UnitType*
{
     return typeid(UnitType*) == typeid(this) ? dynamic_cast<UnitType*>(this) : nullptr;
}

 // Constructors

 TypeObj::TypeObj(MM::Type t) : type(t) {}

 IntType::IntType(int val) : TypeObj(MM::Type::INT), value(val) {}
 BoolType::BoolType(bool val) : TypeObj(MM::Type::BOOL), value(val) {}
 FloatType::FloatType(float val) : TypeObj(MM::Type::FLOAT), value(val) {}
 StringType::StringType(std::string val) : TypeObj(MM::Type::STRING), value(val) {}
 UnitType::UnitType(BWAPI::Unit val) : TypeObj(MM::Type::UNIT), value(val) {}


 // Inter functionality

 auto isSameType(TypeObj const * const t1, TypeObj const * const t2)->bool
 {
     if (t1 == nullptr || t2 == nullptr)
         return false;

     return t1->type == t2->type;
 }