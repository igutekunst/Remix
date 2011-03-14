#ifndef OUTPUT_H
#define OUTPUT_H

#include "WProgram.h"
class Output
{
public:
    Output(int hub, int deviceNumber,int port);
    void setValue(int value);
    int getDeviceNumber();
    char * getString(); 

private:
    void _set(int port);
    int _hub;
    int _deviceNumber;
    int _port;
};

class Contact: public Output
{
public:
    Contact (int hub, int deviceNumber,int port);
    void setValue(int value);
private:
    /* data */
};

#endif