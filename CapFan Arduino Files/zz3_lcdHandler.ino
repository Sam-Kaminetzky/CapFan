/**
 * @file zz3_lcdHandler.ino
 * @author Sam Kaminetzky
 * @author Zichao Chen
 * @date   10-June-2022
 * @brief display the current speed of the fan and the temperature of the environment on 
 *        the LCD screen
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
 *   @brief display the current speed of the fan and the temperature of the environment on 
 *          the LCD screen
 *   @param[in] \*pvParameters the parameter pointer value
 */
void lcdHandler(void *pvParameters){
  double val = 0; ///< Current fan speed from 0 to 255
  float tempF = 0; ///< Current temperature in fahrenheit
  const int rs = 42, en = 40, d4 = 52, d5 = 50, d6 = 48, d7 = 46; ///< Pins for LCD's inputs
  LiquidCrystal lcd(rs, en, d4, d5, d6, d7); ///< New LCD display

  //Start LCD screen with two rows, and 16 characters per row
  lcd.begin(16, 2);

  for(;;){
    //Get fan speed data from the fan handler
    xQueueReceive(lcd_Queue, &val, 0);

    //Get temperature data from the DHT handler
    xQueueReceive(tempSense_Queue, &tempF, 0);

    //First line:
    lcd.setCursor(0, 0);
    lcd.print("Spd: ");

    //Fan's speed percent
    lcd.print((val/255)*100);
    lcd.print("%        ");

    //Second line:
    lcd.setCursor(0, 1);

    //Temperature in Fahrenheit
    lcd.print("Temp : ");
    lcd.print(tempF);
    lcd.print("F     ");

    //Slow operation for easy of reading floats
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}
