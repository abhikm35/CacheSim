#pragma once

#include <cstdint>

#include "Cache.h"
#include "MainMemory.h"

class MemorySystem {
public:
    MemorySystem();
    ~MemorySystem();

    uint32_t Read(uint32_t address);
    void Write(uint32_t address, uint8_t data);
    void PrintMainMemory();
    CacheStatistics GetCacheStatistics() const;
    void ResetCacheStatistics();
private:
    MainMemory mainMemory;
    Cache cache;
};