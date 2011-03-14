#ifndef SENSORS_H
#define SENSORS_H 

#define NUM_SAMPLES 67
#define ADC_CONVERSION_FACTOR .00488

#define WATTAGE_DEVICE_TYPE 1

#include "WProgram.h"




class Sensor
{
public:


private:
    
};
class WattageSensor:public Sensor
{
public:
    WattageSensor (int hub, int deviceNumber,int currentPort, int voltagePort, float cOffset, float cSF,float vOffset,float vSF);
    char * getString();
    void test();
private:
    float _rms(float * buffer, int numSamples);
    void _sample(int currentPort, int voltagePort, float *currentBuffer, float *voltageBuffer, int numSamples );
    void _samplesToUnits(int currentPort, int voltagePort, float * currentBuffer, float *voltageBuffer, int numSamples );
    float _phaseShift(float * signal1, float * signal2, int numSamples );
    float _getValue();
    float * _currentBuffer;
    float * _voltageBuffer;
    
    int _deviceNumber;
    int _currentPort;
    int _voltagePort;
    float _cOffset;
    float _cSF;
    float _vOffset;
    float _vSF;
    
    int _hub;
    
    
};


    

class ContactClosure:public Sensor
{
public:
    ContactClosure (int hub, int port,int deviceNumber);
    char * getString();

private:
    bool _getValue();
    int _port;
    int _deviceNumber;
    int _hub;
};


class Counter: public Sensor
{
public:
    Counter (int hub, int port,int deviceNumber,char eepromAddress);
    char * getString();
    int readInt(char eepromAddress);
    void writeInt(char eepromAddress, int data);
private:
    int _getValue();
    bool _state;
    char _eepromAddress;
    int _value;
    int _port;
    int _deviceNumber;
    int _hub;
};

class Thermistor:public Sensor
{
public:
    
    Thermistor(int hub,int port, int deviceNumber, long a=0.0285292, long b=-0.004253, long c=0.0000162878,int resistor=10000);
    char * getString();

private:
    long _getValue();
    
    int _port;
    int _deviceNumber;
    long _a;
    long _b;
    long _c;
    long _resistor;
    int _hub;
};

/*
class Log:public Sensor
{
public:
    
    Thermistor(int hub);
    char * getString( char * data);

private:
    int _hub;
};
*/
#endif