/* 
 * File:   ecu_lcd.c
 * Author: mohamed ibrahim
 *
 * Created on November 14, 2023, 3:14 PM
 */

/********************* Section : Includes *************************************/
#include "ecu_lcd.h"

/***********************************static functions **********************************/

static Std_ReturnType lcd_send_4bits(const lcd_4bit_t *_lcd_ , uint8 _data_command);
static Std_ReturnType lcd_4bit_send_enable_signal(const lcd_4bit_t *_lcd_);
static Std_ReturnType lcd_8bit_send_enable_signal(const lcd_8bit_t *_lcd_);
static Std_ReturnType lcd_8bit_set_cursor (const lcd_8bit_t *_lcd_, uint8 row , uint8 coulmn);
static Std_ReturnType lcd_4bit_set_cursor (const lcd_4bit_t *_lcd_, uint8 row , uint8 coulmn);

/************************************LCD_4BIT*********************************/

/**
 * @brief
 * @param _lcd_
 * @return status of the function
 *          (E_OK)      : the function done successfully
 *          (E_NOT_OK)  : the function has issue 
 */
Std_ReturnType lcd_4bit_initialize               (const lcd_4bit_t *_lcd_){
    Std_ReturnType ret = E_OK;
    uint8 l_data_pins_counter = ZERO_INIT;
    if(NULL == _lcd_){
        ret = E_NOT_OK;
    }
    else{
        ret = gpio_pin_intialize(&(_lcd_->lcd_rs));
        ret = gpio_pin_intialize(&(_lcd_->lcd_en));
        for(l_data_pins_counter = 0 ; l_data_pins_counter<4 ; l_data_pins_counter++){
            ret = gpio_pin_intialize(&(_lcd_->lcd_data[l_data_pins_counter]));
        }
        
        __delay_ms(20);
        ret = lcd_4bit_send_command(_lcd_, _LCD_8BIT_MODE_2_LINE);
        __delay_ms(5);
        ret = lcd_4bit_send_command(_lcd_, _LCD_8BIT_MODE_2_LINE);
        __delay_us(150); 
        ret = lcd_4bit_send_command(_lcd_, _LCD_8BIT_MODE_2_LINE);
        
        ret = lcd_4bit_send_command(_lcd_, _LCD_CLEAR);
        ret = lcd_4bit_send_command(_lcd_, _LCD_RETURN_HOME);
        ret = lcd_4bit_send_command(_lcd_, _LCD_ENTRY_MODE);
        ret = lcd_4bit_send_command(_lcd_, _LCD_CURSOR_OFF_DISPLAY_ON);
        ret = lcd_4bit_send_command(_lcd_, _LCD_4BIT_MODE_2_LINE);
        ret = lcd_4bit_send_command(_lcd_, 0x80);//start point in lcd
        
    }
    
    
    return ret;
}

/**
 * @brief
 * @param _lcd_
 * @param command
 * @return status of the function
 *          (E_OK)      : the function done successfully
 *          (E_NOT_OK)  : the function has issue
 */
Std_ReturnType lcd_4bit_send_command             (const lcd_4bit_t *_lcd_ ,uint8 command){
    Std_ReturnType ret = E_OK;
    if(NULL == _lcd_){
        ret = E_NOT_OK;
    }
    else{
        ret = gpio_pin_write_logic(&(_lcd_->lcd_rs), GPIO_LOW);
        ret = lcd_send_4bits(_lcd_ , command>>4);
        ret = lcd_4bit_send_enable_signal(_lcd_);
        ret = lcd_send_4bits(_lcd_ , command);
        ret = lcd_4bit_send_enable_signal(_lcd_);
    }
    
    
    return ret;
}

/**
 * @brief
 * @param _lcd_
 * @param data
 * @return status of the function
 *          (E_OK)      : the function done successfully
 *          (E_NOT_OK)  : the function has issue
 */
Std_ReturnType lcd_4bit_send_char_data           (const lcd_4bit_t *_lcd_ ,uint8 data){
    Std_ReturnType ret = E_OK;
    if(NULL == _lcd_){
        ret = E_NOT_OK;
    }
    else{
        ret = gpio_pin_write_logic(&(_lcd_->lcd_rs), GPIO_HIGH);
        ret = lcd_send_4bits(_lcd_ , data>>4);
        ret = lcd_4bit_send_enable_signal(_lcd_);
        ret = lcd_send_4bits(_lcd_ , data);
        ret = lcd_4bit_send_enable_signal(_lcd_);
    }
    
    
    return ret;
}

/**
 * @brief
 * @param _lcd_
 * @param row
 * @param column
 * @param data
 * @return status of the function
 *          (E_OK)      : the function done successfully
 *          (E_NOT_OK)  : the function has issue
 */
Std_ReturnType lcd_4bit_send_char_data_position  (const lcd_4bit_t *_lcd_ ,uint8 row ,uint8 column , uint8 data){
    Std_ReturnType ret = E_OK;
    if(NULL == _lcd_){
        ret = E_NOT_OK;
    }
    else{
        ret = lcd_4bit_set_cursor(_lcd_, row , column);
        ret = lcd_4bit_send_char_data (_lcd_ , data);
    }
    
    
    return ret;
}

/**
 * @brief
 * @param _lcd_
 * @param str
 * @return status of the function
 *          (E_OK)      : the function done successfully
 *          (E_NOT_OK)  : the function has issue
 */
Std_ReturnType lcd_4bit_send_string              (const lcd_4bit_t *_lcd_ ,uint8 *str){
    Std_ReturnType ret = E_OK;
    if(NULL == _lcd_){
        ret = E_NOT_OK;
    }
    else{
        while(*str){
            ret = lcd_4bit_send_char_data(_lcd_, *str++);
        }
    }
    
    
    return ret;
}

/**
 * @brief
 * @param _lcd_
 * @param row
 * @param column
 * @param str
 * @return status of the function
 *          (E_OK)      : the function done successfully
 *          (E_NOT_OK)  : the function has issue 
 */
Std_ReturnType lcd_4bit_send_string_position     (const lcd_4bit_t *_lcd_ ,uint8 row ,uint8 column , uint8 *str){
    Std_ReturnType ret = E_OK;
    if(NULL == _lcd_){
        ret = E_NOT_OK;
    }
    else{
        ret = lcd_4bit_set_cursor(_lcd_, row , column);
        while(*str){
            ret = lcd_4bit_send_char_data(_lcd_, *str++);
        }
    }
    
    
    return ret;
}

/**
 * @brief
 * @param _lcd_
 * @param row
 * @param column
 * @param _chr
 * @param mem_pos
 * @return status of the function
 *          (E_OK)      : the function done successfully
 *          (E_NOT_OK)  : the function has issue 
 */
Std_ReturnType lcd_4bit_send_custome_chr         (const lcd_4bit_t *_lcd_ ,uint8 row ,uint8 column , const uint8 _chr[], uint8 mem_pos){
    Std_ReturnType ret = E_OK;
    uint8 l_lcd_counter = ZERO_INIT;
    if(NULL == _lcd_){
        ret = E_NOT_OK;
    }
    else{
        ret = lcd_4bit_send_command(_lcd_, (_LCD_CGRAM_START+(mem_pos*8)));
        for(l_lcd_counter = 0 ; l_lcd_counter<8 ; l_lcd_counter++){
            ret = lcd_4bit_send_char_data(_lcd_, _chr[l_lcd_counter]);
        }
        ret = lcd_4bit_send_char_data_position(_lcd_ , row , column , mem_pos);
    }
    
    
    return ret;
}

/*********************************LCD_8bit**************************************/

/**
 * @brief
 * @param _lcd_
 * @return status of the function
 *          (E_OK)      : the function done successfully
 *          (E_NOT_OK)  : the function has issue
 */
Std_ReturnType lcd_8bit_initialize               (const lcd_8bit_t *_lcd_){
    Std_ReturnType ret = E_OK;
    uint8 l_data_pins_counter = ZERO_INIT;
    if(NULL == _lcd_){
        ret = E_NOT_OK;
    }
    else{
        ret = gpio_pin_intialize(&(_lcd_->lcd_rs));
        ret = gpio_pin_intialize(&(_lcd_->lcd_en));
        for(l_data_pins_counter = 0 ; l_data_pins_counter<8 ; l_data_pins_counter++){
            ret = gpio_pin_intialize(&(_lcd_->lcd_data[l_data_pins_counter]));
        }
        __delay_ms(20);
        ret = lcd_8bit_send_command(_lcd_, _LCD_8BIT_MODE_2_LINE);
        __delay_ms(5);
        ret = lcd_8bit_send_command(_lcd_, _LCD_8BIT_MODE_2_LINE);
        __delay_us(150); 
        ret = lcd_8bit_send_command(_lcd_, _LCD_8BIT_MODE_2_LINE);
        
        ret = lcd_8bit_send_command(_lcd_, _LCD_CLEAR);
        ret = lcd_8bit_send_command(_lcd_, _LCD_RETURN_HOME);
        ret = lcd_8bit_send_command(_lcd_, _LCD_ENTRY_MODE);
        ret = lcd_8bit_send_command(_lcd_, _LCD_CURSOR_OFF_DISPLAY_ON);
        ret = lcd_8bit_send_command(_lcd_, _LCD_8BIT_MODE_2_LINE);
        ret = lcd_8bit_send_command(_lcd_, 0x80);//start point in lcd
    }
    
    
    return ret;
}

/**
 * @brief
 * @param _lcd_
 * @param command
 * @return status of the function
 *          (E_OK)      : the function done successfully
 *          (E_NOT_OK)  : the function has issue
 */
Std_ReturnType lcd_8bit_send_command             (const lcd_8bit_t *_lcd_ ,uint8 command){
    Std_ReturnType ret = E_OK;
    uint8 l_pin_counter = ZERO_INIT;
    if(NULL == _lcd_){
        ret = E_NOT_OK;
    }
    else{
        ret = gpio_pin_write_logic(&(_lcd_->lcd_rs), GPIO_LOW);
        for(l_pin_counter = 0 ; l_pin_counter < 8 ; l_pin_counter++){
            ret = gpio_pin_write_logic(&(_lcd_->lcd_data[l_pin_counter]), (command >> l_pin_counter) & (uint8)0x01);
        }
        ret = lcd_8bit_send_enable_signal(_lcd_);
    }
    
    
    return ret;
}

/**
 * @brief
 * @param _lcd_
 * @param data
 * @return status of the function
 *          (E_OK)      : the function done successfully
 *          (E_NOT_OK)  : the function has issue
 */
Std_ReturnType lcd_8bit_send_char_data           (const lcd_8bit_t *_lcd_ ,uint8 data){
    Std_ReturnType ret = E_OK;
    uint8 l_pin_counter = ZERO_INIT;
    if(NULL == _lcd_){
        ret = E_NOT_OK;
    }
    else{
        ret = gpio_pin_write_logic(&(_lcd_->lcd_rs), GPIO_HIGH);
        for(l_pin_counter = 0 ; l_pin_counter < 8 ; l_pin_counter++){
            ret = gpio_pin_write_logic(&(_lcd_->lcd_data[l_pin_counter]), (data >> l_pin_counter) & (uint8)0x01);
        }
        ret = lcd_8bit_send_enable_signal(_lcd_);
    }
    
    
    return ret;
}

/**
 * @brief
 * @param _lcd_
 * @param row
 * @param column
 * @param data
 * @return status of the function
 *          (E_OK)      : the function done successfully
 *          (E_NOT_OK)  : the function has issue 
 */
Std_ReturnType lcd_8bit_send_char_data_position  (const lcd_8bit_t *_lcd_ ,uint8 row ,uint8 column , uint8 data){
    Std_ReturnType ret = E_OK;
    if(NULL == _lcd_){
        ret = E_NOT_OK;
    }
    else{
        ret = lcd_8bit_set_cursor(_lcd_, row, column);
        ret = lcd_8bit_send_char_data(_lcd_, data);
    }
    
    
    return ret;
}

/**
 * @brief
 * @param _lcd_
 * @param str
 * @return status of the function
 *          (E_OK)      : the function done successfully
 *          (E_NOT_OK)  : the function has issue 
 */
Std_ReturnType lcd_8bit_send_string              (const lcd_8bit_t *_lcd_ ,uint8 *str){
    Std_ReturnType ret = E_OK;
    if(NULL == _lcd_){
        ret = E_NOT_OK;
    }
    else{
        while(*str){
            ret = lcd_8bit_send_char_data(_lcd_, *str++);
        }
    }
    
    
    return ret;
}

/**
 * @brief
 * @param _lcd_
 * @param row
 * @param column
 * @param str
 * @return status of the function
 *          (E_OK)      : the function done successfully
 *          (E_NOT_OK)  : the function has issue
 */
Std_ReturnType lcd_8bit_send_string_position     (const lcd_8bit_t *_lcd_ ,uint8 row ,uint8 column , uint8 *str){
    Std_ReturnType ret = E_OK;
    if(NULL == _lcd_){
        ret = E_NOT_OK;
    }
    else{
        ret = lcd_8bit_set_cursor(_lcd_, row, column);
        while(*str){
            ret = lcd_8bit_send_char_data(_lcd_, *str++);
        }
    }
    return ret;
}

/**
 * @brief
 * @param _lcd_
 * @param row
 * @param column
 * @param _chr
 * @param mem_pos
 * @return status of the function
 *          (E_OK)      : the function done successfully
 *          (E_NOT_OK)  : the function has issue 
 */
Std_ReturnType lcd_8bit_send_custome_chr         (const lcd_8bit_t *_lcd_ ,uint8 row ,uint8 column , const uint8 _chr[], uint8 mem_pos){
    Std_ReturnType ret = E_OK;
    uint8 l_lcd_counter = ZERO_INIT;
    if(NULL == _lcd_){
        ret = E_NOT_OK;
    }
    else{
        ret = lcd_8bit_send_command(_lcd_, (_LCD_CGRAM_START+(mem_pos*8)));
        for(l_lcd_counter = 0 ; l_lcd_counter<8 ; l_lcd_counter++){
            ret = lcd_8bit_send_char_data(_lcd_, _chr[l_lcd_counter]);
        }
        ret = lcd_8bit_send_char_data_position(_lcd_ , row , column , mem_pos);
    }
    return ret;
}

/*******************************************CONVERT_TO_STRING*******************************************/

Std_ReturnType convert_byte_to_string  (uint8 value, uint8 *str){
    Std_ReturnType ret = E_OK;
    if(NULL == str){
        ret = E_NOT_OK;
    }
    else{
        memset(str, '\0', 4);
        sprintf(str , "%i", value);
    }
    return ret;
}

Std_ReturnType convert_short_to_string (uint16 value, uint8 *str){
    Std_ReturnType ret = E_OK;
    if(NULL == str){
        ret = E_NOT_OK;
    }
    else{
        memset(str, '\0', 6);
        sprintf(str , "%i", value);
    }
    return ret;
}

Std_ReturnType convert_int_to_string   (uint32 value, uint8 *str){
    Std_ReturnType ret = E_OK;
    if(NULL == str){
        ret = E_NOT_OK;
    }
    else{
        memset(str, '\0', 11);
        sprintf(str , "%i", value);
    }
    return ret;
}

/**********************************************HELPER FUNCTIONS*********************************************/
static Std_ReturnType lcd_send_4bits(const lcd_4bit_t *_lcd_ , uint8 _data_command){
    Std_ReturnType ret = E_OK;
    ret = gpio_pin_write_logic(&(_lcd_->lcd_data[0]), (_data_command >> 0) & (uint8)0x01);
    ret = gpio_pin_write_logic(&(_lcd_->lcd_data[1]), (_data_command >> 1) & (uint8)0x01);
    ret = gpio_pin_write_logic(&(_lcd_->lcd_data[2]), (_data_command >> 2) & (uint8)0x01);
    ret = gpio_pin_write_logic(&(_lcd_->lcd_data[3]), (_data_command >> 3) & (uint8)0x01);
    
    return ret;
}

static Std_ReturnType lcd_4bit_send_enable_signal(const lcd_4bit_t *_lcd_){
    Std_ReturnType ret = E_OK;
    ret = gpio_pin_write_logic(&(_lcd_->lcd_en), GPIO_HIGH);
    __delay_us(5);
    ret = gpio_pin_write_logic(&(_lcd_->lcd_en), GPIO_LOW);
    return ret;
}

static Std_ReturnType lcd_8bit_send_enable_signal(const lcd_8bit_t *_lcd_){
    Std_ReturnType ret = E_OK;
    ret = gpio_pin_write_logic(&(_lcd_->lcd_en), GPIO_HIGH);
    __delay_us(5);
    ret = gpio_pin_write_logic(&(_lcd_->lcd_en), GPIO_LOW);
    return ret;
}

static Std_ReturnType lcd_8bit_set_cursor (const lcd_8bit_t *_lcd_, uint8 row , uint8 coulmn){
    Std_ReturnType ret = E_OK;
    coulmn--;
    switch (row){
        case ROW1 : ret = lcd_8bit_send_command(_lcd_ , (0x80 + coulmn));
        break;
        case ROW2 : ret = lcd_8bit_send_command(_lcd_ , (0xc0 + coulmn));
        break;
        case ROW3 : ret = lcd_8bit_send_command(_lcd_ , (0x94 + coulmn));
        break;
        case ROW4 : ret = lcd_8bit_send_command(_lcd_ , (0xd4 + coulmn));
        break;
        default : ;
    }
    return ret;
}

static Std_ReturnType lcd_4bit_set_cursor (const lcd_4bit_t *_lcd_, uint8 row , uint8 coulmn){
    Std_ReturnType ret = E_OK;
    coulmn--;
    switch (row){
        case ROW1 : ret = lcd_4bit_send_command(_lcd_ , (0x80 + coulmn));
        break;
        case ROW2 : ret = lcd_4bit_send_command(_lcd_ , (0xc0 + coulmn));
        break;
        case ROW3 : ret = lcd_4bit_send_command(_lcd_ , (0x94 + coulmn));
        break;
        case ROW4 : ret = lcd_4bit_send_command(_lcd_ , (0xd4 + coulmn));
        break;
        default : ;
    }
    return ret;
}


