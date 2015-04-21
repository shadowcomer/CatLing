#ifndef TYPES_H
#define TYPES_H

class IntType;
class BoolType;

class TypeObj
{
public:
	virtual ~TypeObj() = 0 {};

	auto toInt()->IntType*;
	auto toBool()->BoolType*;

	operator IntType();
	operator BoolType();
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