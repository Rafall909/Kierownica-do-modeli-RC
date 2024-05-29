
#include <stdint.h>
#include <SPI.h>
#include "printf.h"
#include "RF24.h"
#define CE_PIN 9
#define CSN_PIN 8

RF24 radio(CE_PIN, CSN_PIN);

// adresy do sparowania
uint8_t address[][6] = { "1Node", "2Node" };

bool radioNumber = 0;  // 0 uses address[0] to transmit, 1 uses address[1] to transmit

uint32_t dane;

void stworzdane ();

void setup() 
  {
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);

    Serial.begin(115200);

    radio.setPALevel(RF24_PA_LOW);

    radio.setPayloadSize(sizeof(dane));

    radio.openWritingPipe(address[radioNumber]);

    radio.openReadingPipe(1, address[!radioNumber]);

    radio.stopListening();
  }

void loop() 
  {
    stworzdane();

    unsigned long start_timer = micros();

    bool report = radio.write(&dane, sizeof(uint32_t));

    unsigned long end_timer = micros();

    if (report) 
      {
        Serial.print(F("Transmisja pomyslna! "));
        Serial.print(F("Czas przesylu = "));
        Serial.print(end_timer - start_timer);  // print the timer result
        Serial.print(F(" us. Wyslano: "));
        Serial.println(dane);  // print payload sent
      } 
      else 
        {
          Serial.println(F("Transmisja nieudana lub czas minal"));  // payload was not delivered
        }
  }  // loop


void stworzdane()
  {
    uint32_t skret = analogRead(A0);
    uint32_t gaz = analogRead(A2);
    uint32_t hamulec = analogRead(A1);
    dane = skret;
    dane |= gaz<<10;
    dane|= hamulec<<20;
    Serial.print((" Dane pobrane: "));
    Serial.print(dane, BIN);

  }