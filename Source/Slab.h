/*
    A Slab is a container. Slabs contain a list of Entries [Entry] which
    have the same topology.

    This class is thread-safe.
*/

#ifndef SLAB_H
#define SLAB_H

#include <unordered_map>
#include <memory>

#include "../include/TBB/tbb/mutex.h"
#include "../include/TBB/tbb/concurrent_hash_map.h"

#include "Types.h"

typedef tbb::concurrent_hash_map<std::string, SlabTypes::TypeObj const * const> TypeList;
typedef std::vector<SlabTypes::TypeObj const * const > TypeVec;
typedef std::vector<SlabTypes::TypeObj*> Entry;
typedef std::vector<Entry> EntryList;

class Slab
{
private:
    // WARNING: Keep the order of the next variables consistent with
    // the constructors' initialization list order.
    TypeList const m_fields;
    TypeVec const m_fieldsVec;
    // END-WARNING

    EntryList m_entries;

    /*
    Generates an easily iterable vector. This is called during construction to
    guarantee that the TypeList is transcribed into something that will retain
    it's topology after every call; a hash map does not have this property.

    Do not call this function during the lifetime of the object.
    */
    auto generateFieldsVector(TypeList fields)->TypeVec;

    tbb::mutex SYNC_operation;

public:
    Slab(TypeList const fields);
    Slab() = delete;
    Slab(Slab&) = delete;

    ~Slab();

    /*
    Retrieves the Entry at the given position. Returns true if the
    entry was retrieved successfully, and copies the entry into
    out_entry; returns false otherwise and leaves out_entry
    unmodified.
    */
    auto getEntry(size_t pos, Entry& entry)->bool;

    /* The same as getEntry, but will also remove the Entry from
    the Slab.
    */
    auto getAndRemoveEntry(size_t pos, Entry& entry)->bool;

    /*
    Retrieves a vector that contains copies of each entry.
    */
    auto getEntries()->std::vector<Entry>;

    /*
    Checks whether the given entry is compatible with this Slab.
    Compatibility requires that each of the fields of the Entry match
    the ones that this Slab uses, in the same order.
    */
    auto isCompatible(Entry entry)->bool;

    /*
    Appends an Entry to the Slab.
    */
    auto appendEntry(Entry entry)->bool;

    /*
    Given an Entry's position, it removes it from the Slab.
    */
    auto removeEntry(size_t i)->bool;

    /*
    Given a TypeObj, it modifies the value of a given Entry's field.
    The modification is a copy of the original value.
    */
    auto modifyEntry(size_t i, size_t j, SlabTypes::TypeObj* val)->bool;

    auto discover()->TypeList const;
};

#endif