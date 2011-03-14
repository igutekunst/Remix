#ifndef Yaler_h
#define Yaler_h
#include <Ethernet.h>
#include <EthernetDHCP.h>


#include "WProgram.h"

#include <avr/wdt.h> 
//Twelve hours in ms
#define HRS_12 43200000

#define TIMEOUT_MS 30000

enum YalerState {PRE_HS,WAITING_HS,WAITING_REQ,ERROR,SUCCESS};
enum YalerActions {DISCONNECT,REPLY,DO_NOTHING,WAIT,POKE};
class YalerComplete
{
    public:
        void begin();
        YalerComplete(char id[],uint8_t *mac,uint8_t *server,int port,void (*callback)(char *,Client));
        void maintain();
        
        
    private:
        YalerActions _tick(char input[]);
        void _startEthernet();
        void (*_reply)(char *, Client);
        void _pokeRelay();
        Client _client;
        bool _clientConnected;
        unsigned long _timeoutTimer;
        int _code;
        
        void _restart();
        
        YalerState _state;
        void _reset();
        char _buffer[50];
        char * _buffPtr;
        char _newBuffer[50];
        
        uint8_t _mac[6];
        char _id[20];
        //Client _client;
};

#endif