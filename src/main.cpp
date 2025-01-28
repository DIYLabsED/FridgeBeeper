#include "Arduino.h"                    
#include "avr/sleep.h"                   // sleep
#include "avr/interrupt.h"               // wake up

const int BUZZER_PIN = 4;
const int SENSOR_PIN = 1;                // not expected to be changed, apparently attachInterrupt() only works on INT0

volatile bool startStopwatch = true;
bool stopwatchRunning = false;
uint32_t startTime, elapsedTime;

const uint32_t BEEP1_INTERVAL = 60000;
const uint32_t BEEP2_INTERVAL = 60000;

void beep1();                            // *click*
void beep2();                            // *beep*
void beep3();                            // *BEEP BEEP*
void handleBeep();
void risingISR();
void sleep();


void setup(){

    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(SENSOR_PIN, INPUT_PULLUP);    

    attachInterrupt(digitalPinToInterrupt(SENSOR_PIN), risingISR, RISING);
                          
}

void loop(){

  if(startStopwatch && !stopwatchRunning){

    startTime = millis();
    stopwatchRunning = true;
    startStopwatch = false;

  }

  if(stopwatchRunning){

    handleBeep();

  }

  if(digitalRead(SENSOR_PIN) == LOW){
    sleep();
  }

}

void risingISR(){
  startStopwatch = true;
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

  elapsedTime = millis() - startTime;

  if(elapsedTime < BEEP1_INTERVAL){
    beep1();
  }
  else if(elapsedTime >= BEEP1_INTERVAL && elapsedTime < BEEP2_INTERVAL + BEEP1_INTERVAL){
    beep2();
  }
  else if (elapsedTime > BEEP1_INTERVAL + BEEP2_INTERVAL){
    beep3();
  }

}

void sleep() {

  stopwatchRunning = false;
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // set sleep mode to Power Down
  sleep_enable();                        // enable sleep mode
  sei();                                 // enable global interrupts
  sleep_cpu();                           // put the microcontroller to sleep
  sleep_disable();                       // disable sleep mode after waking up

}
