// arduino-cli compile --fqbn esp8266:esp8266:nodemcuv2 bed-lights.ino 
// arduino-cli upload -p /dev/cu.usbserial-14340  --fqbn esp8266:esp8266:nodemcuv2
// arduino-cli compile --fqbn esp8266:esp8266:nodemcuv2 bed-lights.ino && arduino-cli upload -p /dev/cu.usbserial-14340  --fqbn esp8266:esp8266:nodemcuv2:baud=3000000
// arduino-cli compile --fqbn esp8266:esp8266:nodemcuv2 bed-lights.ino && arduino-cli upload -p /dev/cu.usbserial-14140  --fqbn esp8266:esp8266:nodemcuv2:baud=921600

// arduino-cli compile --fqbn esp8266:esp8266:d1_mini_lite bed-lights.ino 
// arduino-cli upload -p /dev/cu.usbserial-14440  --fqbn esp8266:esp8266:d1_mini_lite:baud=3000000
// arduino-cli compile --fqbn esp8266:esp8266:d1_mini_lite bed-lights.ino && arduino-cli upload -p /dev/cu.usbserial-14340  --fqbn esp8266:esp8266:d1_mini_lite:baud=3000000
// arduino-cli compile --fqbn esp8266:esp8266:d1_mini_lite bed-lights.ino && arduino-cli upload -p /dev/cu.usbserial-14140  --fqbn esp8266:esp8266:d1_mini_lite:baud=921600


// Here's how to control the LEDs from any two pins:
//#define DATAPIN    D1
//#define CLOCKPIN   D2
//Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);
// The last parameter is optional -- this is the color data order of the
// DotStar strip, which has changed over time in different production runs.
// Your code just uses R,G,B colors, the library then reassigns as needed.
// Default is DOTSTAR_BRG, so change this if you have an earlier strip.

// Hardware SPI is a little faster, but must be wired to specific pins
// (Arduino Uno = pin 11 for data, 13 for clock, other boards are different).
// FOR D1 mini lite we use D5 (blu wire) ad D7 (green)
//stripConfig.strip = Adafruit_DotStar(stripConfig.NUMPIXELS, DOTSTAR_BGR);


#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Adafruit_DotStar.h>
#include <SPI.h>         // COMMENT OUT THIS LINE FOR GEMMA OR TRINKET
#include <arduino-timer.h>
#include "ColorConverterLib.h"
//#include <ESP8266mDNS.h>



#define LEDTYPE 'dotstar'
#define VERBOSE false
#define NUMPIXELS 144 // Number of LEDs in strip

#if LEDTYPE != 'dotstar'
    int rpin = D6;
    int gpin = D5;
    int bpin = D0;
#endif

struct wifiSetup {
     char* ssid = "your_ssid";
     char* password = "your_pass";
} wifiConfig;

struct udpSetup {
    WiFiUDP udp;
    unsigned int udpPort = 4210;
    char incomingPacket[255];
    int packetSize;
} udpConfig;

IPAddress staticIP(192, 168, 1, 232); //ESP static ip
IPAddress gateway(192, 168, 1, 1);   //IP Address of your WiFi Router (Gateway)
IPAddress subnet(255, 255, 255, 0);  //Subnet mask
IPAddress dns(8, 8, 8, 8);  //DNS

struct pirSetup {
    int PIR_lock = 0;
    int PIR_pin = D1;
    Timer<>::Task PIR_task;
    int light_on_timer = 15; 
} pirConfig;

struct stripSetup {
    Adafruit_DotStar strip=Adafruit_DotStar(NUMPIXELS, DOTSTAR_BGR);
    uint32_t previous_color;
    uint32_t current_color;
    char working_mode[15]="basic";
} stripConfig;

auto timer = timer_create_default();

// void mDNS() {

//     if (MDNS.begin("bed")) {              // Start the mDNS responder for esp8266.local
//         Serial.println("mDNS responder started");
//     } else {
//         Serial.println("Error setting up MDNS responder!");
//     }

//     MDNS.addService("strip", "udp", (int)udpConfig.udpPort);
// }

void setup() {

    Serial.begin(115200);

    #if LEDTYPE != 'dotstar'
        testSetup();
    #else
        stripConfig.strip.begin(); // Initialize pins for output
        stripConfig.strip.show();  // Turn all LEDs off ASAP
    #endif

    connectWiFi();
    //mDNS();
}

#if LEDTYPE != 'dotstar'
    void testSetup() {
        pinMode(rpin, OUTPUT);
        pinMode(gpin, OUTPUT);
        pinMode(bpin, OUTPUT);

        analogWrite(rpin, 255);
        analogWrite(gpin, 0);
        analogWrite(bpin, 0);
    }
#endif


void connectWiFi() {

  byte ledStatus = LOW;
  
  WiFi.mode( WIFI_STA );
  WiFi.config(staticIP, gateway, subnet, dns);

    // Connect WiFi
  WiFi.hostname("bed_strip");
  WiFi.begin(wifiConfig.ssid, wifiConfig.password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    digitalWrite( BUILTIN_LED, ledStatus ); // Write LED high/low.
    ledStatus = ( ledStatus == HIGH ) ? LOW : HIGH;
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println( WiFi.localIP() );
 
  // Print the IP address
  Serial.print("IP address: ");
  Serial.print(WiFi.localIP());

  udpConfig.udp.begin(udpConfig.udpPort);  
  
}


void readIncomingUdp() {
    udpConfig.packetSize = udpConfig.udp.parsePacket();
    if (udpConfig.packetSize) {
        #if VERBOSE
            Serial.printf("Received %d bytes from %s, port %d\n", udpConfig.packetSize, udpConfig.udp.remoteIP().toString().c_str(), udpConfig.udp.remotePort());
        #endif
        int len = udpConfig.udp.read(udpConfig.incomingPacket, 255);
        if (len > 0) {
            udpConfig.incomingPacket[len] = 0;
        }
    }
}

void parseUdp(char* incomingPacket) {

    if (startsWith(incomingPacket, "0x")) {

        stripConfig.previous_color = stripConfig.current_color;
        stripConfig.current_color = (long int)strtol(udpConfig.incomingPacket, NULL, 0);
    }
    else {
        #if VERBOSE 
            Serial.printf("This is a command: %s\n",incomingPacket);
        #endif

        strcpy(stripConfig.working_mode, incomingPacket);
        //When some command has to be executed only once and not in the loop
       if  (strcmp(stripConfig.working_mode,"nightmode") == 0)
            whole_strip(0x000000);
        //stripConfig.working_mode=incomingPacket;

    }

}

bool startsWith(const char *a, const char *b) {
    return (strncmp(a, b, strlen(b)) == 0) ? true : false;   
}

#if LEDTYPE != 'dotstar'
    void lightTest(uint32_t hexColor) {
        int b = hexColor % 256;
        int g = ((hexColor-b)/256) % 256;
        int r = hexColor/(256*256);

        //Serial.printf("r %d ,g %d, b %d\n", r,g,b);
        analogWrite(rpin, r);
        analogWrite(gpin, g);
        analogWrite(bpin, b);
    }
#else

    //, bool save_last_color
    void whole_strip(uint32_t color) {

        for (int i=0; i <= NUMPIXELS; i++) {
            stripConfig.strip.setPixelColor(i, color);     
        }
        
        stripConfig.strip.show();                   

        //if (save_last_color)
            //current_color = color;
    }

#endif

bool night_mode_led_off (void *) {
  whole_strip(0x000000);
  pirConfig.PIR_lock = 0;
  #if VERBOSE 
    Serial.printf("night_mode_led_off, pir enable\n");
  #endif
  return true;
}

void nightmode() { 
    if ( (digitalRead(pirConfig.PIR_pin) == HIGH) && (pirConfig.PIR_lock == 0) ) { 
        pirConfig.PIR_lock = 1;
        #if VERBOSE 
            Serial.printf("Motion detected\n");
        #endif
        if (pirConfig.PIR_task) { 
            #if VERBOSE 
                Serial.printf("restarting timer*\n");
            #endif
            timer.cancel(pirConfig.PIR_task);
        } 
        whole_strip(stripConfig.current_color);
        pirConfig.PIR_task = timer.in(pirConfig.light_on_timer * 1000, night_mode_led_off);

    }
    else if ( (digitalRead(pirConfig.PIR_pin) == LOW) && ( !pirConfig.PIR_task ) && (pirConfig.PIR_lock == 1 ) ) {

        #if VERBOSE 
            Serial.printf("unlcok the pir\n");
        #endif
        pirConfig.PIR_lock = 0;
         
    }
}


void workingModeLoop() {

    if (strcmp(stripConfig.working_mode,"rainbow") == 0)
        rainbowCycle(10);
    else if (strcmp(stripConfig.working_mode,"newkitt") == 0)
        NewKITT(0xff, 0, 0, 30, 10, 0);
    else if (strcmp(stripConfig.working_mode,"meteorrain") == 0)
        meteorRain(0xff,0xff,0xff,10, 64, true, 30);
    else if (strcmp(stripConfig.working_mode,"bouncingballs") == 0)
        BouncingBalls(0xff,0,0, 3);
    else if (strcmp(stripConfig.working_mode,"disco") == 0)
        disco(10);
    else if (strcmp(stripConfig.working_mode,"strobo") == 0)
        strobo(65);
    else if (strcmp(stripConfig.working_mode,"nightmode") == 0) 
        nightmode();
    else if (strcmp(stripConfig.working_mode,"italy") == 0) 
        italy();
    else if (strcmp(stripConfig.working_mode,"greece") == 0) 
        greece();
    else if (strcmp(stripConfig.working_mode,"germany") == 0) 
        germany();

        

}

void workingModeSingle() {

     if (strcmp(stripConfig.working_mode,"basic") == 0){

        #if LEDTYPE != 'dotstar'
            lightTest(stripConfig.current_color);
        #else
            whole_strip(stripConfig.current_color);
        #endif
    }
    else if (strcmp(stripConfig.working_mode,"crossfade") == 0)
        crossFade(stripConfig.current_color,stripConfig.previous_color);
}

void loop() {

    timer.tick();
    //MDNS.update();

    readIncomingUdp();

    if (udpConfig.packetSize) {

        #if VERBOSE 
            Serial.printf("UDP packet contents: %s\n", udpConfig.incomingPacket);
        #endif

        parseUdp(udpConfig.incomingPacket);
        workingModeSingle();

    }

    workingModeLoop();
    
    //uint32_t udp_color = (int)strtol(udpConfig.incomingPacket, NULL, 0);
    

}
