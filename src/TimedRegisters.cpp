#include "TimedRegister.h"

TimedRegister::TimedRegister(void){

    //Creer le thread du conteur
    std::thread toSwap(&TimedRegister::CountDown,this);
    counterThread.swap(toSwap);

    exitFlag = false;

}

void TimedRegister::CountDown(){

    while(!exitFlag){

        //Bloquer l'acces au registre
        registerMutex.lock();

        //Si besoin réduire la valeur
        if(currentValue > 0)
            currentValue--;
        
        //Liberer l'acces au registre
        registerMutex.unlock();

        //Couper le thread pour un temps 
        SDL_Delay(1000/frenquency);
    }
}

short TimedRegister::getTimerValue(void){
    
    //Bloquer l'acces au registre
    registerMutex.lock();

    short snapedCurrentValue = currentValue;

    //Liberer l'acces au registre
    registerMutex.unlock();

    return snapedCurrentValue;

}

void TimedRegister::setTimerValue(short _value){
    
    //Bloquer l'acces au registre
    registerMutex.lock();

    currentValue = _value;

    //Liberer l'acces au registre
    registerMutex.unlock();

}


TimedRegister::~TimedRegister(void){

    exitFlag = true;
    counterThread.join();

}