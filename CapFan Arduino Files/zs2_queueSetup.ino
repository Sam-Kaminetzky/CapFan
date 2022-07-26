/**
 * @file zs2_queueSetup.ino
 * @author Sam Kaminetzky
 * @author Zichao Chen
 * @date   10-June-2022
 * @brief  Create queues for the capacitive sensor, temperature sensor, and LCD display sensor to pass 
 *         data to the fan handler. 
 * 
 * Project: Use a capacitive sensor and temperature sensor to control the spin speed of a fan, and 
 *          control it to turn on and off. The spin speed of the fan decreases when a hand is positioned 
 *          closer to the capacitive sensor. When the capacitive sensor is touched, the fan will be turned 
 *          off if it was on, and will be turned on if it was off. When the temperature of the environment 
 *          is above a certain value, the fan is turned on automatically. It turns off when the temperture 
 *          is below the value. The LCD screen displays the current speed of the fan and the current 
 *          temperture of the environment. 
 */

QueueHandle_t capSense_Queue;   ///< queue for the capacitive sensor data
QueueHandle_t readyTemp_Queue;  ///< queue for ready signal (tells fan if it is hot enough to start running)
QueueHandle_t tempSense_Queue;  ///< queue for the temperature sensor data
QueueHandle_t lcd_Queue;        ///< queue for the LCD display data
