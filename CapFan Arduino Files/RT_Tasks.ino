/**
 * @file RT_Tasks.ino
 * @author Sam Kaminetzky
 * @author Zichao Chen
 * @date   10-June-2022
 * @brief  Lab 4: real-time preemptive operating system: RT-OS
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
 
const int SAMPLES = 256;  ///< number of samples used to compute the FFT
double randomArray[SAMPLES];  ///< an array that stores the randomly generated doubles for the FFT

  QueueHandle_t FFT_Queue;   ///< queue for holding the passed in randomly generated doubles for the FFT
  QueueHandle_t COMPLETE_Queue;  ///< queue for holding the value that signals the completion of the FFT computation
  double vImag[SAMPLES];    ///< array that holds the imaginary values for the FFT 
  double vReal[SAMPLES];    ///< array that holds the real values for the FFT 
  
  boolean complete = 0;     ///< value that signals the completion of the FFT computation
  
  arduinoFFT FFT = arduinoFFT();  ///< Create FFT object

/**  
 *   @brief Turns on an LED on for 250ms, then off for 100ms, repeatedly.
 *   @param[in] \*pvParameters the parameter pointer value
 */
void TaskBlink(void *pvParameters){ 
  
/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, LEONARDO, MEGA, and ZERO 
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN takes care 
  of use the correct LED pin whatever is the board used.
  
  The MICRO does not have a LED_BUILTIN available. For the MICRO board please substitute
  the LED_BUILTIN definition with either LED_BUILTIN_RX or LED_BUILTIN_TX.
  e.g. pinMode(LED_BUILTIN_RX, OUTPUT); etc.
  
  If you want to know what pin the on-board LED is connected to on your Arduino model, check
  the Technical Specs of your board  at https://www.arduino.cc/en/Main/Products
  
  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
  
  modified 2 Sep 2016
  by Arturo Guadalupi
*/

  pinMode(LED_BUILTIN, OUTPUT); // initialize digital LED_BUILTIN on pin 13 as an output.

  for (;;) // A Task shall never return or exit.
  {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    vTaskDelay( 250 / portTICK_PERIOD_MS ); // wait for 250ms
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    vTaskDelay( 100 / portTICK_PERIOD_MS ); // wait for 100ms
  }
}

/**  
 *   @brief Reads an analog input and prints the result to the serial monitor.
 *   @param[in] \*pvParameters the parameter pointer value
 */
void TaskAnalogRead(void *pvParameters){
  
/*
  AnalogReadSerial
  Reads an analog input on pin 0, prints the result to the serial monitor.
  Graphical representation is available using serial plotter (Tools > Serial Plotter menu)
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.
*/

  for (;;)
  {
    // read the input on analog pin 0:
    int sensorValue = analogRead(A7);  // read the input on pin A7
    // print out the value you read:
    Serial.println(sensorValue);
    vTaskDelay(500/portTICK_PERIOD_MS);  // 0.5 sec in between reads for stability
  }
}

/**  
 *   @brief Turns on an off board LED on for 100ms, then off for 200ms, repeatedly.
 *   @param[in] \*pvParameters the parameter pointer value
 */
void Task_RT1(void *pvParameters) {
  const int LED_PIN = 39;   // LED pin 
  pinMode(LED_PIN, OUTPUT);  // set the LED pin as output

  for (;;) // A Task shall never return or exit.
  {
    digitalWrite(LED_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    vTaskDelay( 100 / portTICK_PERIOD_MS ); // wait for 100ms
    digitalWrite(LED_PIN, LOW);    // turn the LED off by making the voltage LOW
    vTaskDelay( 200 / portTICK_PERIOD_MS ); // wait for 200ms
  }
}

/**  
 *   @brief Play "Close Encounters Theme" on the speaker 3 times, with 1.5 second of 
 *          pause in between, then stop itself
 *   @param[in] \*pvParameters the parameter pointer value
 */
void Task_RT2(void *pvParameters) {
  const int ONE_SECOND = 1000;  // 1 second
  const int MS_250 = 250;       // 250ms

  //NOTE: Must be a #define to function
  #define TOP  OCR4A  ///< Highest number hardware timer 4 will reach before resetting to 0, for freq control
  // Note frequencies' associated top values for freq control, 16MHz / freq / prescaler / 2 - 1
  const int d4 = 27302;  // 293 Hz   
  const int e4 = 24315;  // 329 Hz
  const int c4 = 30650;  // 261 Hz
  const int c3 = 61537;  // 130 Hz
  const int g3 = 40815 ; // 196 Hz
  const int R = 0;

  const long int melody[] = {d4, R, e4, R, c4, R, c3, R, g3, R};  // List of notes in the melody being played
  const int SONG_LENGTH = sizeof(melody) / sizeof(melody[0]) * MS_250;  // time length of the song
  const int TOTAL_SONG_LENGTH = SONG_LENGTH + 1.5 * ONE_SECOND;  // length of the song plus the 1.5 second pause
  
  for (;;) // A Task shall never return or exit.
  {
    int RT2_Timer;   // timer of Task_RT2
    while (millis() < 3 * TOTAL_SONG_LENGTH) {
      RT2_Timer = millis() % TOTAL_SONG_LENGTH;
      if (RT2_Timer < SONG_LENGTH) {
        TOP = melody[RT2_Timer / MS_250];
      } else if (RT2_Timer < TOTAL_SONG_LENGTH) {
        TOP = 0;
      }
    }
    TOP = 0;
    vTaskDelete(NULL);
  }
}

/**  
 *   @brief Measure the performance of an existing FFT function based on a 
 *          random signal
 *   @param[in] \*pvParameters the parameter pointer value
 */
void RT3p0(void *pvParameters) {
  for (int i = 0; i < SAMPLES; i++) {
    randomArray[i] = rand();
  }
  for (int i = 0; i < SAMPLES; i++) {
    vImag[i] = 0.0;
  }
  FFT_Queue = xQueueCreate(SAMPLES, sizeof(double));
  COMPLETE_Queue= xQueueCreate(1, sizeof(char));

  xTaskCreate(
    RT3p1
    ,  "Queue Handler"
    ,  1100  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL );
  xTaskCreate(
    RT4
    ,  "FFT Compute"
    ,  1100  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL );
    vTaskDelete(NULL);
}

/**  
 *   @brief Measure and report the "wall-clock time" elapsed for 5 FFTs
 *   @param[in] \*pvParameters the parameter pointer value
 */
void RT3p1(void *pvParameters) {
  for(;;){
    double runtime = millis();
    complete = 0;
    for (int i = 0; i < 5; i++) {
      xQueueSendToBack(FFT_Queue, &randomArray, 1000 / portTICK_PERIOD_MS);
      xQueueReceive(COMPLETE_Queue, &complete, 1000 / portTICK_PERIOD_MS);
      complete = 0;
    }
    runtime = millis() - runtime;
    Serial.print("Runtime is : ");
    Serial.println(runtime);
    //vTaskDelete(NULL);
  }
}

/**  
 *   @brief Compute the FFT and send a signal indicating that the computation is done
 *   @param[in] \*pvParameters the parameter pointer value
 */
void RT4(void *pvParameters) {
//  int counter = 0;

  for(;;){
//      if(counter >=5){
//        vTaskDelete(NULL);
//        return;
//      }
      
      xQueueReceive(FFT_Queue, vReal, 1000 / portTICK_PERIOD_MS);
      
      FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD); // Compute FFT
      complete = 1;
      xQueueSendToBack(COMPLETE_Queue, &complete, 1000 / portTICK_PERIOD_MS);
//      counter++;
  }
}
