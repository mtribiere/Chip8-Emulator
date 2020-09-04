#include <iostream>
#include <fstream>
#include "fileLoader.h"

using namespace std;

bool loadFileToRAM(string path,int baseAddress,RAM *ram){
    bool asLoaded = false;

    //Ouvrir le flux
    ifstream file (path, ios::in|ios::binary);
    
    //Si le fichier est ouvert
    if(file.is_open()){

        int offset = 0;
        char c;

        //Tant que le fichier n'est pas fini
        while (file.get(c)){
            
            //Charger dans la RAM
            ram->setMemoryAddress(baseAddress+offset,((unsigned short)c) & 0x00ff);
            offset++;
        }
        
        //S'il y a plus d'une instruction
        if(offset > 0)
            asLoaded = true;

        file.close();
    }else{
        cout << "Error while loading rom : " << path <<endl;
        exit(-1);
    }


    return asLoaded;
}