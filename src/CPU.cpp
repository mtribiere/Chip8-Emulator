#include "CPU.h"

CPU::CPU(void){
    
    //Creation des registers
    registers = new unsigned char[REGISTER_COUNT];

    for(int i = 0;i<REGISTER_COUNT;i++)
        registers[i] = 0;
    

    PCRegister = 0x200; // Point d'entrée par default
    IRegister = 0x0000;

    
}

void CPU::RunNextInstruction(RAM *ram,Screen *screen){

    //Recuperer l'instruction
    int operation = (ram->getMemoryAddress(PCRegister)<<8) + ram->getMemoryAddress(PCRegister+1);
    std::cout << "Operation : " << std::hex << operation << std::endl;

    //Flag pour le changerment manuel du PC
    bool notCountPCFlag = false;

    ///////Trouver le type de l'instruction
    //Instruction de fin
    if(operation == 0x0000){
        std::cout << "Fin de la rom" << std::endl;
        romEndFlag = true;
    }
    
    //CLS
    else if(operation == 0x00E0){
        screen->clearScreen();
    }

    //RET
    else if(operation == 0x00EE){
        PCRegister = cpuStack.top();
        cpuStack.pop();
    }

    //JP
    else if((operation & 0xF000) == 0x1000){
        PCRegister = (operation & 0x0FFF);
        notCountPCFlag = true;
    }

    //CALL
    else if((operation & 0xF000) == 0x2000){
        cpuStack.push(PCRegister);
        PCRegister = operation & 0x0FFF;
        notCountPCFlag = true;
        
    }

    //SE Vx,kk
    else if((operation & 0xF000) == 0x3000){

        int regNum = (operation & 0x0F00) >> 8;
        short compareNum = (operation & 0x00FF);
        
        if(registers[regNum] == compareNum){
            PCRegister +=2;
        }
    }

    //SNE Vx,kk
    else if((operation & 0xF000) == 0x4000){

        int regNum = (operation & 0x0F00) >> 8;
        short compareNum = (operation & 0x00FF);

        if(registers[regNum] != compareNum){
            PCRegister +=2;
        }
    }

    //SE Vx,Vy
    else if((operation & 0xF00F) == 0x5000){

        int firstReg = (operation & 0x0F00) >> 8;
        int secondReg = (operation & 0x00F0) >> 4;

        if(registers[firstReg] == registers[secondReg]){
            PCRegister +=2;
        }
    }

    //LD Vx,bytes
    else if((operation & 0xF000) == 0x6000){

        int regNum = (operation & 0x0F00) >> 8;
        registers[regNum] = (operation & 0x00FF);

    }

    //ADD Vx,kk
    else if((operation & 0xF000) == 0x7000){

        int regNum = (operation & 0x0F00) >> 8;
        short num = (operation & 0x00FF);

        registers[regNum] = (registers[regNum] + num);
    }

    //8XY-
    else if((operation & 0xF000) == 0x8000){

        int firstReg = (operation & 0x0F00) >> 8;
        int secondReg = (operation & 0x00F0) >> 4;

         //LD Vx,Vy
        if((operation & 0x000F) == 0x0000)
            registers[firstReg] = registers[secondReg];

        //OR Vx,Vy
        else if((operation & 0x000F) == 0x0001)
            registers[firstReg] |= registers[secondReg];

        //AND Vx,Vy
        else if((operation & 0x000F) == 0x0002)
            registers[firstReg] &= registers[secondReg];
        
        //XOR Vx,Vy
        else if((operation & 0x000F) == 0x0003)
            registers[firstReg] ^= registers[secondReg];

        //ADD Vx,Vy
        else if((operation & 0x000F) == 0x0004){
            registers[firstReg] += registers[secondReg];
            registers[0xF] = (registers[firstReg] + registers[secondReg] > 255 ) ? 1 : 0;
        }
        
        //SUB Vx,Vy
        else if((operation & 0x000F) == 0x0005){
            registers[firstReg] -= registers[secondReg];
            registers[0xF] = (registers[firstReg] > registers[secondReg]) ? 1 : 0;
        }
        
        //SHR Vx,Vy
        else if((operation & 0x000F) == 0x0006){
            registers[0xF] = ((registers[firstReg] & 0x0001) == 0x0001) ? 1 : 0;
            registers[firstReg] /= 2;
        }

        //SUBN Vx,Vy
        else if((operation & 0x000F) == 0x0007){
            registers[firstReg] = registers[secondReg] - registers[firstReg];
            registers[0xF] = (registers[firstReg] < registers[secondReg]) ? 1 : 0;
        }

        //SHL Vx,Vy
        else if((operation & 0x000F) == 0x000E){
            registers[0xF] = ((registers[firstReg] & 0x80) == 0x80) ? 1 : 0;
            registers[firstReg] *= 2;
        }

    }

    //SNE Vx,Vy
    else if((operation & 0xF00F) == 0x9000){
        
        int firstReg = (operation & 0x0F00) >> 8;
        int secondReg = (operation & 0x00F0) >> 4;

        if(registers[firstReg] != registers[secondReg]){
            PCRegister +=2;
        }
    }
    
    //LD I,addr
    else if((operation & 0xF000) == 0xA000){
        IRegister = operation & 0x0FFF;
    }

    //JP V0, addr
    else if((operation & 0xF000) == 0xB000){
        PCRegister = (operation & 0x0FFF) + registers[0];
    }

    //RND Vx,byte
    else if((operation & 0xF000) == 0xC000){

        int registerNum = (operation & 0x0F00) >> 8;
        int value = operation & 0x00FF;

        registers[registerNum] = (rand() % 256) & value;


    }

    //DRW Vx,Vy, nibble
    else if((operation & 0xF000) == 0xD000){
        int lineCount = operation & 0x000F;
        int firstRegister = (operation & 0x0F00) >> 8;
        int secondRegister = (operation & 0x00F0) >> 4;

        bool overridPixel = false;

        for(int i = 0;i<lineCount;i++){
           overridPixel |= screen->drawSprite(registers[firstRegister],registers[secondRegister]+i,ram->getMemoryAddress(IRegister+i));
        }

        registers[0xF] = overridPixel ? 1 : 0;
    }

    //SKP Vx
    else if((operation & 0xF0FF) == 0xE09E){

        int registerNum = (operation & 0x0F00) >> 8;
        SDL_PumpEvents();
        const Uint8 *keyState = SDL_GetKeyboardState(NULL);
       
        if(keyState[keyMapping[registers[registerNum]]]) PCRegister +=2;
    }

    //SKNP Vx
    else if((operation & 0xF0FF) == 0xE0A1){

        int registerNum = (operation & 0x0F00) >> 8;
        SDL_PumpEvents();
        const Uint8 *keyState = SDL_GetKeyboardState(NULL);
       
        if(!keyState[keyMapping[registers[registerNum]]]) PCRegister +=2;
    }

    //LD Vx,DT
    else if((operation & 0xF0FF) == 0xF007){

        int registerNum = (operation & 0x0F00)>>8;
        registers[registerNum] = timeRegister.getTimerValue();

    }

    else if((operation & 0xF0FF) == 0xF00A){


        int registerNum = (operation & 0x0F00) >> 8;
        SDL_PumpEvents();
        const Uint8 *keyState = SDL_GetKeyboardState(NULL);

        bool keyPressed = false;
        int keyPressedValue = -1;
       
       //Tant qu'une touche n'est pas pressée
        while(!keyPressed){

            SDL_PumpEvents();
            keyState = SDL_GetKeyboardState(NULL);

            for(int i = 0;i<16;i++){
               if(keyState[keyMapping[i]]){ 
                   keyPressed = true;
                   keyPressedValue = i;
               }
            }
        }   

        registers[registerNum] = keyPressedValue;
    }

    //LD DT, Vx
    else if((operation & 0xF0FF) == 0xF015){

        int registerNum = (operation & 0x0F00)>>8;
        timeRegister.setTimerValue(registers[registerNum]);

    }

    //LD ST,Vx
    else if((operation & 0xF0FF) == 0xF018){
        std::cout << "Audio channel not yet implemented" << std::endl;
    }

    //ADD I,Vx
    else if((operation & 0xF0FF) == 0xF01E){

        int registerNum = (operation & 0x0F00)>>8;
        IRegister += registers[registerNum];
       
    }
    
    //LD F, Vx
    else if((operation & 0xF0FF) == 0xF029){

        int registerNum = (operation & 0x0F00)>>8; 
        IRegister = (registers[registerNum] * 5);

    }

    //LD B,Vx
    else if((operation & 0xF0FF) == 0xF033){
        
        int registerNum = (operation & 0x0F00)>>8; 
        short toConvert = registers[registerNum];

        ram->setMemoryAddress(IRegister+2, toConvert%10);
        ram->setMemoryAddress(IRegister+1, (toConvert%100 - toConvert%10)/10);
        ram->setMemoryAddress(IRegister, (toConvert - ((toConvert%100 - toConvert%10)/10))/100);
    
    }

    //LD [I],Vx
    else if((operation & 0xF0FF) == 0xF055){

        int registerNum = (operation & 0x0F00)>>8; 
        
        for(int i = 0;i<=registerNum;i++)
            ram->setMemoryAddress(IRegister+i,registers[i]);
    }

    //LD Vx,[I]
    else if((operation & 0xF0FF) == 0xF065){

        int registerNum = (operation & 0x0F00)>>8; 
        
        for(int i = 0;i<=registerNum;i++)
            registers[i] = ram->getMemoryAddress(IRegister+i);
    }

    else{

        std::cout <<  "Unknow  instruction: " << std::hex << operation << std::endl;
        romEndFlag = true;

    }

    //Instruction suivante
    if(!romEndFlag && !notCountPCFlag){
        PCRegister += 2;
    }
    
}

bool CPU::getRomEndFlag(void){
    return romEndFlag;
}

void CPU::printCPU(void){

    std::cout << "PC : " << std::hex << PCRegister << std::endl;
    std::cout << "SP : " << std::hex << cpuStack.size() << std::endl;
    std::cout << "I : " << std::hex << IRegister << std::endl;
    std::cout << "Registers : ";
    for(int i = 0;i<REGISTER_COUNT;i++){
        std::cout << std::hex << (int)registers[i] << ";";
    }

    std::cout << std::endl;

}

CPU::~CPU(void){
    delete[] registers;
}