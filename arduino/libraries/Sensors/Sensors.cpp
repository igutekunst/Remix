
#include "Sensors.h"

#include <math.h>
#include <EEPROM.h>



WattageSensor::WattageSensor (int hub, int deviceNumber,int currentPort, int voltagePort, float cOffset, float cSF,float vOffset,float vSF){
    _hub = hub;
    _deviceNumber = deviceNumber;
    _currentPort = currentPort;
    _voltagePort = voltagePort;
    _cOffset = cOffset;
    _cSF = cSF;
    _vOffset = vOffset;
    _vSF = vSF;
   
}

void WattageSensor::test(){
    
    Serial.println("OMG");
}
float WattageSensor::_getValue(){
    /*
        TODO seems to work
    */
    _currentBuffer = (float *) malloc(sizeof(float)*NUM_SAMPLES);
    _voltageBuffer = (float *) malloc(sizeof(float)*NUM_SAMPLES);
    _sample(_currentPort,_voltagePort,_currentBuffer,_voltageBuffer,NUM_SAMPLES);
    _samplesToUnits(_currentPort,_voltagePort,_currentBuffer,_voltageBuffer,NUM_SAMPLES);
    float voltage = 120.0;//_rms(_voltageBuffer,NUM_SAMPLES);
    float current = _rms(_currentBuffer,NUM_SAMPLES);
    /*
        TODO add power factor correction
    */
    
    free(_currentBuffer);
    free(_voltageBuffer);
    return voltage*current;
            
}




char * WattageSensor::getString(){
    char outputString[15];
    int value = (int) _getValue();
    sprintf(outputString,"%d#%d#1#%d",_hub,_deviceNumber,value);
    int checksum = strlen(outputString) + strlen(outputString)/10 + 1;
   
    char * output =  (char * ) malloc(sizeof(char)*checksum + 1);
    sprintf(output,"%s#%d",outputString,checksum);
    
    return output;
}



/*
* Samples two adc pins alternating between the two until NUM_SAMPLES is is reached
*/
void WattageSensor::_sample(int currentPort, int voltagePort, float currentBuffer[], float voltageBuffer[],int numSamples){
    for(int i = 0; i < numSamples; i++)
    {
        currentBuffer[i] = (float) analogRead(currentPort);
        voltageBuffer[i] = (float) analogRead(voltagePort);
    }
}

/**
* Converts two arrays of raw adc values into voltages and currents using current and voltage scaling factors
* which are stored in two other arrays.
* 
*/
void WattageSensor::_samplesToUnits(int currentPort, int voltagePort, float currentBuffer[], float voltageBuffer[], int numSamples ){
    for(int i = 0; i < NUM_SAMPLES; i++)
       {
            currentBuffer[i] *= ADC_CONVERSION_FACTOR;
            currentBuffer[i] -= _cOffset;
            currentBuffer[i] *= _cSF;
            
            
            voltageBuffer[i] *= ADC_CONVERSION_FACTOR;
            voltageBuffer[i] -= _vOffset;
            voltageBuffer[i] *= _vSF;
       }
    
}

/*
* Calculagte the RMS value of the the samples
* = Sqrt(sum(every value squared)/number_of_samples)
* http://en.wikipedia.org/wiki/Root_mean_square
*/
float WattageSensor::_rms(float buffer[], int numSamples){
    float sum = 0;
    for(int  i = 0; i < numSamples; i++)
    {
        sum += buffer[i] * buffer[i];
    }
    
    sum /= numSamples;
 
    return sqrt(sum);
}

/*
* Calculates the phase shift in radians between two signals... usually nice sine waves
* If signal 1 is leading, then should be positive
* If signal 2 is leading, then should be negative
*/ 
float WattageSensor::_phaseShift(float signal1[], float signal2[], int numSamples ){
    
}

ContactClosure::ContactClosure(int hub, int port, int deviceNumber){
    _hub = hub;
    _port = port;
    _deviceNumber = deviceNumber;
}
char * ContactClosure::getString(){
    char outputString[15];
    int value = (int) _getValue();
    sprintf(outputString,"%d#%d#3#%d",_hub,_deviceNumber,value);
    int checksum = strlen(outputString) + strlen(outputString)/10 + 1;
   
    char * output =  (char * ) malloc(sizeof(char)*checksum + 1);
    sprintf(output,"%s#%d",outputString,checksum);
    
    return output;
}

bool ContactClosure::_getValue(){
    pinMode(_port,INPUT);
    int data =  analogRead(_port);
    if(data > 500)
    {
        return 1;
    }
    else{
        return 0;
    }
}


Thermistor::Thermistor(int hub, int port, int deviceNumber, long a, long b, long c,int resistor){
    _hub = hub;
    _port = port;
    _deviceNumber= deviceNumber;
    _a = a;
    _b = b;
    _c = c;
    _resistor = resistor;
    
}




char * Thermistor::getString(){
    char outputString[15];
    int value = (int) _getValue();
    sprintf(outputString,"%d#%d#2#%d",_hub,_deviceNumber,value);
    int checksum = strlen(outputString) + strlen(outputString)/10 + 1;
   
    char * output =  (char * ) malloc(20);
    sprintf(output,"%s#%d",outputString,checksum);
    
    return output;
}


long Thermistor::_getValue(){
    int RawADC = analogRead(2);
   // long Resistance;  double Temp;  // Dual-Purpose variable to save space.
   // Resistance=((10240000/RawADC) - 10000);  // Assuming a 10k Thermistor.  Calculation is actually: Resistance = (1024 * BalanceResistor/ADC) - BalanceResistor
   // Temp = log(Resistance); // Saving the Log(resistance) so not to calculate it 4 times later. // "Temp" means "Temporary" on this line.
   //  Temp = 1 / (0.001129148 + (0.000234125 * Temp) + (0.0000000876741 * Temp * Temp * Temp));    // Now it means both "Temporary" and "Temperature"
   // Temp = Temp - 273.15;  // Convert Kelvin to Celsius
   // Serial.println(Temp);
    return RawADC;
    
}

Counter::Counter(int hub,int port, int deviceNumber, char eepromAddress){
    _hub = hub;
    _port = port;
    _deviceNumber = deviceNumber;
    _eepromAddress = eepromAddress;
    _value = readInt(_eepromAddress);
}

char * Counter::getString(){
    char outputString[15];
    int value = (int) _getValue();
    sprintf(outputString,"%d#%d#2#%d",_hub,_deviceNumber,value);
                        
                          
    int checksum = strlen(outputString) + strlen(outputString)/10 + 1;
   
    char * output =  (char * ) malloc(sizeof(char)*checksum + 1);
    sprintf(output,"%s#%d",outputString,checksum);
    
    return output;
}

int Counter::readInt(char eepromAddress){
    int out;
    char * outPtr = (char *) &out;
    *outPtr++ = EEPROM.read(eepromAddress);
    *outPtr = EEPROM.read(eepromAddress+1);
    return out;
}
void Counter::writeInt(char eepromAddress,int data){

    char * outPtr = (char *) &data;
    EEPROM.write(eepromAddress,*outPtr++);
    EEPROM.write(eepromAddress+1,*outPtr);
}
int Counter::_getValue(){
    pinMode(_port,INPUT);
    bool pin = digitalRead(_port);
   
    if(_state==true)
    {
        if(pin)
        {
            return _value; 
        }else{
            
            _value++;
            writeInt(_eepromAddress,_value);
            _state = false;
            return _value;
        }
    }else{
        if(pin)
        {
            
            _value++;
            writeInt(_eepromAddress,_value);

            _state = true;
            return _value;
        }else{
            return _value;
        }
    }
    
}
