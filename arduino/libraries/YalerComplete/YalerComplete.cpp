/**
*@author Isaac Gutekunst
* Based off yaler.org
* Not sure what licencing applies here, so use anything you want. Must attriute to me though.
* A formal licence will be written eventually..
*/
//#define YALER_DEBUG

#include "WProgram.h"
#include "YalerComplete.h"
#include <Ethernet.h>
#include <stdlib.h>

// Just a utility function to nicely format an IP address.
const char* ip_to_str(const uint8_t* ipAddr)
{
  static char buf[16];
  sprintf(buf, "%d.%d.%d.%d\0", ipAddr[0], ipAddr[1], ipAddr[2], ipAddr[3]);
  return buf;
}


/**
* Creates a Yaler client or server, depending on how you think about it. 
* This class will obtain an IP using DHCP, register with the yaler relay and POKE it to maintain a connection
* When a client wants to make a request to the arduino, the library will call the specified callback function with the GET request. 
* Currently post requests are not handled an no headers are passed to the callback function
* @param id the yaler unique id that identifies this node
* @param mac the mac address, specified as an array of four bytes
* @param server the IP address of the yaler relay specified as an array of four bytes
* @param port the port that the yaler relay is running on. 
*/
YalerComplete::YalerComplete(char id[],uint8_t *mac,uint8_t *server,int port, void (*callback)(char *,Client) ): _client(server,port)
{
    //set yaler id
    
    _reply = callback;
    for(char i = 0; i < 6; ++i)
    {
       _mac[i] = mac[i];
    }
    
    strcpy(_id,id);
    _state = PRE_HS;
    
    
    strcpy(_buffer,"");
    _buffPtr = &_buffer[0];
    strcpy(_newBuffer,"");
    _clientConnected = false;
   
    

}


void YalerComplete::begin(){
    _startEthernet();
}


/**
* Restarts the yaler state machine to a pre handshake state.
*
*/
void YalerComplete::_restart(){
    _state = PRE_HS;
}
/**
* Performs one tick on the Yaler State Machine
* Accepts a string of the HTTP request and runs the Yaler Library
* This needs to be called once per loop to allow Yaler to maintain a connection
* The library will call your callback function specified as a paramter to the constructor 
* You should never need to call this function (well it's private...)
*/
YalerActions YalerComplete::_tick(char input[]){
    switch(_state){
        case PRE_HS:
                // In this state the Arduino is waiting to send a request to the yaler relay asking if it is ready
                // it outputs the action poke, which instructs the library to send the POST /id HTTP/1.1 request
                // the SM transitions into a state which waits for the relay's response
                _state = WAITING_HS;
                return POKE;

        break;
        case WAITING_HS:
            //In this state, the Arduino has poked the yaler realy and is awaiting a response
            
            
            //I f the Arduino receives a "HTTP/1.1 101 Switching Protocols\r\n" it transitions to wait for a request from the client browser
            // It instrcuts the library do nothing
           if(strncmp(input,"HTTP/1.1 101 Switching Protocols\r\n",25)==0){
                _state = WAITING_REQ;
                return DO_NOTHING;
            }
            
            // Otherwise, if there is no client waiting for a request, the machine returns to the original state, and commands the Arduino to wait
            // Commanding a wait indicates  that the Arduino can leave the socket open
            else if(strncmp(input,"HTTP/1.1 204 No Connection\r\n",12)==0){
                _state = PRE_HS;
                return WAIT;
            }
            
            // If the Arduino has not received anything yet, the SM will stay in the current state
            // This has the potential to become an infinte loop, so YalerComplete::maintain() has a thirty second timeout
            else{
                _state = WAITING_HS ;
                return DO_NOTHING;
            }
        break;
        case WAITING_REQ:
            // In this state, the Arduino is waiting for a request from the client browser
            if(strncmp(input,"GET",3)==0){
                //if the request begins with a GET, the library assumes that it is a valid GET request, and commands
                // the library to pass the request to the callback function
                // At this point, the library assumes that everything is ok. It is the job of the YalerComplete::maintain() funtion to asses an actual success
                _state = SUCCESS;
                return REPLY;
            }
            else{
                //Otherwise, the client should simply wait for a succesfful request
                // This is another potential timeout issue.. 
                _state = WAITING_REQ;
                
                return DO_NOTHING;
            }
        break;
        
        case ERROR:
            //currently the SM can never reach this state, but there is room for imporvement
            _state = ERROR;
            return DISCONNECT;
        break;
        case SUCCESS:
            _state = ERROR;
            return DISCONNECT;
        break;
    
    }
}


void YalerComplete::_reset(){
    cli();			// Clear interrupts
    wdt_enable(WDTO_15MS);	// Set the Watchdog to 15ms
    while(1);		// Enter an infinite loop 
    
}

/**
* Maintains the Yaler Connection
* This needs to be called once per loop to allow Yaler to maintain a connection
* The library will call your callback function specified as a paramter to the constructor
*/
void YalerComplete::maintain(){
        if(millis()>HRS_12)
        {
            Serial.println("12 hr Reset");
            //_reset();
        }


                if( _client.connected() and _clientConnected ){
                    if(_client.available()){
                        *_buffPtr = _client.read();
                        if(*_buffPtr =='\n'){
                            _buffPtr++;
                            *_buffPtr = '\0';




                            _buffPtr = &_buffer[0];
                            strcpy(_newBuffer,_buffer);
                        }else{
                            _buffPtr++; 
                        }

                    }
                    YalerActions code = _tick(_newBuffer);
                    Serial.print(_newBuffer);
                    strcpy(_newBuffer,"");
                   // enum YalerActions {DISCONNECT,REPLY,DO_NOTHING,WAIT,POKE};
                    switch(code){

                        case REPLY:
                        (*_reply)(_newBuffer,_client);
                        delay(5);
                         _restart();
                         Serial.println("Restart Yaler");
                        _clientConnected = false;

                        break;

                        case WAIT:
                        Serial.println("Waiting");

                        _clientConnected = true;
                        _timeoutTimer = millis();
                        break;

                        case DO_NOTHING:

                        //why are there two actions that both do nothing??
                        break;

                        case POKE:
                        _pokeRelay();
                        Serial.println("Poking");
                        _timeoutTimer = millis();
                        _clientConnected = true;
                        break;
                        case DISCONNECT:
                        _clientConnected = false;
                        Serial.println("Disconnecing");
                        _restart();
                        break;
                        default:
                        Serial.println("Default?");
                        //should i do something here??
                        break;

                    }

                }
                else{
                    if( !_client.connected())
                    {
                        Serial.println("Client already D'd");
                        if(_client.connect())
                        {
                           while(!_client.connected()){
                               //Connecting
                           }
                           _restart();
                           Serial.println("Client Connected");
                           _timeoutTimer = millis();
                           _clientConnected = true;
                        }else{
                            Serial.println("FUcked");
                        }

                    }



                }
            
                if(!_client.connected() or !_clientConnected){
                    Serial.println("stopping");
                    _client.flush();
                    _client.stop();

                }


        if(millis() - _timeoutTimer > TIMEOUT_MS )
        {
           _startEthernet();
           _clientConnected = true;
           _timeoutTimer = millis();
           delay(150);
        }

        EthernetDHCP.maintain();
    }



/*
    TODO make this poll instead of block.. and react if no ip is obtained.. (what the f would it do, light up an led??)
*/
void YalerComplete::_startEthernet(){
    
    Serial.println("Attempting to obtain a DHCP lease...");
      delay(150);
      // Initiate a DHCP session. The argument is the MAC (hardware) address that
      // you want your Ethernet shield to use. This call will block until a DHCP
      // lease has been obtained. The request will be periodically resent until
      // a lease is granted, but if there is no DHCP server on the network or if
      // the server fails to respond, this call will block forever.
      // Thus, you can alternatively use polling mode to check whether a DHCP
      // lease has been obtained, so that you can react if the server does not
      // respond (see the PollingDHCP example).
      
      for(char i = 0; i < 6; ++i)
      {
          Serial.println((int)_mac[i]);
      }
      EthernetDHCP.begin(_mac);
      
      

      // Since we're here, it means that we now have a DHCP lease, so we print
      // out some information.
      
      #ifdef YALER_DEBUG
      const byte* ipAddr = EthernetDHCP.ipAddress();
      const byte* gatewayAddr = EthernetDHCP.gatewayIpAddress();
      const byte* dnsAddr = EthernetDHCP.dnsIpAddress();
      
      Serial.println("A DHCP lease has been obtained.");

      Serial.print("My IP address is ");
      Serial.println(ip_to_str(ipAddr));

      Serial.print("Gateway IP address is ");
      Serial.println(ip_to_str(gatewayAddr));

      Serial.print("DNS IP address is ");
      Serial.println(ip_to_str(dnsAddr));

      //EthernetDNS.setDNSServer(dnsAddr);
      #endif
     //wait to give the WizNet time to breath.. grrrhh :)             
    delay(1500);
}

/**
* Sends an POST /_id HTTP/1.1  which essentially pokes the yaler relay and asks if a client is connected
* This function does not handle the reply, as it is handled by the state machine (YalerComplete::_tick())
*/
void YalerComplete::_pokeRelay(){
    //Serial.println("Connected");
    
    /*
        TODO either make this use sprintf or see if it will work in two parts
        needs to read from _id to use correct id
        also use Yaler's client
    */
    char * string = (char * ) malloc(sizeof(char)*((strlen(_id)+1)+18));
    sprintf(string,"POST /%s HTTP/1.1\r\n",_id);
    Serial.print(string);
    _client.print(string);
    _client.print("Upgrade: PTTH/1.0\r\n");
    _client.print("Connection: Upgrade\r\n");
    _client.print("Host: arduino.serveftp.org\r\n\r\n");
    
    free(string);
}
