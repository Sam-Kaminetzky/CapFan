/**
 * @file zz0_capSenseHandler.ino
 * @author Sam Kaminetzky
 * @author Zichao Chen
 * @date   10-June-2022
 * @brief  A capacitive sensor handler that reads the value for the capacitive sensor. 
 * 
 * Project: Use a capacitive sensor and temperature sensor to control the spin speed of a fan, and 
 *          control it to turn on and off. The spin speed of the fan decreases when a hand is positioned 
 *          closer to the capacitive sensor. When the capacitive sensor is touched, the fan will be turned 
 *          off if it was on, and will be turned on if it was off. When the temperature of the environment 
 *          is above a certain value, the fan is turned on automatically. It turns off when the temperture 
 *          is below the value. The LCD screen displays the current speed of the fan and the current 
 *          temperture of the environment. 
 */

/**  
 *   @brief Read the value of the capacitive sensor and send it to the queue for 
 *          the capacitive sensor for the output handler to use
 *   @param[in] \*pvParameters the parameter pointer value
 */
void capSenseHandler(void *pvParameters){
  // Makes a new capacitive sensor
  CapacitiveSensor   cs_0_1 = CapacitiveSensor(A0,A1);

  double val;    // value of the capacitive sensor reading
  
  //Readies the queues for sending capacitor data to the output handler, and lcd handler
  capSense_Queue = xQueueCreate(1, sizeof(double));
  lcd_Queue = xQueueCreate(1, sizeof(double));

  //Recalibration should be rare for the capacitive touch sensor. Sets it to an arbitrarily high value (200 seconds)
  cs_0_1.set_CS_AutocaL_Millis(200000);
  for(;;){
    //Takes a single sample from the capacitive touch sensor
    val = cs_0_1.capacitiveSensor(1);

    //Makes this value inversely proportional to fan speed, whose max speed is 255, and min is 0
    val = (255*100)/((val+3)*3) - 60; // Biases are arbitrary experimental values that produced fan speeds between 255 and 0 when hand was near
    if(val<1) val = 0; // Floor of fan speed
    if(val>200) val = 255; // Ceiling of fan speed

    //Send capacitor values to the output handler
    xQueueSendToBack(capSense_Queue, &val, 0);

    //Wait a bit to give time for lower priority tasks
    vTaskDelay(2);
  }
}
