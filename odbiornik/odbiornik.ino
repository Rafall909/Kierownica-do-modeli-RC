#include <stdint.h>
#include <SPI.h>
#include "printf.h"
#include "RF24.h"

#include <Servo.h>
#define CE_PIN 9
#define CSN_PIN 8


RF24 radio(CE_PIN, CSN_PIN);
Servo serwomechanizm;

uint8_t address[][6] = { "1Node", "2Node" };
bool radioNumber = 1;

uint32_t odbior;
float pozycja;

void setup() 
  {
    serwomechanizm.attach(5);

    Serial.begin(115200);

    radio.setPALevel(RF24_PA_LOW);

    radio.setPayloadSize(sizeof(uint32_t));

    radio.openWritingPipe(address[radioNumber]); 

    radio.openReadingPipe(1, address[!radioNumber]);
    
    radio.startListening();
  }

void loop() 
  {
    uint8_t pipe;
    if (radio.available(&pipe)) 
      {
      uint8_t bytes = radio.getPayloadSize();  // get the size of the payload
      radio.read(&odbior, bytes);
      Serial.print(F(" Otrzymano "));
      Serial.print(bytes);
      Serial.print(F(" bajtow: "));
      Serial.println(odbior);  // print the payload's value
      obliczpozycje();
      }
  }


void obliczpozycje()
  {
    uint32_t skret = (odbior & 0b00000000000000000000001111111111);
    uint32_t gaz = ((odbior & 0b00000000000011111111110000000000) >> 10);
    uint32_t hamulec = ((odbior & 0b00111111111100000000000000000000) >> 20);
    Serial.print((" Skret: "));
    Serial.print(skret, BIN);
    Serial.print((" Gaz: "));
    Serial.print(gaz, BIN);
    Serial.print((" Hamulec: "));
    Serial.print(hamulec, BIN);
    Serial.print((" Dane pobrane: "));
    Serial.print(odbior, BIN);
    if(skret>=500 && skret<=535) { pozycja = 90;}
    if(skret<535) { pozycja = (90.0-90.0*((skret-535.0)/(1023.0-535.0)));}
    if(skret>500) { pozycja = (90.0+90.0*((500.0-skret)/(500.0)));}
    serwomechanizm.write(pozycja);
  }