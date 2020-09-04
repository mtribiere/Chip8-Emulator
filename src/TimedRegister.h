#ifndef TIMEDREGISTER_H
#define TIMEDREGISTER_H

#include <iostream>
#include <thread>
#include <mutex>
#include <SDL.h>
#include <atomic>


class TimedRegister{

    int frenquency = 60;
    unsigned char currentValue = 0;

    std::thread counterThread;
    std::mutex registerMutex;

    std::atomic<bool> exitFlag;

    public:
        TimedRegister(void);
        TimedRegister(int);

        short getTimerValue(void);
        void setTimerValue(short _value);

        ~TimedRegister(void);

    private:
        void CountDown();

};

#endif