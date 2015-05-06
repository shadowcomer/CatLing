#ifndef TYPES_H
#define TYPES_H

#include <string>

class IntType;
class BoolType;
class FloatType;
class StringType;

class TypeObj
{
public:
	virtual ~TypeObj() = 0 {};

	auto toInt()->IntType*;
	auto toBool()->BoolType*;
	auto toFloat()->FloatType*;
	auto toString()->StringType*;

	operator IntType();
	operator BoolType();
	operator FloatType();
	operator StringType();
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

#endif