/* 
 * File:   ecu_keypad.c
 * Author: mohamed ibrahim
 *
 * Created on November 8, 2023, 5:10 PM
 */

#include "ecu_keypad.h"

static const uint8 btn_values[KEYPAD_ROW][KEYPAD_COLUMNS] = {
                                                                {'7','8','9','/'},
                                                                {'4','5','6','*'},
                                                                {'1','2','3','-'},
                                                                {'#','0','=','+'},
                                                            };
/**
 * @brief
 * @param _keypad_obj
 * @return status of the function
 *          (E_OK)      : the function done successfully
 *          (E_NOT_OK)  : the function has issue 
 */
Std_ReturnType keypad_initialize (const keypad_t *_keypad_obj){
    Std_ReturnType ret = E_OK;
    uint8 rows_counter = ZERO_INIT, columns_counter = ZERO_INIT;
    if(NULL == _keypad_obj){
        ret = E_NOT_OK;
    }
    else{
        for(rows_counter = ZERO_INIT ; rows_counter < KEYPAD_ROW ; rows_counter++){
            ret = gpio_pin_intialize(&(_keypad_obj->keypad_row_pins[rows_counter]));
        }
        for(columns_counter = ZERO_INIT ; columns_counter < KEYPAD_COLUMNS ; columns_counter++){
            ret = gpio_pin_direction_intialize(&(_keypad_obj->keypad_column_pins[columns_counter]));
        }
    }
    return ret;
}

/**
 * @brief
 * @param _keypad_obj
 * @param value
 * @return status of the function
 *          (E_OK)      : the function done successfully
 *          (E_NOT_OK)  : the function has issue  
 */
Std_ReturnType keypad_get_value  (const keypad_t *_keypad_obj, uint8 *value){
    Std_ReturnType ret = E_OK;
    uint8 l_rows_counter = ZERO_INIT , l_columns_counter = ZERO_INIT , l_counter = ZERO_INIT;
    uint8 coulomn_logic = ZERO_INIT;
    if((NULL == _keypad_obj) && (NULL == value)){
        ret = E_NOT_OK;
    }
    else{
        for(l_rows_counter = ZERO_INIT ; l_rows_counter < KEYPAD_ROW ; l_rows_counter++){
            for(l_counter = ZERO_INIT ; l_counter < KEYPAD_ROW ; l_counter++){
               ret = gpio_pin_write_logic(&(_keypad_obj->keypad_row_pins[l_counter]) , GPIO_LOW);
            }
            gpio_pin_write_logic(&(_keypad_obj->keypad_row_pins[l_rows_counter]) , GPIO_HIGH);
            __delay_ms(10);
            for(l_columns_counter = ZERO_INIT ; l_columns_counter < KEYPAD_COLUMNS ; l_columns_counter++){
                ret = gpio_pin_read_logic(&(_keypad_obj->keypad_column_pins[l_columns_counter]) , &coulomn_logic);
                if(GPIO_HIGH == coulomn_logic){
                    *value = btn_values[l_rows_counter][l_columns_counter];
                }
            }
        }
    }
    return ret;
}
