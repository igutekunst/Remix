#ifndef EE_H
#define EE_H value
#include <EEPROM.h>
#include "WProgram.h"

struct ee_pointer_t
{
    unsigned int start : 11;
    unsigned int type: 5;
};

class EE
{
public:
    EE(int);
    
    char * loadVariable(int id);
    int createVariable(int start);
    bool variableExists(int id);
    int writeVariable(int id,char * data, int length);
private:
    bool _isFree(int id);
    
};
#endif