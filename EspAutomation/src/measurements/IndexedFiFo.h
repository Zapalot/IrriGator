#pragma once
#include "Measurements.h"
#define FiFoEntryType Measurement

template <class EntryType>
class IndexedFiFo
{
public:
    void initialize(uint32_t bufferSize);
    ~IndexedFiFo();
    void pushEntry(EntryType newEntry);                                                    // push a new Entry into the Ring Buffer, discard oldest Entry if it is full
    EntryType getEntry(uint32_t index) const;                                              // index counts upwards from oldest (0) to newest ()entry in the buffer
    uint32_t getNumEntriesInBuffer() const { return std::min(totalPushCount, capacity); }; // number of entries available in the Buffer
    uint32_t getPushCount() const { return totalPushCount; };                              // how many times have entries been pushed in total?
    bool willWarpOnNextPush() const { return nextNewEntryIndex == capacity ; };        //  true when last Entry is at end of Buffer, useful for processing Data in chunks
protected:
    uint32_t getLookUpIndex(uint32_t index) const;
    EntryType *buffer = nullptr;
    uint32_t nextNewEntryIndex = 0;
    uint32_t oldestEntryIndex = 0;
    uint32_t totalPushCount = 0;
    uint32_t capacity = 0;
};
template <class EntryType>
void IndexedFiFo<EntryType>::initialize(uint32_t bufferSize)
{
    capacity = bufferSize;
    if (buffer != nullptr)
        delete[] buffer;
    buffer = new EntryType[capacity];
    nextNewEntryIndex = 0;
    oldestEntryIndex = 0;
    totalPushCount = 0;
}
template <class EntryType>
IndexedFiFo<EntryType>::~IndexedFiFo<EntryType>()
{
    if (buffer != nullptr)
        delete[] buffer;
}

template <class EntryType>
// push a new Entry into the Ring Buffer, discard oldest Entry if it is full
void IndexedFiFo<EntryType>::pushEntry(EntryType newEntry)
{
    totalPushCount++;
    if (nextNewEntryIndex == capacity) // warp around
        nextNewEntryIndex = 0;

    buffer[nextNewEntryIndex] = newEntry;
    nextNewEntryIndex++;

    if (totalPushCount > capacity)
        oldestEntryIndex++;           // start forgetting old entries once buffer is full
    if (oldestEntryIndex == capacity) // warp around
        oldestEntryIndex = 0;
}

template <class EntryType>
EntryType IndexedFiFo<EntryType>::getEntry(uint32_t index) const
{
    if (index < getNumEntriesInBuffer() && index >= 0)
    {
        return (buffer[getLookUpIndex(index)]);
    }
    else
        return EntryType();
}

template <class EntryType>
uint32_t IndexedFiFo<EntryType>::getLookUpIndex(uint32_t index) const
{
    uint32_t lookupIndex = oldestEntryIndex + index;
    while (lookupIndex >= capacity)
        lookupIndex -= capacity;
    return lookupIndex;
}