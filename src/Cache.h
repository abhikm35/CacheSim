#pragma once

#include <cstdint>
#include <array>
#include <cmath>
#include "MainMemory.h"
#include "ReplacementAlgorithms.h"

const uint8_t  MEMORY_ADDRESS_SIZE        = 32;
const uint8_t  CACHE_LINE_SIZE            = 64;
const uint32_t CACHE_SETS                 = 64;
const uint8_t  CACHE_WAYS                 = 4;
const uint8_t  CACHE_LINE_BYTE_OFFSET     = 6;
const uint8_t  CACHE_LINE_SET_INDEX_SIZE  = 6;
const uint8_t  CACHE_LINE_TAG_SIZE        = 20;

struct CacheLine {
   uint32_t tag = 0;
   std::array<uint8_t, CACHE_LINE_SIZE> data{};
   bool valid = false;
};

struct AddressParts {
   uint32_t tag = 0;
   uint8_t setIndex = 0;
   uint8_t byteOffset = 0;

   explicit AddressParts(uint32_t address) {
       byteOffset = address & (CACHE_LINE_SIZE - 1);
       setIndex = (address >> CACHE_LINE_BYTE_OFFSET) & ((1u << CACHE_LINE_SET_INDEX_SIZE) - 1u);
       tag = address >> (CACHE_LINE_BYTE_OFFSET + CACHE_LINE_SET_INDEX_SIZE);
   }
};

struct CacheStatistics {
   uint32_t cacheAccesses;
   uint32_t cacheHits;
   
   float_t GetHitRate() {
      return (static_cast<float_t>(cacheHits) /(cacheAccesses));
   }
};

class CacheSet {
public:
   CacheSet();
   ~CacheSet();

   CacheLine* Find(uint32_t tag);
   CacheLine* Replace(uint32_t tag, uint8_t* sourceData);

private:
   std::array<CacheLine, CACHE_WAYS> lines{};
   RandomReplacement replacement;
};

class Cache {
public:
   void Initialize(MainMemory* mainMemory);
   uint32_t Read(uint32_t address);
   CacheStatistics GetStatistics() const;
   void ResetStatistics();
   void Write(uint32_t address, uint8_t data);

private:
   std::array<CacheSet, CACHE_SETS> sets{};
   MainMemory* mainMemory = nullptr;
   CacheStatistics statistics;
};