#include <Arduino.h>
#include "main.h"
#define DMAN 1

#if DMAN
#include <DManchester.h>
#else
#include <Manchester.h>
#endif

const int pinA = 2;
const int pinB = 4;
#define LED 17

enum Mode {
    SEND,
    RECEIVE
};

Mode mode;

void setup(){
    pinMode(pinA,OUTPUT);
    pinMode(pinB,OUTPUT);
    pinMode(9,INPUT_PULLUP);

    mode = (digitalRead(9)==HIGH) ? SEND : RECEIVE;

    if(mode==SEND){
        setupTransmit();
    }
    else {
        setupReceive();
    }
    
}

void loop(){
    if(mode==SEND){
        loopTransmit();
    }
    else {
        loopReceive();
    }
}

const int bufferSize=32;
char buffer[bufferSize];
char msg[bufferSize];
int len = 0;

void setupReceive(){
    #if DMAN
    dman.setupReceive(pinA, MAN_600);
    dman.beginReceiveArray(bufferSize, (uint8_t*)buffer);
    #else
    man.setupReceive(pinA, MAN_600);
    man.beginReceiveArray(bufferSize, (uint8_t*)buffer);
    #endif
}

void setupTransmit(){
    #if DMAN
    dman.setupTransmitPwr(pinA,pinB, MAN_600);
    #else 
    man.setupTransmit(pinA, MAN_600);
    #endif
}


void loopReceive(){
    #if DMAN
    bool receiveComplete = dman.receiveComplete();
    #else
    bool receiveComplete = man.receiveComplete();
    #endif
    
    if(receiveComplete){
        if(Serial)
            Serial.println("receiving");
        digitalWrite(LED,LOW);
        len = buffer[0];
        strncpy(msg,(char*)&buffer[1],len-1);
        Serial.println(msg);
        #if DMAN
        dman.beginReceiveArray(bufferSize, (uint8_t*)buffer);
        #else
        man.beginReceiveArray(bufferSize, (uint8_t*)buffer);
        #endif
        
        digitalWrite(LED,HIGH);
        
    }
    delay(5);
}

uint8_t counter;
uint8_t datalength=2;
char message[] = "Test message";
void loopTransmit(){
    if(digitalRead(9)==HIGH){
        delay(50);
        return;
    }
    if(Serial) Serial.println("transmitting");
    int msgLen = sizeof(message);
    
    msgLen=sprintf(buffer," %s %d",message,counter++);
    buffer[0] = (uint8_t)msgLen;
    if(Serial)
        Serial.println(buffer);
    digitalWrite(LED,LOW);
    #if DMAN
    dman.transmitArray(msgLen+1, (uint8_t*)buffer);
    #else 
    man.transmitArray(msgLen+1, (uint8_t*)buffer);
    #endif
    digitalWrite(LED,HIGH);
    Serial.println();
    delay(1000);
    
}