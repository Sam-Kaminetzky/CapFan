/**
 * @file zzd_debug_commands.ino
 * @author Sam Kaminetzky
 * @author Zichao Chen
 * @date   10-June-2022
 * @brief functions for debugging the code
 */

/**
 * @brief a function for debugging the code. Stops interrupts, delays 1 sec, prints line in serial
 * @param[in] line The current line number. If \_\_LINE\_\_ is entered as a parameter, this will be done automatically, though different tabs may share line numbers
 */
void debug(int line){
  noInterrupts();
  Serial.print("Reached line : ");
  Serial.println(line);
  delay(1000);
  interrupts();
}

/**
 * @brief a function for debugging the code. Stops interrupts and prints a msg in serial
 * @param[in] msg A message you want to be printed if the function is run
 */
void debugmsg(String msg){
  noInterrupts();
  Serial.println(msg);
  delay(15);
  interrupts();
}
