/**
*@author Isaac Gutekunst
* Based off yaler.org
* Not sure what licencing applies here, so use anything you want. Must attriute to me though.
* A formal licence will be written eventually..
*/
#include "WProgram.h"
#include "Yaler.h"
#include <Ethernet.h>

Yaler::Yaler(char id[], Client  client)
{
    //set yaler id
    strcpy(_id,id);
    _state = PRE_HS;
    //_client = client;

}


void Yaler::restart(){
    _state = PRE_HS;
    
}

YalerActions Yaler::tick(char input[]){
    switch(_state){
        case PRE_HS:
                /*
                    TODO find out how to call self...
                */
                     
                _state = WAITING_HS;
                return POKE;

        break;
        case WAITING_HS:
           if(strncmp(input,"HTTP/1.1 101 Switching Protocols\r\n",25)==0){
                _state = WAITING_REQ;
                return DO_NOTHING;
            }
            else if(strncmp(input,"HTTP/1.1 204 No Connection\r\n",12)==0){
                _state = PRE_HS;
                return WAIT;
            }
            else{
                _state = WAITING_HS ;
                return DO_NOTHING;
            }
        break;
        case WAITING_REQ:
            if(strncmp(input,"GET",3)==0){
                _state = SUCCESS;
                return REPLY;
            }
            else{
                _state = WAITING_REQ;
                
                return DO_NOTHING;
            }
        break;
        case ERROR:
            _state = ERROR;
            return DISCONNECT;
        break;
        case SUCCESS:
            _state = ERROR;
            return DISCONNECT;
        break;
    
    }
}


void Yaler::pokeRelay(Client client){
    //Serial.println("Connected");
    
    /*
        TODO either make this use sprintf or see if it will work in two parts
        needs to read from _id to use correct id
        also use Yaler's client
    */
    client.print("POST /snix HTTP/1.1\r\n");
    client.print("Upgrade: PTTH/1.0\r\n");
    client.print("Connection: Upgrade\r\n");
    client.print("Host: arduino.serveftp.org\r\n\r\n");
}
