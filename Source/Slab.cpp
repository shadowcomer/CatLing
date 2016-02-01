#include "Slab.h"
#include <assert.h>

using namespace SlabTypes;

Slab::Slab(TypeList const fields) :
m_fields(fields),
m_fieldsVec(generateFieldsVector(m_fields))
{

}

Slab::~Slab()
{

}

auto Slab::generateFieldsVector(TypeList fields)->TypeVec
{
    TypeVec vec;
    for each(auto elem in m_fields)
    {
        vec.push_back(elem.second);
    }

    return vec;
}

auto Slab::isCompatible(Entry entry)->bool
{
    if (entry.size() != m_fieldsVec.size())
    {
        return false;
    }

    for (size_t i = 0; i < m_fieldsVec.size(); i++)
    {
        if (!isSameType(entry[i], m_fieldsVec[i]))
            return false;
    }

    return true;
}

auto Slab::appendEntry(Entry entry)->bool
{
    if (!isCompatible(entry))
    {
        return false;
    }

    {
        tbb::mutex::scoped_lock lock(SYNC_operation);
        m_entries.push_back(entry);
    }
    return true;
}

auto Slab::removeEntry(size_t i)->bool
{
    {
        tbb::mutex::scoped_lock lock(SYNC_operation);

        if (i < 0 ||
            i > (m_entries.size() - 1)){
            return false;
        }

        auto it = m_entries.begin();
        std::advance(it, i);
        m_entries.erase(it);
    }
    return true;
}

auto Slab::modifyEntry(size_t i, size_t j, TypeObj* val)->bool
{
    if (val == nullptr)
        return false;

    // Check if 'j' is a valid field offset
    if (j < 0 ||
        j >(m_fieldsVec.size() - 1)){
        return false;
    }

    // Make sure it's the correct type
    if (val->type != m_fieldsVec[j]->type){
        return false;
    }

    {
        tbb::mutex::scoped_lock lock(SYNC_operation);

        // Check if 'i' is a valid entry offset
        if (i < 0 ||
            i >(m_entries.size() - 1)){
            return false;
        }

        auto entry = m_entries.begin();
        std::advance(entry, i);
        auto entryField = (*entry)[j];

        IntType* iParamObj = nullptr;
        IntType* iTargetObj = nullptr;

        BoolType* bParamObj = nullptr;
        BoolType* bTargetObj = nullptr;

        FloatType* fParamObj = nullptr;
        FloatType* fTargetObj = nullptr;

        StringType* sParamObj = nullptr;
        StringType* sTargetObj = nullptr;

        UnitType* uParamObj = nullptr;
        UnitType* uTargetObj = nullptr;

        switch (val->type)
        {
        case Type::INT:
            iParamObj = val->toInt();
            iTargetObj = entryField->toInt();

            assert(iParamObj != nullptr);
            assert(iTargetObj != nullptr);

            iTargetObj->value = iParamObj->value;
            break;

        case Type::BOOL:
            bParamObj = val->toBool();
            bTargetObj = entryField->toBool();

            assert(bParamObj != nullptr);
            assert(bTargetObj != nullptr);

            bTargetObj->value = bParamObj->value;
            break;

        case Type::FLOAT:
            fParamObj = val->toFloat();
            fTargetObj = entryField->toFloat();

            assert(fParamObj != nullptr);
            assert(fTargetObj != nullptr);

            fTargetObj->value = fParamObj->value;
            break;

        case Type::STRING:
            sParamObj = val->toString();
            sTargetObj = entryField->toString();

            assert(sParamObj != nullptr);
            assert(sTargetObj != nullptr);

            sTargetObj->value = sParamObj->value;
            break;

        case Type::UNIT:
            uParamObj = val->toUnit();
            uTargetObj = entryField->toUnit();

            assert(uParamObj != nullptr);
            assert(uTargetObj != nullptr);

            uTargetObj->value = uParamObj->value;
            break;

        default:
            assert(false);
            return false;
        }
    }

    return true;
}

auto Slab::discover()->TypeList const
    { return m_fields; }
