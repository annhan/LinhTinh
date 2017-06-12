/************************************************************************************************************************************
 * ShiftPWM blocking RGB fades example, (c) Elco Jacobs, updated August 2012.
 *
 * ShiftPWM blocking RGB fades example. This example uses simple delay loops to create fades.
 * If you want to change the fading mode based on inputs (sensors, buttons, serial), use the non-blocking example as a starting point.
 * Please go to www.elcojacobs.com/shiftpwm for documentation, fuction reference and schematics.
 * If you want to use ShiftPWM with LED strips or high power LED's, visit the shop for boards.
 ************************************************************************************************************************************/
 
// ShiftPWM uses timer1 by default. To use a different timer, before '#include <ShiftPWM.h>', add
// #define SHIFTPWM_USE_TIMER2  // for Arduino Uno and earlier (Atmega328)
// #define SHIFTPWM_USE_TIMER3  // for Arduino Micro/Leonardo (Atmega32u4)

// Clock and data pins are pins from the hardware SPI, you cannot choose them yourself if you use the hardware SPI.
// Data pin is MOSI (Uno and earlier: 11, Leonardo: ICSP 4, Mega: 51, Teensy 2.0: 2, Teensy 2.0++: 22) 
// Clock pin is SCK (Uno and earlier: 13, Leonardo: ICSP 3, Mega: 52, Teensy 2.0: 1, Teensy 2.0++: 21)

// You can choose the latch pin yourself.
//#include <FiniteStateMachine.h>
const int ShiftPWM_latchPin=8;

// ** uncomment this part to NOT use the SPI port and change the pin numbers. This is 2.5x slower **
// #define SHIFTPWM_NOSPI
// const int ShiftPWM_dataPin = 11;
// const int ShiftPWM_clockPin = 13;


// If your LED's turn on if the pin is low, set this to true, otherwise set it to false.
const bool ShiftPWM_invertOutputs = false; 

// You can enable the option below to shift the PWM phase of each shift register by 8 compared to the previous.
// This will slightly increase the interrupt load, but will prevent all PWM signals from becoming high at the same time.
// This will be a bit easier on your power supply, because the current peaks are distributed.
const bool ShiftPWM_balanceLoad = false;
#define FLIP_LIGHT_SWITCH 1
int trangthai=0;
//#define SHIFTPWM_USE_TIMER2
#include <ShiftPWM.h>   // include ShiftPWM.h after setting the pins!

// Here you set the number of brightness levels, the update frequency and the number of shift registers.
// These values affect the load of ShiftPWM.
// Choose them wisely and use the PrintInterruptLoad() function to verify your load.
// There is a calculator on my website to estimate the load.

unsigned char maxBrightness = 255;
unsigned char pwmFrequency = 75;
int numRegisters = 6;
int numRGBleds = numRegisters*8/3;
int hue = 0;
void on_trans_light_on_light_off()
{
  Serial.println("Transitioning from LIGHT_ON to LIGHT_OFF");
}
/*int stateLED[16][16]={{255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //1
                      {223,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0},//2
                      {191,223,255,0,0,0,0,0,0,0,0,0,0,0,0,0},//3
                      {159,191,223,255,0,0,0,0,0,0,0,0,0,0,0,0},//4
                      {127,159,191,223,255,0,0,0,0,0,0,0,0,0,0,0},//5
                      {95,127,159,191,223,255,0,0,0,0,0,0,0,0,0,0},//6
                      {63,95,127,159,191,223,255,0,0,0,0,0,0,0,0,0},//7
                      {31,63,95,127,159,191,223,255,0,0,0,0,0,0,0,0},//8
                      {0,31,63,95,127,159,191,223,255,0,0,0,0,0,0,0},//9
                      {0,0,31,63,95,127,159,191,223,255,0,0,0,0,0,0},//10
                      {0,0,0,31,63,95,127,159,191,223,255,0,0,0,0,0},//11
                      {0,0,0,0,31,63,95,127,159,191,223,255,0,0,0,0},//12
                      {0,0,0,0,0,31,63,95,127,159,191,223,255,0,0,0},//13
                      {0,0,0,0,0,0,31,63,95,127,159,191,223,255,0,0},//14
                      {0,0,0,0,0,0,0,31,63,95,127,159,191,223,255,0},//15
                      {0,0,0,0,0,0,0,0,31,63,95,127,159,191,223,255}};//16
                      */
 
int stateLED[24][24]={{255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //1
                      {223,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},//2
                      {191,223,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},//3
                      {159,191,223,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},//4
                      {127,159,191,223,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},//5
                      {95,127,159,191,223,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},//6
                      {63,95,127,159,191,223,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},//7
                      {31,63,95,127,159,191,223,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},//8
                      {0,31,63,95,127,159,191,223,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},//9
                      {0,0,31,63,95,127,159,191,223,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0},//10
                      {0,0,0,31,63,95,127,159,191,223,255,0,0,0,0,0,0,0,0,0,0,0,0,0},//11
                      {0,0,0,0,31,63,95,127,159,191,223,255,0,0,0,0,0,0,0,0,0,0,0,0},//12
                      {0,0,0,0,0,31,63,95,127,159,191,223,255,0,0,0,0,0,0,0,0,0,0,0},//13
                      {0,0,0,0,0,0,31,63,95,127,159,191,223,255,0,0,0,0,0,0,0,0,0,0},//14
                      {0,0,0,0,0,0,0,31,63,95,127,159,191,223,255,0,0,0,0,0,0,0,0,0},//15
                      {0,0,0,0,0,0,0,0,31,63,95,127,159,191,223,255,0,0,0,0,0,0,0,0},//16
                      {0,0,0,0,0,0,0,0,0,31,63,95,127,159,191,223,255,0,0,0,0,0,0,0},//17
                      {0,0,0,0,0,0,0,0,0,0,31,63,95,127,159,191,223,255,0,0,0,0,0,0},//18
                      {0,0,0,0,0,0,0,0,0,0,0,31,63,95,127,159,191,223,255,0,0,0,0,0},//19
                      {0,0,0,0,0,0,0,0,0,0,0,0,31,63,95,127,159,191,223,255,0,0,0,0},//20
                      {0,0,0,0,0,0,0,0,0,0,0,0,0,31,63,95,127,159,191,223,255,0,0,0},//21
                      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,31,63,95,127,159,191,223,255,0,0},//22
                      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,31,63,95,127,159,191,223,255,0},//23
                      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,31,63,95,127,159,191,223,255}};//24                      
 
/*int stateLED[16][16]={{255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //1
                      {191,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0},//2
                      {127,191,255,0,0,0,0,0,0,0,0,0,0,0,0,0},//3
                      {63,127,191,255,0,0,0,0,0,0,0,0,0,0,0,0},//4
                      {0,63,127,191,255,0,0,0,0,0,0,0,0,0,0,0},//5
                      {255,0,63,127,191,255,0,0,0,0,0,0,0,0,0,0},//6
                      {191,255,0,63,127,191,255,0,0,0,0,0,0,0,0,0},//7
                      {127,191,255,0,63,127,191,255,0,0,0,0,0,0,0,0},//8
                      {63,127,191,255,0,63,127,191,255,0,0,0,0,0,0,0},//9
                      {0,63,127,191,255,0,63,127,191,255,0,0,0,0,0,0},//10
                      {255,0,63,127,191,255,0,63,127,191,255,0,0,0,0,0},//11
                      {191,255,0,63,127,191,255,0,63,127,191,255,0,0,0,0},//12
                      {127,191,255,0,63,127,191,255,0,63,127,191,255,0,0,0},//13
                      {63,127,191,255,0,63,127,191,255,0,63,127,191,255,0,0},//14
                      {0,63,127,191,255,0,63,127,191,255,0,63,127,191,255,0},//15
                      {255,0,63,127,191,255,0,63,127,191,255,0,63,127,191,255}};//16 */ 


unsigned long previousMillis = 0;  
const long interval = 1000;
int chay;
int randNumber;
int stt=0;
byte flag=0;
void setup(){
  
  Serial.begin(9600);
  Serial.println("Begin");
  pinMode(2, INPUT);
 // attachInterrupt(2, dem, RISING);
  // Sets the number of 8-bit registers that are used.
  ShiftPWM.SetAmountOfRegisters(numRegisters);

  // SetPinGrouping allows flexibility in LED setup. 
  // If your LED's are connected like this: RRRRGGGGBBBBRRRRGGGGBBBB, use SetPinGrouping(4).
  ShiftPWM.SetPinGrouping(1); //This is the default, but I added here to demonstrate how to use the funtion
  
  ShiftPWM.Start(pwmFrequency,maxBrightness);
  ShiftPWM.SetAll(0);
        randNumber = random(10, 50);
        randNumber=randNumber+20;
 Serial.println(randNumber);
                     
/** the state machine controls which of the states get attention and execution time */

}

void loop()
{    
dem();
  if (trangthai>0) {
      if ((stt==0)||(stt==5) ||(stt==15)||(stt==randNumber)){
        
        if (chay==0){
          chay=1;
        for (int state=0; state<24; state++)
        {
          for (int led=0; led<24; led++)
          {
            //ShiftPWM.SetHSV(led,hue,255,stateLED[state][led]);
            ShiftPWM.SetOne(led,stateLED[state][led]);
            delay(6);
          }
          delay(20);
        }
        }
    
  }
  else chay=0;
  }
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval){ 
    previousMillis = currentMillis;
    if (trangthai>0){
     // Serial.println(stt);
      stt++;
      if (stt>randNumber+5){
        stt=0;
        randNumber = random(10, 90);
        randNumber=randNumber+20;
        Serial.println(randNumber);
        }
     }
    else {stt=0;}
  }

}
void dem()
{ if (digitalRead(2)==HIGH){
  delay(50);
  if (digitalRead(2)==HIGH){
    if ( flag == 0){
         trangthai++;
          if (trangthai>1){trangthai=0;}
          Serial.println(trangthai);
      flag=1; //change flag variable
    }

  }
}
else flag=0;
}
