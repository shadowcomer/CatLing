#include "SlabAllocator.h"
#include <iostream>

SlabAllocator::SlabAllocator()
{
    m_slabs = SlabMap(10);
}

SlabAllocator::~SlabAllocator()
{

}

bool SlabAllocator::createSlab(std::string name, TypeList fields)
{
    std::string n(name);
    TypeList f(fields);
    bool completed = false;

    {
        Slab* s = new Slab(f);
        SlabMap::accessor a;

        completed = m_slabs.insert(a, n);
        a->second = s;
        a.release();
    }

    return completed;
}

bool SlabAllocator::destroySlab(const std::string name)
{
    return m_slabs.erase(name);
}

auto SlabAllocator::begin()->SlabMap::iterator
    { return m_slabs.begin(); }

auto SlabAllocator::begin() const ->SlabMap::const_iterator
    { return m_slabs.begin(); }

auto SlabAllocator::end() ->SlabMap::iterator
    { return m_slabs.end(); }

auto SlabAllocator::end() const->SlabMap::const_iterator
    { return m_slabs.end(); }

auto SlabAllocator::find(std::string slabName, Slab** result)->bool
{
    Slab* slab = nullptr;
    {
        SlabMap::accessor a;
        if(m_slabs.find(a, slabName))
            slab = a->second;
        a.release();

        if (result != nullptr)
            *result = slab;
    }
    return slab != nullptr;
}
