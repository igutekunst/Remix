#include "Output.h"

Output::Output (int hub, int deviceNumber,int port){
    _hub = hub;
    _deviceNumber = deviceNumber;
    _port = port;
}
/*
    TODO need a getString method similar to sensors
*/
void Output::setValue(int value){
    
    _set(value);
}

char * Output::getString(){
    char outputString[25];
    int value = 0;
    sprintf(outputString,"%d#%d#102#%d",_hub,_deviceNumber,value);
    int checksum = strlen(outputString) + strlen(outputString)/10 + 1;
   
    char * output =  (char * ) malloc(sizeof(char)*checksum + 1);
    sprintf(output,"%s#%d",outputString,checksum);
    
    return output;      
}



int Output::getDeviceNumber(){
    return _deviceNumber;
}
void Output::_set(int value){
    pinMode(_port,OUTPUT);
    analogWrite(_port,value);
    
    /*
        TODO should be digtial write sometimes
    */
}