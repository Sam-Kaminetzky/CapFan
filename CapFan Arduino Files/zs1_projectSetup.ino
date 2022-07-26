/**
 * @file zs1_projectSetup.ino
 * @author Sam Kaminetzky
 * @author Zichao Chen
 * @date   10-June-2022
 * @brief  Create all the tasks for the project. 
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
 * @brief Create all the tasks for the project
 */
void projectSetup(){
  xTaskCreate(
  outputHandler
    ,  "Fan Handler"
    ,  1000  // Stack size
    ,  NULL
    ,  3  // Priority
    ,  NULL );
    
  xTaskCreate(
    capSenseHandler
    ,  "Capacitive Sensor Handler"
    ,  1000 // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL );

  xTaskCreate(
    lcdHandler
    ,  "LCD Screen Handler"
    ,  700  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL );
  
  xTaskCreate(
    Task_RT1
    ,  "Flash LED"
    ,  128  // Stack size
    ,  NULL
    ,  3  // Priority
    ,  NULL );

  xTaskCreate(
    DHT_Handler
    ,  "Temperature Sensor"
    ,  512  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL );
}
