 #include "MainMemory.h"
 #include <array>
 #include <iostream>
 #include <string>
 #include <iomanip>
 #include <cstring>

 MainMemory::MainMemory(){
    memory = std::make_unique<std::array<uint8_t, MAIN_MEMORY_SIZE>>();
    memory.get()->fill(0x00);
 }

 MainMemory::~MainMemory(){
    memory.reset();
 }

 void MainMemory::Read(uint32_t startAddress, uint8_t size, uint8_t* destination){
    std::cout << "Reading from main memory (address: 0x"
              << std::hex << startAddress << std::dec << ")" << std::endl;
    std::memcpy(destination, &memory.get()->at(startAddress), size);
 }

 void MainMemory::Write(uint32_t startAddress, uint8_t size, uint8_t* source){
    std::memcpy(&memory.get()->at(startAddress), source, size);
    std::cout << "Writing to main memory (address: 0x"
              << std::hex << startAddress << std::dec << ")" << std::endl;
 }

 void MainMemory::Print(){
    uint32_t ROW = 24;
    uint8_t COL = 12;
    for(uint32_t i = 0; i < ROW; i++){
        for(uint8_t j = 0; j < COL; j++){
            uint8_t value = memory.get()->at(i * COL + j);
            std::cout << "0x" << std::hex << static_cast<int>(value) << "\t";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    for(uint32_t dash = 0; dash < 80; dash++){
        std::cout << "-";
    }
    std::cout << std::endl;
 }