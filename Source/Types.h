#ifndef TYPES_H
#define TYPES_H

class IntType;

class TypeObj
{
public:
	virtual ~TypeObj() = 0 {};

	auto toInt()->IntType*;
	operator IntType();
};

class IntType : public TypeObj
{
public:
	IntType(int val);
	int value;

private:
};

#endif