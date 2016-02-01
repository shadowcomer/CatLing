/*
    This file contains the different Types that may be used in a Slab.

    A TypeObj is the abstract representation of these types, and it implements
    the required functionality for down-casting to a specific type.

    Each specific type gives public access to it's value for easy manipulation.
*/

// NOTE: Consider removing implicit conversion operators; they could hide bugs.

#ifndef TYPES_H
#define TYPES_H

#include <string>

#include "BWAPI\Unit.h"

namespace MM
{
    enum class Type
    {
        INT,
        BOOL,
        FLOAT,
        STRING,
        UNIT
    };
}
class IntType;
class BoolType;
class FloatType;
class StringType;
class UnitType;

class TypeObj
{
private:

public:
    TypeObj(MM::Type t);
    virtual ~TypeObj() = 0 {};

    auto toInt()->IntType*;
    auto toBool()->BoolType*;
    auto toFloat()->FloatType*;
    auto toString()->StringType*;
    auto toUnit()->UnitType*;

    operator IntType();
    operator BoolType();
    operator FloatType();
    operator StringType();
    operator UnitType();

    MM::Type const type;
};

class StringType : public TypeObj
{
public:
    StringType(std::string s);
    std::string value;
};

class FloatType : public TypeObj
{
public:
    FloatType(float f);
    float value;
};

class BoolType : public TypeObj
{
public:
    BoolType(bool b);
    bool value;
};

class IntType : public TypeObj
{
public:
    IntType(int val);
    int value;
};

class UnitType : public TypeObj
{
public:
    UnitType(BWAPI::Unit u);
    BWAPI::Unit value;
};

/*
Compares whether two given TypeObj are the same specific type.
*/
auto isSameType(TypeObj const * const t1, TypeObj const * const t2)->bool;

#endif