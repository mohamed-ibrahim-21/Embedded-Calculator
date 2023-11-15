/* 
 * File:   application.h
 * Author: mohamed ibrahim
 *
 * Created on October 4, 2023, 10:21 PM
 */

#ifndef APPLICATION_H
#define	APPLICATION_H

/* Section : Includes */
#include "ECU_Layer/LED/ecu_led.h"
#include "ECU_Layer/Push_button/ecu_push_button.h"
#include "ECU_Layer/Relay/ecu_relay.h"
#include "ECU_Layer/DC_Motor/ecu_dc_motor.h"
#include "ECU_Layer/7_Segment/ecu_seven_segment.h"
#include "ECU_Layer/Keypad/ecu_keypad.h"
#include "ECU_Layer/LCD/ecu_lcd.h"
/* Section : Macro Declarations */

/* Section : Macro Functions Declarations*/

/* Section : Data Type Declarations*/

/* Section : Functions Declarations*/
void application_intialize(void);


#endif	/* APPLICATION_H */

