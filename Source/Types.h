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

	inline auto val() -> decltype(m_value) {
		return m_value;
	}
};

class IntType : public TypeObj, public TypeProperty < int >
{
public:
	inline IntType(int v) : TypeProperty(v) {}
	~IntType() {};

private:

};



#endif