//  Copyright (C) 2010 Georg Kaindl
//  http://gkaindl.com
//
//  This file is part of Arduino EthernetDNS.
//
//  EthernetDNS is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as
//  published by the Free Software Foundation, either version 3 of
//  the License, or (at your option) any later version.
//
//  EthernetDNS is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with EthernetDNS. If not, see
//  <http://www.gnu.org/licenses/>.
//

//  Illustrates how to do non-blocking DNS queries via polling.

#include <Ethernet.h>
#include <EthernetDNS.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };



const char* ip_to_str(const uint8_t*);

void setup()
{
    Serial.begin(9600);

    Serial.println("Attempting to obtain a DHCP lease...");

    // Initiate a DHCP session. The argument is the MAC (hardware) address that
    // you want your Ethernet shield to use. This call will block until a DHCP
    // lease has been obtained. The request will be periodically resent until
    // a lease is granted, but if there is no DHCP server on the network or if
    // the server fails to respond, this call will block forever.
    // Thus, you can alternatively use polling mode to check whether a DHCP
    // lease has been obtained, so that you can react if the server does not
    // respond (see the PollingDHCP example).
    EthernetDHCP.begin(mac);

    // Since we're here, it means that we now have a DHCP lease, so we print
    // out some information.
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

    EthernetDNS.setDNSServer(dnsAddr);
    
    
  Serial.println("Enter a host name via the Arduino Serial Monitor to have it "
                 "resolved.");

void loop()
{ 
  char hostName[512];
  int length = 0;
  
  while (Serial.available()) {
    hostName[length] = Serial.read();
    length = (length+1) % 512;
    delay(1);
  }
  hostName[length] = '\0';
  
  if (length > 0) {
    
    byte ipAddr[4];
    
    Serial.print("Resolving ");
    Serial.print(hostName);
    Serial.print("...");
    
    // Let's send our DNS query. If anything other than DNSSuccess is returned,
    // an error has occurred. A full list of possible return values is
    // available in EthernetDNS.h
    DNSError err = EthernetDNS.sendDNSQuery(hostName);

    if (DNSSuccess == err) {
      do {
        // This will not wait for a reply, but return immediately if no reply
        // is available yet. In this case, the return value is DNSTryLater.
        // We can use this behavior to go on with our sketch while the DNS
        // server and network are busy finishing our request, rather than
        // being blocked and waiting.
        err = EthernetDNS.pollDNSReply(ipAddr);
			
        if (DNSTryLater == err) {
          // You could do real stuff here, or go on with a your loop(). I'm
          // just printing some dots to signal that the query is being
          // processed.
          delay(20);
          Serial.print(".");
        }
      } while (DNSTryLater == err);
    }

    Serial.println();

    // Finally, we have a result. We're just handling the most common errors
    // here (success, timed out, not found) and just print others as an
    // integer. A full listing of possible errors codes is available in
    // EthernetDNS.h
    if (DNSSuccess == err) {
      Serial.print("The IP address is ");
      Serial.print(ip_to_str(ipAddr));
      Serial.println(".");
    } else if (DNSTimedOut == err) {
      Serial.println("Timed out.");
    } else if (DNSNotFound == err) {
      Serial.println("Does not exist.");
    } else {
      Serial.print("Failed with error code ");
      Serial.print((int)err, DEC);
      Serial.println(".");
    }
  }  
}

// Just a utility function to nicely format an IP address.
const char* ip_to_str(const uint8_t* ipAddr)
{
  static char buf[16];
  sprintf(buf, "%d.%d.%d.%d\0", ipAddr[0], ipAddr[1], ipAddr[2], ipAddr[3]);
  return buf;
}
