#ifndef TYPES_H
#define TYPES_H

enum TypeName
{
	INTEGER,
};

class TypeObj
{
public:
	TypeObj(TypeName type) : CUR_TYPE(type) {};
	virtual ~TypeObj() = 0 {};
	const TypeName CUR_TYPE;

private:
	
};

template<class TYPE>
class TypeProperty
{
private:
	TYPE m_value;

public:
	template<class TYPE> inline TypeProperty(TYPE t){ m_value = t; }
	TypeProperty(){};
	inline virtual ~TypeProperty() = 0 {};

	inline auto val() -> decltype(m_value) & {
		return m_value;
	}
};

class IntType : public TypeObj, public TypeProperty < int >
{
public:
	inline IntType(int v) : TypeProperty(v), TypeObj(TypeName::INTEGER) {}
	~IntType() {};

private:

};



#endif