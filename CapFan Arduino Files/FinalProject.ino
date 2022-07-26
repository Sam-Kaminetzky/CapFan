/**
 * @file FinalProject.ino
 * @author Sam Kaminetzky
 * @author Zichao Chen
 * @date   10-June-2022
 * @brief  Lab 4: real-time preemptive operating system: RT-OS
 *         Project: Use a capacitive sensor and temperature sensor to control the spin speed of a fan, and 
 *                  control it to turn on and off. The current speed of the fan and the temperature is 
 *                  displayed on an LCD display. 
 * 
 * Lab 4 implements a real-time preemptive operating system, RT-OS, and performs several tasks based on that. 
 * 
 * 4.1: Use an analog thumbstick to generate a varying 0-5VDC positive voltage on an analog input in. Read the 
 * analog values on the Serial Monitor and observe them to change between 0-1023. In addition an LED is flashing 
 * at the same time. 
 * 
 * 4.2: 
 * Task RT-1: Flash an off-board LED on for 100ms, off for 200ms. 
 * Task RT-2: Use Timer 4 to play "Close Encounters Theme" on the speaker three times, with a pause of 1.5 second 
 * between playbacks. After that stop itself. 
 * Task RT-3: Measure the performance of an existing FFT function based on a random signal, which is done by 
 * measuring the "wall-clock time" taken by calling the FFT 5 times.
 * 
 * Project: Use a capacitive sensor and temperature sensor to control the spin speed of a fan, and 
 *          control it to turn on and off. The spin speed of the fan decreases when a hand is positioned 
 *          closer to the capacitive sensor. When the capacitive sensor is touched, the fan's on/off state will be toggled. When the temperature of the environment 
 *          is above a certain value, the fan is turned on automatically. It turns off when the temperture 
 *          is below the value. The LCD screen displays the current speed of the fan and the current 
 *          temperture of the environment.
 *          
 * External Libraries and examples used: 
 *                            Capacitive Sensor Library: https://www.arduino.cc/reference/en/libraries/capacitivesensor/,   Author: Paul Bagder, Paul Stoffregen
 *                            DHT Sensor Library :       https://www.arduino.cc/reference/en/libraries/dht-sensor-library/, Author: Adafruit
 *                            FFT Library :              https://github.com/kosme/arduinoFFT,                               Repo Owned by: Enrique Condes
 *                            
 *                            
 *              
 */

#include <CapacitiveSensor.h>
#include <Arduino_FreeRTOS.h>
#include <queue.h>
#include <LiquidCrystal.h>
#include <arduinoFFT.h>
#include <DHT.h>

//Bit 3 is 1 with 0-indexing, ie, 0b1000
const int BIT_3 = (1 << 3);

// define tasks
void TaskBlink( void *pvParameters );
void TaskAnalogRead( void *pvParameters );
void Task_RT1( void *pvParameters );
void Task_RT2( void *pvParameters );
void RT3p0(void *pvParameters);
void RT3p1(void *pvParameters);
void RT4(void *pvParameters);
void demoTaskSetup();
void projectSetup();

void setup() {
  PRR1 &= ~(1 << PRTIM4);   // start the Timer/Counter 4
  DDRH |= BIT_3;   // set pin 6 (OC4A) as output
  TCCR4A = 0;   // initialize Timer/Counter 4 Control Register A to 0
  TCCR4B = 0;   // initialize Timer/Counter 4 Control Register B to 0
  TCCR4A |= (1 << COM4A0);   // toggle OC4A on compare match
  TCCR4B |= (1 << WGM42) | (1 << CS40);  // put OC4A on CTC mode and set no prescaling
  
  Serial.begin(19200);  // initialize serial communication at 19200 bits per second

  /**
   * @brief wait for serial port to connect. Needed for native USB, on LEONARDO, MICRO, YUN, and other 32u4 based boards.
   */
  while (!Serial) {
    ; 
  } 

    //demoTaskSetup();  // Lab 4.2 demo
    projectSetup();     // Project demo
    
  vTaskStartScheduler();  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
}

void loop()
{
  // Empty. Things are done in Tasks.
}
