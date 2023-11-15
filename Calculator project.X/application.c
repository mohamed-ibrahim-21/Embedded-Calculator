/* 
 * File:   application.c
 * Author: mohamed ibrahim
 *
 * Created on October 4, 2023, 10:15 PM
 */


#include "application.h"

keypad_t keypad1 = {
  .keypad_row_pins[0].port = PORTC_INDEX,
  .keypad_row_pins[0].pin = GPIO_PIN0,
  .keypad_row_pins[0].logic = GPIO_LOW,
  .keypad_row_pins[0].direction = GPIO_DIRECTION_OUTPUT,
  .keypad_row_pins[1].port = PORTC_INDEX,
  .keypad_row_pins[1].pin = GPIO_PIN1,
  .keypad_row_pins[1].logic = GPIO_LOW,
  .keypad_row_pins[1].direction = GPIO_DIRECTION_OUTPUT,
  .keypad_row_pins[2].port = PORTC_INDEX,
  .keypad_row_pins[2].pin = GPIO_PIN2,
  .keypad_row_pins[2].logic = GPIO_LOW,
  .keypad_row_pins[2].direction = GPIO_DIRECTION_OUTPUT,
  .keypad_row_pins[3].port = PORTC_INDEX,
  .keypad_row_pins[3].pin = GPIO_PIN3,
  .keypad_row_pins[3].logic = GPIO_LOW,
  .keypad_row_pins[3].direction = GPIO_DIRECTION_OUTPUT,
  
  
  .keypad_column_pins[0].port = PORTC_INDEX,
  .keypad_column_pins[0].pin = GPIO_PIN4,
  .keypad_column_pins[0].logic = GPIO_LOW,
  .keypad_column_pins[0].direction = GPIO_DIRECTION_INPUT,
  .keypad_column_pins[1].port = PORTC_INDEX,
  .keypad_column_pins[1].pin = GPIO_PIN5,
  .keypad_column_pins[1].logic = GPIO_LOW,
  .keypad_column_pins[1].direction = GPIO_DIRECTION_INPUT,
  .keypad_column_pins[2].port = PORTC_INDEX,
  .keypad_column_pins[2].pin = GPIO_PIN6,
  .keypad_column_pins[2].logic = GPIO_LOW,
  .keypad_column_pins[2].direction = GPIO_DIRECTION_INPUT,
  .keypad_column_pins[3].port = PORTC_INDEX,
  .keypad_column_pins[3].pin = GPIO_PIN7,
  .keypad_column_pins[3].logic = GPIO_LOW,
  .keypad_column_pins[3].direction = GPIO_DIRECTION_INPUT,
};

lcd_4bit_t lcd_1 = {
    .lcd_rs.port = PORTD_INDEX,
    .lcd_rs.pin  = GPIO_PIN0,
    .lcd_rs.direction = GPIO_DIRECTION_OUTPUT,
    .lcd_rs.logic = GPIO_LOW,
    
    .lcd_en.port = PORTD_INDEX,
    .lcd_en.pin  = GPIO_PIN1,
    .lcd_en.direction = GPIO_DIRECTION_OUTPUT,
    .lcd_en.logic = GPIO_LOW,
    
    .lcd_data[0].port = PORTD_INDEX,
    .lcd_data[0].pin  = GPIO_PIN2,
    .lcd_data[0].direction = GPIO_DIRECTION_OUTPUT,
    .lcd_data[0].logic = GPIO_LOW,
    
    .lcd_data[1].port = PORTD_INDEX,
    .lcd_data[1].pin  = GPIO_PIN3,
    .lcd_data[1].direction = GPIO_DIRECTION_OUTPUT,
    .lcd_data[1].logic = GPIO_LOW,
    
    .lcd_data[2].port = PORTD_INDEX,
    .lcd_data[2].pin  = GPIO_PIN4,
    .lcd_data[2].direction = GPIO_DIRECTION_OUTPUT,
    .lcd_data[2].logic = GPIO_LOW,
    
    .lcd_data[3].port = PORTD_INDEX,
    .lcd_data[3].pin  = GPIO_PIN5,
    .lcd_data[3].direction = GPIO_DIRECTION_OUTPUT,
    .lcd_data[3].logic = GPIO_LOW,
};


int main() { 
    Std_ReturnType ret = E_NOT_OK;
    application_intialize();
    while(1){
             
    }
    
    return (EXIT_SUCCESS);
}


void application_intialize(void){
    Std_ReturnType ret = E_NOT_OK;
    ret = lcd_4bit_initialize(&lcd_1);
    ret = keypad_initialize(&keypad1);
}