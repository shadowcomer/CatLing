#include "StaticMemoryAllocator.h"

#define DEFAULT_INITIAL_CONTAINER_SIZE 30

StaticMemoryAllocator::StaticMemoryAllocator(const unsigned int initialContainerCapacity = DEFAULT_INITIAL_CONTAINER_SIZE) :
m_initialContainerCapacity(initialContainerCapacity)
{

}

StaticMemoryAllocator::~StaticMemoryAllocator()
{

}