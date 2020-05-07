
#include "Graphic.h"

uint32_t triggerCount, mil;
uint64_t gTick = 0;
uint64_t startMeasureTime = 0, measureFreqCounter, triggerTime;
uint32_t period;
int uTick = 0, globalTic;
uint16_t freq = 0, q;
float f2 = 0.0;
uint16_t measureFreqInterval = 200; //0.1 sec Interval

// Interrupt setup:
const uint16_t isr_cnt =  100;     // interrupt rate 100us= 10khz; 50us = 20khz; 25us = 40KHz
const uint32_t ISR_RATE = 10000;   // interrupt rate 10000Hz = 10 pulses per ms = 100us per isr

int lifeRate = 499;

const int clockPin = PB4;
bool start = true, life;
  
tuner_note tn;

void setup() {
  pinMode(PC13, OUTPUT); //  Onboard LED
  initMTable(); // octave limit table
  initISR();
  initGraphic();

  pinMode(clockPin, INPUT_PULLDOWN);
  attachInterrupt(clockPin, TriggerReceived, FALLING); //LOW, FALLING, RISING, CHANGE
  Serial.begin(115200);
}


  
void loop() {

 
  if (millis() > 500 && start) {
    start = false;
    startMeasureTime = 0;
    triggerCount = 0;
  }
  else {
    
   uint16_t diff = millis() - startMeasureTime;
   if (diff >= measureFreqInterval) {  
      //freq = triggerCount;
      //freq = 1000000 / period;
      if (period > 0) {
        f2 = 1000000.0 / (float) period;
        tn = getTuneResult(f2);
      }
      else {
        f2 = -1;
        tn.freq = f2;
      }
      period = 0;
      triggerCount = 0;
      requestToUpdate = true;
      startMeasureTime =  millis();
      
   }
   updateDisplay();
   
  }
}

void initISR() {
  /** 
   *  Setup Timer 2 to 25u (40000 per second)
   */
  Timer2.setMode(TIMER_CH1, TIMER_OUTPUTCOMPARE);
  Timer2.setPeriod(isr_cnt);                          // in microseconds (100u = 10kHz)
  Timer2.setCompare(TIMER_CH1, 1);                   
  Timer2.attachInterrupt(TIMER_CH1, handler_Synth);   // in this routine the synth moves through the wave-table
}

/**
 * Here we measure one period of the trigger signal
 */
void TriggerReceived() {
  uint64_t actual = micros();
  period = actual - triggerTime;
  triggerTime = actual;
}

void handler_Synth(void) {
  gTick++;   // count up all the time at 10khz
  globalTic++;            
  if (globalTic > 9) {
    globalTic = 0;        // 10 tics at 10 khz is one millisec.
    // one millisecond has passed--------------
    mil++;                // increase millisecond tic, used for button debounce etc.
     // Lifesign of interrupt routine, let the onboard LED blink
    uint16_t dif = mil - q;
    if (dif > lifeRate) {  // Lifesign routine 499
      q = mil;
      life = !life;
      digitalWrite(PC13, life); // Lifesign
    }
  }
}

/**
   * Update the display
   */
  void updateDisplay() {
   switch (page) {
    case TUNER: // tuner page, shown on start-up
      if (requestToUpdate) {
       displayPage1(tn);
      }
      break;
   }
   if (requestToUpdate) {
     requestToUpdate = false;
     display.display();
   }
  }

  

  
