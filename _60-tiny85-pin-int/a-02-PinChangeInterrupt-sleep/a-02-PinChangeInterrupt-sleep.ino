// https://github.com/NicoHood/PinChangeInterrupt/blob/master/examples/PinChangeInterrupt_LowLevel/PinChangeInterrupt_LowLevel.ino

/*
 Copyright (c) 2014-2015 NicoHood
 See the readme for credit to other people.

 PinChangeInterrupt_LowLevel
 Demonstrates how to use the library without the API

 Make sure to comment "//#define PCINT_API" in the settings file.

 To maximize speed and size also uncomment all not used pins above.
 Then you could also uncomment "#define PCINT_COMPILE_ENABLED_ISR"
 to get away the .a linkage overhead.

 Connect a button/cable to pin 7 and ground (Uno).
 Strong overwritten callback functions are called when an interrupt occurs.
 The Led state will change if the pin state does.

 PinChangeInterrupts are different than normal Interrupts.
 See readme for more information.
 Dont use Serial or delay inside interrupts!
 This library is not compatible with SoftSerial.

 The following pins are usable for PinChangeInterrupt:
 Arduino Uno/Nano/Mini: All pins are usable
 Arduino Mega: 10, 11, 12, 13, 50, 51, 52, 53, A8 (62), A9 (63), A10 (64),
               A11 (65), A12 (66), A13 (67), A14 (68), A15 (69)
 Arduino Leonardo/Micro: 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI)
 HoodLoader2: All (broken out 1-7) pins are usable
 Attiny 24/44/84: All pins are usable
 Attiny 25/45/85: All pins are usable
 Attiny 13: All pins are usable
 Attiny 441/841: All pins are usable
 ATmega644P/ATmega1284P: All pins are usable
 */





#include "PinChangeInterrupt.h"
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define adc_disable() (ADCSRA &= ~(1<<ADEN)) // disable ADC
#define adc_enable()  (ADCSRA |=  (1<<ADEN)) // re-enable ADC


// Choose a valid PinChangeInterrupt pin of your Arduino board
#define pinBlink 0
#define LED_PIN 1

void setup() {
  adc_disable();
  // set pin to input with a pullup, led to output
  pinMode(pinBlink, INPUT_PULLUP);
  //pinMode(pinBlink, INPUT);
  pinMode(LED_PIN, OUTPUT);

  attachPCINT(digitalPinToPCINT(pinBlink), blinkLed, RISING);
}

void blinkLed(void) {
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
}

void loop() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  attachPCINT(digitalPinToPCINT(pinBlink), blinkLed, RISING);
  sleep_enable();
  sei();
  sleep_cpu();

  sleep_disable();
  sei();
  detachPCINT(digitalPinToPCINT(pinBlink));
  for (int i=0; i < 10 ; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(50);
    digitalWrite(LED_PIN, LOW);
    delay(50);     
  }
}
