#ifndef Yaler_h
#define Yaler_h
#include <Ethernet.h>
#include "WProgram.h"
struct outputStruct{
    int command;
    char * data;
};

enum YalerState {PRE_HS,WAITING_HS,WAITING_REQ,ERROR,SUCCESS};
enum YalerActions {DISCONNECT,REPLY,DO_NOTHING,WAIT,POKE};
class Yaler
{
    public:
        Yaler(char id[],Client);
        YalerActions tick(char input[]);
        void pokeRelay(Client client);
        void restart();
    private:
        YalerState _state;
        
        char _id[20];
        //Client _client;
};

#endif