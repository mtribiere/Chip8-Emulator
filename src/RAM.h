#ifndef RAM_H
#define RAM_H

class RAM{
    int size = 4096;
    unsigned short int *memory;

    public:
        RAM();
        RAM(int);

        void setMemoryAddress (int,unsigned short);
        unsigned short getMemoryAddress (int);

        void printRAM(void);

        ~RAM();

};

#endif