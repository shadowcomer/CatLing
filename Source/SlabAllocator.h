/*
    A SlabAllocator manages a set of Slabs [Slab]. Each Slab is named.

    This class is thread-safe.
*/

#ifndef SLABALLOCATOR_H
#define SLABALLOCATOR_H

#include <unordered_map>
#include <memory>

#include "../include/TBB/tbb/mutex.h"
#include "../include/TBB/tbb/concurrent_hash_map.h"

#include "Slab.h"

typedef tbb::concurrent_hash_map<std::string, Slab*> SlabMap;

class SlabAllocator
{
private:
    SlabMap m_slabs;

public:
    SlabAllocator();
    ~SlabAllocator();

    /*
    Creates a new and empty Slab for this SlabAllocator with the
    given name.
    */
    bool createSlab(std::string name, TypeList fields);

    /*
    Destroys the Slab identified by the given name.
    */
    bool destroySlab(const std::string name);

    /*
    Returns an interator to the beginning of the SlabAllocator's Slabs.
    */
    auto begin()->SlabMap::iterator;

    /*
    Returns an interator to the beginning of the SlabAllocator's Slabs.
    */
    auto begin() const ->SlabMap::const_iterator;

    /*
    Returns an interator to the end of the SlabAllocator's Slabs.
    */
    auto end()->decltype(m_slabs.end());

    /*
    Returns an interator to the end of the SlabAllocator's Slabs.
    */
    auto end() const->SlabMap::const_iterator;

    /*
    Finds a Slab with the given name.
    Returns true if found, false otherwise.
    Fills in 'result' with the Slab, if found.
    */
    auto find(std::string slabName, Slab** result)->bool;
};

#endif