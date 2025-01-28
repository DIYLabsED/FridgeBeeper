#include "Arduino.h"                    
#include "avr/sleep.h"                   // sleep
#include "avr/interrupt.h"               // wake up

const int BUZZER_PIN = 4;
const int SENSOR_PIN = 2;                // not expected to be changed, PB2 is hard-coded when setting up registers

volatile bool sensorState = true;        // true = door open, false = door closed

const uint16_t STAGE1_INTERVAL = 60000;  // 1 minute
const uint16_t STAGE2_INTERVAL = 60000; // 5 minutes
int beepState = 0;
uint16_t prevMillis = 0;

void beep1();                            // *click*
void beep2();                            // *beep*
void beep3();                            // *BEEP BEEP*
void handleBeep();


void setup(){

    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(SENSOR_PIN, INPUT_PULLUP);    

    GIMSK |= (1 << PCIE);                // enable pin change interrupt, General Interrupt Mask
    PCMSK |= (1 << PCINT2);              // enable pin change interrupt on PB2, Pin Change Mask
    sei();                               // enable global interrupts

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);

}

void loop(){
    
    if(sensorState){

      handleBeep();

    }

    else{
        sleep_mode();                    // sleeping conveniently resets millis()    
    }

}

// This interrupt fires if a pin change occurs on port B
ISR(PCINT0_vect) { 

    sensorState = digitalRead(SENSOR_PIN);

}

void beep1(){ 

    digitalWrite(BUZZER_PIN, HIGH);
    delay(5);
    digitalWrite(BUZZER_PIN, LOW);
    delay(1500);

}

void beep2(){ 

    digitalWrite(BUZZER_PIN, HIGH);
    delay(20);
    digitalWrite(BUZZER_PIN, LOW);
    delay(1500);

}

void beep3(){

    digitalWrite(BUZZER_PIN, HIGH);
    delay(40);
    digitalWrite(BUZZER_PIN, LOW);

    delay(200);

    digitalWrite(BUZZER_PIN, HIGH);
    delay(40);
    digitalWrite(BUZZER_PIN, LOW);

    delay(1000);

}

void handleBeep(){

  switch (beepState) {
    case 0:
      if (millis() - prevMillis >= STAGE1_INTERVAL) {
        beepState = 1; // Update state to 1 after interval1
        prevMillis = millis(); // Reset the reference time
      } else {
        beep1();
      }
      break;

    case 1:
      if (millis() - prevMillis >= STAGE2_INTERVAL) {
        beepState = 2; // Update state to 2 after interval2
        prevMillis = millis(); // Reset the reference time
      } else {
        beep2();
      }
      break;

    case 2:
      beep3();
      // No need to update state as beep3() is the final function
      break;

    default:
      // Do nothing
      break;
  }

}