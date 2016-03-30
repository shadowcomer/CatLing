#include "Types.h"

using namespace SlabTypes;

// Implicit conversion operators

TypeObj::operator IntType()
{
    return dynamic_cast<IntType&>(*this);
}

TypeObj::operator BoolType()
{
    return dynamic_cast<BoolType&>(*this);
}

TypeObj::operator FloatType()
{
    return dynamic_cast<FloatType&>(*this);
}

TypeObj::operator StringType()
{
    return dynamic_cast<StringType&>(*this);
}

TypeObj::operator UnitType()
{
    return dynamic_cast<UnitType&>(*this);
}

// Special conversion functions

auto TypeObj::toInt() -> IntType*
{
    return dynamic_cast<IntType*>(this);
}

auto TypeObj::toBool() -> BoolType*
{
    return dynamic_cast<BoolType*>(this);
}

auto TypeObj::toFloat() -> FloatType*
{
    return dynamic_cast<FloatType*>(this);
}


auto TypeObj::toString() -> StringType*
{
    return dynamic_cast<StringType*>(this);
}

auto TypeObj::toUnit() -> UnitType*
{
    return dynamic_cast<UnitType*>(this);
}

// Constructors

TypeObj::TypeObj(Type t) : type(t) {}

IntType::IntType(int val) : TypeObj(Type::INT), value(val) {}
BoolType::BoolType(bool val) : TypeObj(Type::BOOL), value(val) {}
FloatType::FloatType(float val) : TypeObj(Type::FLOAT), value(val) {}
StringType::StringType(std::string val) : TypeObj(Type::STRING), value(val) {}
UnitType::UnitType(BWAPI::Unit val) : TypeObj(Type::UNIT), value(val) {}


// Inter functionality

auto SlabTypes::isSameType(TypeObj const * const t1, TypeObj const * const t2)->bool
{
    if (t1 == nullptr || t2 == nullptr)
        return false;

    return t1->type == t2->type;
}