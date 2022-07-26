/**
 * @file zs0_demoTaskSetup.ino
 * @author Sam Kaminetzky
 * @author Zichao Chen
 * @date   10-June-2022
 * @brief  Create tasks in RT-OS for lab 4. 
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
 */

/**
 * @brief Create tasks in RT-OS for lab 4
 */
void demoTaskSetup() {
  /*
  xTaskCreate(
    TaskBlink
    ,  "Blink"    //A name just for humans
    ,  128   //This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2   //Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

  xTaskCreate(
    TaskAnalogRead
    ,  "AnalogRead"
    ,  128   //Stack size
    ,  NULL
    ,  1   //Priority
    ,  NULL );
  */

  xTaskCreate(
    Task_RT1
    ,  "FlashLED"
    ,  128   //Stack size
    ,  NULL
    ,  1   //Priority
    ,  NULL );

  xTaskCreate(
    Task_RT2
    ,  "Song"
    ,  128   //Stack size
    ,  NULL
    ,  1   //Priority
    ,  NULL );

  xTaskCreate(
    RT3p0
    ,  "R3 Scheduler"
    ,  1100   //Stack size
    ,  NULL
    ,  3   //Priority
    ,  NULL );
}
