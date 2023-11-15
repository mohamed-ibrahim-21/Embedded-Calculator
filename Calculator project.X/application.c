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

sint8 ret_val_from_kpad = -1;

uint32 temp_number = ZERO_INIT;
uint32 answer_numebr = ZERO_INIT;
uint8 last_char = '0';

uint32  final_answer[11];

int main() { 
    Std_ReturnType ret = E_NOT_OK;
    application_intialize();
    ret = lcd_4bit_send_command(&lcd_1, _LCD_CURSOR_ON_BLINK_OFF);
    
    while(1){
        ret = keypad_get_value(&keypad1 , &ret_val_from_kpad);
        __delay_ms(150);
        if(ret_val_from_kpad == '#'){
            ret = lcd_4bit_send_command(&lcd_1, _LCD_CLEAR); 
            ret = lcd_4bit_send_command(&lcd_1, _LCD_RETURN_HOME);
            ret_val_from_kpad = -1;
            temp_number = 0;
            answer_numebr = 0;
            last_char = '0';
            ret = lcd_4bit_send_string_position(&lcd_1 , 1 , 6 , "Calculator");
            __delay_ms(1000);
            ret = lcd_4bit_send_command(&lcd_1, _LCD_CLEAR);
        }
        if(ret_val_from_kpad != -1){
            ret = lcd_4bit_send_char_data(&lcd_1 , ret_val_from_kpad);
            
            if(ret_val_from_kpad>='0' && ret_val_from_kpad<='9'){
                temp_number*=10;
                temp_number+=(ret_val_from_kpad-'0');
            }
            else{
                //12 + 12 - 2 = 22 + 
                if(last_char == '0'){
                    answer_numebr=temp_number;
                    if(ret_val_from_kpad == '+'){
                        last_char='+';
                    }
                    else if(ret_val_from_kpad == '-'){
                        last_char='-';
                    }
                    else if(ret_val_from_kpad == '*'){
                        last_char='*';
                    }
                    else if(ret_val_from_kpad == '/'){
                        last_char='/';
                    }
                }
                
                else if(last_char == '+'){
                    answer_numebr+=temp_number;
                    last_char = ret_val_from_kpad;
                }
                else if(last_char == '-'){
                    answer_numebr-=temp_number;
                    last_char = ret_val_from_kpad;
                }
                else if(last_char == '*'){
                    answer_numebr*=temp_number;
                    last_char = ret_val_from_kpad;
                }
                else if(last_char == '/'){
                    answer_numebr/=temp_number;
                    last_char = ret_val_from_kpad;
                }
                
                
                
                if(ret_val_from_kpad == '='){
                    ret = lcd_4bit_send_command(&lcd_1, _LCD_CLEAR);
                    ret = lcd_4bit_send_command(&lcd_1, _LCD_RETURN_HOME);
                    ret = convert_int_to_string(answer_numebr, final_answer);
                    ret = lcd_4bit_send_string(&lcd_1 , final_answer);
                }
                
                temp_number = 0;
            }
            
            ret_val_from_kpad = -1;
        }
        
        
        
    }
    
    return (EXIT_SUCCESS);
}


void application_intialize(void){
    Std_ReturnType ret = E_NOT_OK;
    ret = lcd_4bit_initialize(&lcd_1);
    ret = keypad_initialize(&keypad1);
}