/**
 * @file zz1_outputHandler.ino
 * @author Sam Kaminetzky
 * @author Zichao Chen
 * @date   10-June-2022
 * @brief Drive the motor of the fan and send the relevant information to the LCD display.
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
 *   @brief Drive the motor of the fan and send the relevant information to the 
 *          LCD display based on the data from the capacitive sensor and temperature 
 *          sensor
 *   @param[in] \*pvParameters the parameter pointer value
 */
void outputHandler(void *pvParameters){
  const int MOTOR_FORWARD = 9, MOTOR_BACKWARD = 11;  // motor pins
  // Let the motor pins send PWM power to the fan
  pinMode(MOTOR_FORWARD, OUTPUT); 
  pinMode(MOTOR_BACKWARD, OUTPUT);

  double val = 0;   // value of the capacitive sensor reading
  /* Boolean values determining if at temp (amOn), 
     if ready to run based on capacitive sensor data (amReady),
     cap sensor is HIGH (sensorPressed),
     button was released (sensorReleased)
     
     Used for making a state machine-like button system for the capacitive sensor*/
  boolean amOn = 0, amReady = 1, sensorPressed = 0, sensorReleased = 0;

  //Initialize queue to send data to the LCD screen about fan speed
  lcd_Queue = xQueueCreate(1, sizeof(double));
  
  //loop
  for(;;){
    //Get data from capacitive sensor about desired fan speed
    xQueueReceive(capSense_Queue, &val, 0);

    //Get data from temp sensor about if it's hot enough to run the fan
    xQueueReceive(readyTemp_Queue, &amOn, 0);

    //If sensor is touched with bare skin / has high enough capacitance (low enough fan speed), toggle fan on and off
    if(val < 50 && amReady){
      amReady = 0;
      sensorPressed = 1;
      Serial.println("Button pressed! Off.");
    } else if(!sensorReleased && sensorPressed && val > 100){
      sensorReleased = 1;
      sensorPressed = 0;
      Serial.println("Button released! Off.");
    } else if(!sensorPressed && val < 50){
      Serial.println("Button pressed! Almost on.");
      sensorPressed = 1;
    } else if(sensorPressed && sensorReleased && val > 100){
      Serial.println("Button released! ON.");
      sensorPressed = 0;
      sensorReleased = 0;
      amReady = 1;
    }

    //If temp is high enough, and in ready state from cap sensor, run the fan at desired speed
    if (amOn && amReady){
      analogWrite(MOTOR_FORWARD, val);
      analogWrite(MOTOR_BACKWARD,0);

      //Tell LCD to display fan speed
      xQueueSendToBack(lcd_Queue, &val, 0);
    } else { //Else, turn fan off
      analogWrite(MOTOR_FORWARD, 0);
      analogWrite(MOTOR_BACKWARD,0);

      //Tell LCD to display 0 fan speed (state of amReady is 0)
      xQueueSendToBack(lcd_Queue, &amReady, 0);
    }
    vTaskDelay(2);
  }
}
