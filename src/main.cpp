#include <iostream>
#include <csignal>
#include "RAM.h"
#include "fileLoader.h"
#include "screen.h"
#include "CPU.h"

using namespace std;

bool exitFlag = false;

void OnQuitSignal(int num){
    cout << "\n<== Exit signal received (" << num << ")" << endl;
    exitFlag = true;
}

int main(int argc,char *argv[]){

    //Si moins de 2 arguments
    if(argc < 2){
        cout << "Usage : " << argv[0] << " <ROM Path> \n";
        return 1; 
    }

    //Creation de la RAM
    RAM ram;
    loadFileToRAM(argv[1],0x200,&ram);
    //ram.printRAM();

    //Creation de l'écran
    Screen screen; 

    //Preparer le signal de sortie
    signal(SIGINT,OnQuitSignal);

    //Creation du CPU
    CPU cpu;
    while(!cpu.getRomEndFlag() && !exitFlag){
        cpu.printCPU();
        cpu.RunNextInstruction(&ram,&screen);
        cout << "\n\n";
    }

    //SDL_Delay(4000);
    
    //Demarrer l'émulation
    cout << "Hello RAM 0x000 : "<< ram.getMemoryAddress(0x000) <<"\n";
    //ram.printRAM();

    return 0;
}