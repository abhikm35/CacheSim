#include "Processor.h"
#include <iostream>
#include <iomanip>
#include <string>
MemorySystem::MemorySystem(){
    cache.Initialize(&mainMemory);
}
MemorySystem::~MemorySystem(){
    cache.Initialize(nullptr);
}
uint32_t MemorySystem::Read(uint32_t address){
    return cache.Read(address);
}
void MemorySystem::Write(uint32_t address, uint8_t data){
    cache.Write(address, data);
}
void MemorySystem::PrintMainMemory(){
    mainMemory.Print();
}
CacheStatistics MemorySystem::GetCacheStatistics() const{
    return cache.GetStatistics();
}
void MemorySystem::ResetCacheStatistics(){
    cache.ResetStatistics();
}
int main(){
    MemorySystem memory;
    //fill up main memory with random data
    for (uint32_t i = 0; i < 128; i++){
        memory.Write(rand() % 0x40000, rand() % 0xFFFFFFFF);
    }
    for(uint32_t cycle = 0; cycle < 1; cycle++){
        memory.ResetCacheStatistics();
        for (uint32_t i = 0; i < 100; i++){
            memory.Read(i * 4);
        }
        float hitRatePercent = memory.GetCacheStatistics().GetHitRate() * 100;
        std::cout << "Cache hit rate: " << hitRatePercent
                  << "% (sequential read " << cycle << ")" << std::endl;
    }
    memory.PrintMainMemory();
    return 0;
}
