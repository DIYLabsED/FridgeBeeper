#include "Arduino.h"
#include "avr/sleep.h"       // sleep
#include "avr/interrupt.h"   // wake up

const int BUZZER_PIN = 4;
const int SENSOR_PIN = 2;    // Not expected to be changed, PB2 is hard-coded when setting up registers

volatile bool sensorState = true; // true = door open, false = door closed

void beep1(); // *click*
void beep2(); // *beep*
void beep3(); // *BEEP BEEP*

void setup(){

    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(SENSOR_PIN, INPUT_PULLUP);    

    GIMSK |= (1 << PCIE);                 // Enable pin change interrupt, General Interrupt Mask
    PCMSK |= (1 << PCINT2);               // Enable pin change interrupt on PB2, Pin Change Mask
    sei();                                // Enable global interrupts

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);

}

void loop(){
    
    if(sensorState){
        beep1();    
    }

    else{
        sleep_mode();        
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