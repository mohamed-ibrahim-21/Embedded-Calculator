/* 
 * File:   ecu_keypad.h
 * Author: mohamed ibrahim
 *
 * Created on November 8, 2023, 5:10 PM
 */

#ifndef ECU_KEYPAD_H
#define	ECU_KEYPAD_H

/* Section : Includes */
#include "ecu_keypad_cfg.h"
#include "../../MCAL_Layer/GPIO/hal_gpio.h"
/* Section : Macro Declarations */
#define KEYPAD_ROW      4
#define KEYPAD_COLUMNS   4
/* Section : Macro Functions Declarations*/

/* Section : Data Type Declarations*/
typedef struct {
    pin_config_t keypad_row_pins[KEYPAD_ROW];
    pin_config_t keypad_column_pins[KEYPAD_COLUMNS];
}keypad_t;


/* Section : Functions Declarations*/
Std_ReturnType keypad_initialize (const keypad_t *_keypad_obj);
Std_ReturnType keypad_get_value  (const keypad_t *_keypad_obj, uint8 *value);
#endif	/* ECU_KEYPAD_H */

