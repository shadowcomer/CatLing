#ifndef TYPES_H
#define TYPES_H

class TypeObj
{
public:
	inline virtual ~TypeObj() = 0 {};

private:
	
};

template<class TYPE>
class TypeProperty
{
private:
	TYPE m_value;

public:
	template<class TYPE> inline TypeProperty(TYPE t){ m_value = t; }
	inline virtual ~TypeProperty() = 0 {};

	inline virtual auto val() -> decltype(m_value) = 0;
};

class IntType : public TypeObj, public TypeProperty < int >
{
public:
	inline IntType(int v) : TypeProperty(v) {}
private:

};



#endif