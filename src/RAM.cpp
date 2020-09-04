#include <iostream>
#include "RAM.h"


// Constructeurs
RAM::RAM(){
    // Creer l'emplacement memoire
    memory = new unsigned short int[size];

    // Remplir la zone reservée
    unsigned short defaultSprites[] = {0xF0,0x90,0x90,0x90,0xF0, //0
                                       0x20,0x60,0x20,0x20,0x70, //1
                                       0xF0,0x10,0xF0,0x80,0xF0, //2
                                       0xF0,0x10,0xF0,0x10,0xF0, //3
                                       0x90,0x90,0xF0,0x10,0x10, //4
                                       0xF0,0x80,0xF0,0x10,0xF0, //5
                                       0xF0,0x80,0xF0,0x90,0xF0, //6
                                       0xF0,0x10,0x20,0x40,0x40, //7
                                       0xF0,0x90,0xF0,0x90,0xF0, //8
                                       0xF0,0x90,0xF0,0x10,0xF0, //9
                                       0xF0,0x90,0xF0,0x90,0x90, //A
                                       0xE0,0x90,0xE0,0x90,0xE0, //B
                                       0xF0,0x80,0x80,0x80,0xF0, //C
                                       0xE0,0x90,0x90,0x90,0xE0, //D
                                       0xF0,0x80,0xF0,0x80,0xF0, //E
                                       0xF0,0x80,0xF0,0x80,0x80  //F
                                       };    

    for(int i = 0;i<(int)(sizeof(defaultSprites)/sizeof(unsigned short));i++){
        setMemoryAddress(i,defaultSprites[i]);
    }
    
}

RAM::RAM(int _size){
    size = _size;
    RAM();
}

void RAM::setMemoryAddress(int addr, unsigned short value){
    memory[addr] = value;
}

unsigned short RAM::getMemoryAddress(int addr){
    return memory[addr];
}

void RAM::printRAM(void){
    for(int i = 0;i<size;i++){
        std::cout << i << ": " << std::hex << memory[i] << "\n";
    } 
}

// Destructeur
RAM::~RAM(){
    delete[] memory;
}