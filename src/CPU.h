#ifndef CPU_H
#define CPU_H

#include <iostream>
#include <stack>  
#include <SDL.h>
#include <thread>
#include <mutex>
#include "RAM.h"
#include "screen.h"
#include "TimedRegister.h"

#define REGISTER_COUNT 16

class CPU{

    unsigned char *registers;
    int PCRegister;
    int IRegister;
    TimedRegister timeRegister;

    std::stack<int> cpuStack;

    bool romEndFlag = false;
    const int keyMapping[16] = {SDL_SCANCODE_KP_0,SDL_SCANCODE_KP_1,SDL_SCANCODE_KP_2,SDL_SCANCODE_KP_3,SDL_SCANCODE_KP_4,SDL_SCANCODE_KP_5,SDL_SCANCODE_KP_6,SDL_SCANCODE_KP_7,SDL_SCANCODE_KP_8,SDL_SCANCODE_KP_9,SDL_SCANCODE_Q,SDL_SCANCODE_B,SDL_SCANCODE_C,SDL_SCANCODE_D,SDL_SCANCODE_E,SDL_SCANCODE_F};

    public:
        CPU(void);
        void RunNextInstruction(RAM *,Screen *);

        bool getRomEndFlag(void);
        void printCPU(void);
        ~CPU(void);


    private:
        short getRegisterValue(int);
        void setRegisterValue(int,short);

};

#endif