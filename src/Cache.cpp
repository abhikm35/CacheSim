 #include "Cache.h"
 #include <string>
 #include <iostream>
 #include <iomanip>
 #include <cstring>

CacheSet::CacheSet(){
    replacement.SetWays(CACHE_WAYS);
}
CacheSet::~CacheSet(){
    replacement.SetWays(0);
}
CacheLine *CacheSet::Find(uint32_t tag){
    for(uint8_t i = 0; i < CACHE_WAYS; i++){
        if(lines[i].tag == tag && lines[i].valid){
            return &lines[i];
        }
    }
    return nullptr;
}
CacheLine* CacheSet::Replace(uint32_t tag, uint8_t* sourceData){
    uint8_t victim = replacement.GetVictim();
    lines[victim].tag = tag;
    lines[victim].valid = true;
    std::memcpy(lines[victim].data.data(), sourceData, CACHE_LINE_SIZE);
    return &lines[victim];
}
void Cache::Initialize(MainMemory* mainMemory){
    this->mainMemory = mainMemory;
}
uint32_t Cache::Read(uint32_t address){
    AddressParts addressParts(address);
    CacheLine* line = sets[addressParts.setIndex].Find(addressParts.tag);
    statistics.cacheAccesses++;
    if(line) {
        std::cout << "Reading from cache (address: 0x"
                  << std::hex << address
                  << ", set: " << std::dec << static_cast<int>(addressParts.setIndex)
                  << ", tag: 0x" << std::hex << addressParts.tag
                  << ")" << std::dec << std::endl;
        uint32_t value = 0;
        std::memcpy(&value, &line->data[addressParts.byteOffset], sizeof(uint32_t));
        statistics.cacheHits++;
        return value;
    }
    else{
        uint32_t lineStart = address & ~(CACHE_LINE_SIZE - 1);
        std::array<uint8_t, CACHE_LINE_SIZE> buffer;
        mainMemory->Read(lineStart, CACHE_LINE_SIZE, buffer.data());
        CacheLine* newLine = sets[addressParts.setIndex].Replace(addressParts.tag, buffer.data());
        uint32_t value = 0;
        std::memcpy(&value, &newLine->data[addressParts.byteOffset], sizeof(uint32_t));
        return value;

    }
}
void Cache::Write(uint32_t address, uint8_t data){
    AddressParts addressParts(address);
    CacheLine* line = sets[addressParts.setIndex].Find(addressParts.tag);
    statistics.cacheAccesses++;
    if(line) {
        statistics.cacheHits++;
        std::cout << "Writing to cache (address: 0x"
                  << std::hex << address
                  << ", set: " << std::dec << static_cast<int>(addressParts.setIndex)
                  << ", tag: 0x" << std::hex << addressParts.tag
                  << ")" << std::dec << std::endl;
        *reinterpret_cast<uint32_t*>(&line->data[addressParts.byteOffset]) = data;
    }
    mainMemory->Write(address, sizeof(uint32_t), reinterpret_cast<uint8_t*>(&data));
}
CacheStatistics Cache::GetStatistics() const{
    return statistics;
}
void Cache::ResetStatistics(){
    statistics.cacheAccesses = 0;
    statistics.cacheHits = 0;
}