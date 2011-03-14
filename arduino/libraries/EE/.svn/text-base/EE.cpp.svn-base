#include "EE.h"
EE::EE(int a){
    
}

int  EE::createVariable( int size ){
    int id =1;
    while(size>0){
        if(_isFree(id)){
            size--;
        }
        
        if(size==0){
            ee_pointer_t pointer;
                char *a =(char *) &pointer;

                *a = EEPROM.read(2*id+300);
                a++;
                *a = EEPROM.read(2*id+301);
                pointer.type = 2;
                pointer.start = 2 + id;
            return id;
        }else{
            id++;
        }
    }
}

bool EE::_isFree(int i){
    
     ee_pointer_t pointer;
        char *a =(char *) &pointer;
        
        *a = EEPROM.read(2*i+300);
        a++;
        *a = EEPROM.read(2*i+301);
        
        if(pointer.type==0)
        {
            return true;
        }
        else{
            return false;
        }
}


bool EE::variableExists(int id){
    
}

int EE::writeVariable(int id, char * buffer, int length){
    ee_pointer_t pointer;
        char *a =(char *) &pointer;
        
        *a = EEPROM.read(2*id+300);
        a++;
        *a = EEPROM.read(2*id+301);
    for( int i = pointer.start; i < pointer.start+pointer.type; ++i)
    {
        EEPROM.write(i+2,*buffer);
        buffer++;
    }
}
char *  EE::loadVariable(int id){
    ee_pointer_t pointer;
    char *a =(char *) &pointer;
    *a = EEPROM.read(2*id);
    a++;
    *a = EEPROM.read(2*id+1);
    
    
    int size;
    if(pointer.type==1 or pointer.type==2)
    {
        size = 2;
    }
    else{
        size = pointer.type -2;
    }
    
    char *output = (char *) malloc(sizeof(char)* size);
    
    for(char i = 0; i < size; ++i)
    {
        *output = EEPROM.read(pointer.start + i);
        output++;
    }
    return output;
}