/*
 Adapted from http://www.arduino.cc/en/Tutorial/WebClientRepeating
 // for these relays, HIGH = on, LOW = off
 */

#include <SPI.h>
#include <Ethernet.h>

// assign a MAC address for the ethernet controller.
// fill in your address here:
byte mac[] = {
  0xA8, 0x61, 0x0A, 0xAE, 0x4E, 0x7D
};
// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 0, 177);
IPAddress myDns(192, 168, 0, 1);

// initialize the library instance:
EthernetClient client;

char server[] = "www.powerplant.ericajewell.com";  // also change the Host line in httpRequest()
//IPAddress server(64,131,82,241);

unsigned long lastConnectionTime = 0;           // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 3*1000;  // delay between updates, in milliseconds

int visitornum;
int relay1, relay2, relay3, relay4, relay5, relay6, relay7, relay8, relay9;
int lightsSet1[9] = {2, 6, 9, A1, A0, 7, 8, 3, 5};
int lightsSet2[9] = {A0, 3, 2, 7, 5, 6, A1, 8, 9};
int lightsSet3[9] = {9, A1, 8, 2, A0, 7, 3, 6, 5};
bool set1 = true;
bool set2 = false;
bool set3 = false;

String incomingData, readString1;
int x = 0;
char lf = 10;

void setup() {
  // Disable SD SPI
  pinMode(4,OUTPUT);
  digitalWrite(4,HIGH);
  // Set up relay pins
  pinMode(A0, OUTPUT);
  digitalWrite(A0, LOW);
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  pinMode(3, OUTPUT);
  digitalWrite(3, LOW);
  pinMode(A1, OUTPUT);
  digitalWrite(A1, LOW);
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  pinMode(6, OUTPUT);
  digitalWrite(6, LOW);
  pinMode(7, OUTPUT);
  digitalWrite(7, LOW);
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);
  pinMode(9, OUTPUT);
  digitalWrite(9, LOW);
  resetRandomness(); // set random groups
  
  // start serial port:
//  Serial.begin(9600);
//  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
//  }

  // start the Ethernet connection:
//  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
//    Serial.println("Failed to configure Ethernet using DHCP");
    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
//      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      while (true) {
        delay(1); // do nothing, no point running without Ethernet hardware
      }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
//      Serial.println("Ethernet cable is not connected.");
    }
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip, myDns);
//    Serial.print("My IP address: ");
//    Serial.println(Ethernet.localIP());
  } else {
//    Serial.print("  DHCP assigned IP ");
//    Serial.println(Ethernet.localIP());
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
}

void loop() {
  incomingData = ""; // clear the string to store incoming data
  x = 0; // clear the line counter
  while(client.connected() && !client.available()) delay(1); //waits for data
  while (client.connected() || client.available()) { //connected or data available
    char c = client.read(); //gets byte from ethernet buffer
//    Serial.print(c); //prints byte to serial monitor 
    if (c == lf){ // if c is a new line (10),
      x = (x + 1); // add 1 to the tally of lines (x)
    }
    if (x == 9){ // if we are on the ninth line
      incomingData += c; // add these characters into a string
      // how do we only read to the end and not the next buffer?
    }
    visitornum = incomingData.toInt(); 
  }
//  Serial.println(visitornum);
 
  if (visitornum == 0){
    digitalWrite(relay1, LOW);
    digitalWrite(relay2, LOW);
    digitalWrite(relay3, LOW);
    digitalWrite(relay4, LOW);
    digitalWrite(relay5, LOW);
    digitalWrite(relay6, LOW);
    digitalWrite(relay7, LOW);
    digitalWrite(relay8, LOW);
    digitalWrite(relay9, LOW);
    // reset random groups
    resetRandomness(); // maybe make a condition for when visitornum changes from something to zero to trigger this
  }

  if (visitornum == 1){
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, HIGH);
    digitalWrite(relay4, LOW);
    digitalWrite(relay5, LOW);
    digitalWrite(relay6, LOW);
    digitalWrite(relay7, LOW);
    digitalWrite(relay8, LOW);
    digitalWrite(relay9, LOW);
  }

  if (visitornum == 2){
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, HIGH);
    digitalWrite(relay4, HIGH);
    digitalWrite(relay5, HIGH);
    digitalWrite(relay6, HIGH);
    digitalWrite(relay7, LOW);
    digitalWrite(relay8, LOW);
    digitalWrite(relay9, LOW);
  }

  if (visitornum >= 3){
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, HIGH);
    digitalWrite(relay4, HIGH);
    digitalWrite(relay5, HIGH);
    digitalWrite(relay6, HIGH);
    digitalWrite(relay7, HIGH);
    digitalWrite(relay8, HIGH);
    digitalWrite(relay9, HIGH);
  }


  // if ten seconds have passed since your last connection,
  // then connect again and send data:
  if (millis() - lastConnectionTime > postingInterval) {
    httpRequest();
  }

}

// this method makes a HTTP connection to the server:
void httpRequest() {
  // close any connection before send a new request.
  // This will free the socket on the WiFi shield
  client.stop();

  // if there's a successful connection:
  if (client.connect(server, 80)) {
//    Serial.println("connecting...");
    // send the HTTP GET request:
    client.println("GET /visitornum.txt HTTP/1.1");
    client.println("Host: www.powerplant.ericajewell.com");
    client.println("User-Agent: arduino-ethernet");
    client.println("Connection: close");
    client.println();

    // note the time that the connection was made:
    lastConnectionTime = millis();
  } else {
    // if you couldn't make a connection:
//    Serial.println("connection failed");
  }
}

void resetRandomness(){
  if (set1 == true){
    relay1 = lightsSet1[0];
    relay2 = lightsSet1[1];
    relay3 = lightsSet1[2];
    relay4 = lightsSet1[3]; 
    relay5 = lightsSet1[4]; 
    relay6 = lightsSet1[5]; 
    relay7 = lightsSet1[6]; 
    relay8 = lightsSet1[7];
    relay9 = lightsSet1[8];
    set1 = false;
    set2 = true;
  }
  else if (set2 == true){
    relay1 = lightsSet2[0];
    relay2 = lightsSet2[1];
    relay3 = lightsSet2[2];
    relay4 = lightsSet2[3]; 
    relay5 = lightsSet2[4]; 
    relay6 = lightsSet2[5]; 
    relay7 = lightsSet2[6]; 
    relay8 = lightsSet2[7];
    relay9 = lightsSet2[8];
    set2 = false;
    set3 = true;
  }
  else if (set3 == true){
    relay1 = lightsSet3[0];
    relay2 = lightsSet3[1];
    relay3 = lightsSet3[2];
    relay4 = lightsSet3[3]; 
    relay5 = lightsSet3[4]; 
    relay6 = lightsSet3[5]; 
    relay7 = lightsSet3[6]; 
    relay8 = lightsSet3[7];
    relay9 = lightsSet3[8];
    set3 = false;
    set1 = true;
  }
}
