#ifndef STATICMEMORYALLOCATOR_H
#define STATICMEMORYALLOCATOR_H

class StaticMemoryAllocator
{
public:
	StaticMemoryAllocator(const unsigned int initialContainerCapacity);
	~StaticMemoryAllocator();

private:
	unsigned int m_initialContainerCapacity;

};

#endif