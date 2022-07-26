/**
 * @file zz2_DHT_Handler.ino
 * @author Sam Kaminetzky
 * @author Zichao Chen
 * @date   10-June-2022
 * @brief Read the value of the temperature and humidity sensor, and use that to determine if the fan 
 *        should be turned on or off. 
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
 *   @brief Read the value of the temperature and humidity sensor. If the temperature is 
 *          above a set value, signal the fan to turn on, else turn off. 
 *   @param[in] \*pvParameters the parameter pointer value
 */
void DHT_Handler(void *pvParameters) {
  #define DHTTYPE  DHT11  ///< type of DHT used
  const int DHTPIN = 2;   ///< DHT pin
  const int LED_PIN = 7;  ///< LED output pin
  const int TEMP_LED_PIN = 9; ///< LED output pin for testing
  DHT dht(DHTPIN, DHTTYPE); ///< New DHT sensor
  boolean fanOn = 0;  ///<  controls the fan to be on or off
  readyTemp_Queue = xQueueCreate(1, sizeof(boolean)); // Initialize queue for if hot enough for fan to run
  tempSense_Queue = xQueueCreate(1, sizeof(float)); // Initialize queue for current temp to be read by LCD screen
  pinMode(TEMP_LED_PIN, OUTPUT);
  dht.begin();  // begin the DHT
  
  for (;;) {
    //debug(__LINE__);
    vTaskDelay( 2000 / portTICK_PERIOD_MS );
    // read humidity
    float humi  = dht.readHumidity();
    // read temperature as Celsius
    float tempC = dht.readTemperature();
    // read temperature as Fahrenheit
    float tempF = dht.readTemperature(true);

    if (isnan(humi) | isnan(tempC) | isnan(tempF)) {
      Serial.println("Failed to read from DHT sensor!");
    } else {
      Serial.print("Humidity: ");
      Serial.print(humi);
      Serial.print("%");

      Serial.print("  |  "); 

      Serial.print("Temperature: ");
      Serial.print(tempC);
      Serial.print("°C ~ ");
      Serial.print(tempF);
      Serial.println("°F");
    }

    if (tempF > 80) {
      digitalWrite(TEMP_LED_PIN, HIGH);
      fanOn = 1;
    } else {
      digitalWrite(TEMP_LED_PIN, LOW);
      fanOn = 0;
    }
    // Sends whether temp is above 80F to fan controller
    xQueueSendToBack(readyTemp_Queue, &fanOn, 0);

    //Sends the current temperature in Fahrenheit to LCD handler
    xQueueSendToBack(tempSense_Queue, &tempF, 0);
    vTaskDelay(2);
  }
}
