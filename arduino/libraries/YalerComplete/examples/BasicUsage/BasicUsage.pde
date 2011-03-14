#include <Ethernet.h>
#include <YalerComplete.h>
#include <EthernetDHCP.h>



byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xE9 };
byte server[] = {18,181,2,90};
void reply(char * request,Client client);

YalerComplete myYaler("yaler-id",mac,server,2319,&reply);

void setup(){
    Serial.begin(9600);
    myYaler.begin();
    
}


void loop(){ 
    myYaler.maintain();
}


void reply(char * request,Client client){
    char * buffPtr = request;
    //do something with the request..
    //then reply 
    client.println("HTTP/1.1 200 OK");
    client.println("Connection: close");
    client.print("Content-Length: ");
    client.println(3);
    client.println("Content-Type: text/html");
    client.println();
    
   

    
   
    delay(5);
    client.flush();
}

