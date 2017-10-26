/*
 * Sam Lavery
 * All rights reserved.
*/

#include <SPI.h>
#include <Ethernet.h>
#include "Arduino.h"
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/power.h>
          
#define RELAY_ON 0
#define RELAY_OFF 1
#define LED_PIN (13)

#define RELAY_1  2  // Arduino Digital I/O pin number
#define RELAY_2  3
#define RELAY_3  4
#define RELAY_4  5
#define RELAY_5  6  
#define RELAY_6  7
#define RELAY_7  8
#define RELAY_8  9

ISR(WDT_vect)
{
        wdt_reset();
}

// The IP address will be dependent on your local network:
byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0x01, 0x09 };
IPAddress ip(10,69,69,200);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);

// Client variables 
char linebuf[80];
int charcount=0;

// Enter a MAC addre
void setup() { 
  // Relay module prepared 
  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
  pinMode(RELAY_3, OUTPUT);
  pinMode(RELAY_4, OUTPUT);
  pinMode(RELAY_5, OUTPUT);
  pinMode(RELAY_6, OUTPUT);
  pinMode(RELAY_7, OUTPUT);
  pinMode(RELAY_8, OUTPUT);
  digitalWrite(RELAY_1, RELAY_OFF);
  digitalWrite(RELAY_2, RELAY_OFF);
  digitalWrite(RELAY_3, RELAY_OFF);
  digitalWrite(RELAY_4, RELAY_OFF);
  digitalWrite(RELAY_5, RELAY_OFF);
  digitalWrite(RELAY_6, RELAY_OFF);
  digitalWrite(RELAY_7, RELAY_OFF);
  digitalWrite(RELAY_8, RELAY_OFF);
  
  // Open serial communication at a baud rate of 9600
  Serial.begin(9600); 
  Serial.println("Initialising..."); delay(100);
  pinMode(LED_PIN, OUTPUT);
  setupWatchDogTimer();
  Serial.println("Initialisation complete."); delay(100);
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}

// Enters the arduino into sleep mode.
void enterSleep(int sleep_cycles) {
  Serial.println("Entering sleep"); 
  while (sleep_cycles > 0) {
    set_sleep_mode(SLEEP_MODE_STANDBY);
    sleep_enable();
    // Now enter sleep mode.
    sleep_mode();
    // The program will continue from here after the WDT timeout
    // First thing to do is disable sleep.
    sleep_disable();
    sleep_cycles = sleep_cycles - 1;
    //Serial.println("Slept"); 
  }
  //Serial.println("Awoke"); 
  // Re-enable the peripherals.
  power_all_enable();  
}


// Setup the Watch Dog Timer (WDT)
void setupWatchDogTimer() {
  MCUSR &= ~(1<<WDRF);
  WDTCSR |= (1<<WDCE) | (1<<WDE);

  // ONE SECOND INTERVALS
  WDTCSR  = (0<<WDP3) | (1<<WDP2) | (1<<WDP1) | (0<<WDP0);
  // Enable the WD interrupt (note: no reset).
  WDTCSR |= _BV(WDIE);
}

//Likely to be replaced
void dashboardPage(EthernetClient &client) {
  client.println("<!DOCTYPE HTML><html><head>");
  client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head><body>");                                                             
  client.println("<h3>Arduino Web Server - <a href=\"/\">Refresh</a></h3>");    
  client.println("</body></html>"); 
}


void loop() {  
  //Flash the led to show how pimptastic this arduino is
  digitalWrite(LED_PIN, 1);
  delay(5);
  digitalWrite(LED_PIN, 0);
  delay(5);
  digitalWrite(LED_PIN, 1);
  delay(5);
  digitalWrite(LED_PIN, 0);

  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    memset(linebuf,0,sizeof(linebuf));
    charcount=0;
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
       char c = client.read();
       //read char by char HTTP request
        linebuf[charcount]=c;
        if (charcount<sizeof(linebuf)-1) charcount++;
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          dashboardPage(client);
          break;
        }
        //ugly, but it works        
        if (c == '\n') {
          if (strstr(linebuf,"GET /1off") > 0){
            digitalWrite(RELAY_1,  RELAY_OFF);
          }
          else if (strstr(linebuf,"GET /1short") > 0){            
            digitalWrite(RELAY_1,  RELAY_ON);
            enterSleep(1);
            digitalWrite(RELAY_1,  RELAY_OFF);
          }
          else if (strstr(linebuf,"GET /1long") > 0){            
            digitalWrite(RELAY_1,  RELAY_ON);
            enterSleep(10);
            digitalWrite(RELAY_1,  RELAY_OFF);
          }
          if (strstr(linebuf,"GET /2off") > 0){
            digitalWrite(RELAY_2,  RELAY_OFF);
          }
          else if (strstr(linebuf,"GET /2short") > 0){            
            digitalWrite(RELAY_2,  RELAY_ON);
            enterSleep(1);
            digitalWrite(RELAY_2,  RELAY_OFF);
          }
          else if (strstr(linebuf,"GET /2long") > 0){            
            digitalWrite(RELAY_2,  RELAY_ON);
            enterSleep(10);
            digitalWrite(RELAY_2,  RELAY_OFF);
          }
          if (strstr(linebuf,"GET /3off") > 0){
            digitalWrite(RELAY_3,  RELAY_OFF);
          }
          else if (strstr(linebuf,"GET /3short") > 0){            
            digitalWrite(RELAY_3,  RELAY_ON);
            enterSleep(1);
            digitalWrite(RELAY_3,  RELAY_OFF);
          }
          else if (strstr(linebuf,"GET /3long") > 0){            
            digitalWrite(RELAY_3,  RELAY_ON);
            enterSleep(10);
            digitalWrite(RELAY_3,  RELAY_OFF);
          }
          if (strstr(linebuf,"GET /4off") > 0){
            digitalWrite(RELAY_4,  RELAY_OFF);
          }
          else if (strstr(linebuf,"GET /4short") > 0){            
            digitalWrite(RELAY_4,  RELAY_ON);
            enterSleep(1);
            digitalWrite(RELAY_4,  RELAY_OFF);
          }
          else if (strstr(linebuf,"GET /4long") > 0){            
            digitalWrite(RELAY_4,  RELAY_ON);
            enterSleep(10);
            digitalWrite(RELAY_4,  RELAY_OFF);
          }
          if (strstr(linebuf,"GET /5off") > 0){
            digitalWrite(RELAY_5,  RELAY_OFF);
          }
          else if (strstr(linebuf,"GET /5short") > 0){            
            digitalWrite(RELAY_5,  RELAY_ON);
            enterSleep(1);
            digitalWrite(RELAY_5,  RELAY_OFF);
          }
          else if (strstr(linebuf,"GET /5long") > 0){            
            digitalWrite(RELAY_5,  RELAY_ON);
            enterSleep(10);
            digitalWrite(RELAY_5,  RELAY_OFF);
          }
          if (strstr(linebuf,"GET /6off") > 0){
            digitalWrite(RELAY_6,  RELAY_OFF);
          }
          else if (strstr(linebuf,"GET /6short") > 0){            
            digitalWrite(RELAY_6,  RELAY_ON);
            enterSleep(1);
            digitalWrite(RELAY_6,  RELAY_OFF);
          }
          else if (strstr(linebuf,"GET /6long") > 0){            
            digitalWrite(RELAY_6,  RELAY_ON);
            enterSleep(10);
            digitalWrite(RELAY_6,  RELAY_OFF);
          }
          if (strstr(linebuf,"GET /7off") > 0){
            digitalWrite(RELAY_7,  RELAY_OFF);
          }
          else if (strstr(linebuf,"GET /7short") > 0){            
            digitalWrite(RELAY_7,  RELAY_ON);
            enterSleep(1);
            digitalWrite(RELAY_7,  RELAY_OFF);
          }
          else if (strstr(linebuf,"GET /7long") > 0){            
            digitalWrite(RELAY_7,  RELAY_ON);
            enterSleep(10);
            digitalWrite(RELAY_7,  RELAY_OFF);
          }
          if (strstr(linebuf,"GET /8off") > 0){
            digitalWrite(RELAY_8,  RELAY_OFF);
          }
          else if (strstr(linebuf,"GET /8short") > 0){            
            digitalWrite(RELAY_8,  RELAY_ON);
            enterSleep(1);
            digitalWrite(RELAY_8,  RELAY_OFF);
          }
          else if (strstr(linebuf,"GET /8long") > 0){            
            digitalWrite(RELAY_8,  RELAY_ON);
            enterSleep(10);
            digitalWrite(RELAY_8,  RELAY_OFF);
          }

          // you're starting a new line
          currentLineIsBlank = true;
          memset(linebuf,0,sizeof(linebuf));
          charcount=0;          
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(3);
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}



