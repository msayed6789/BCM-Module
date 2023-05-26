/*
 * LED.h
 *
 * Created: 4/5/2023 2:01:12 AM
 *  Author: AIO
 */ 


#ifndef LED_H_
#define LED_H_
#include "../../MCAL/dio/DIO_Interface.h"
/***************************LED Configuration******************************************************/
//#define  LED1      PINA0
//#define  LED2      PINA1
//#define  LED3      PINA2
//#define  LED4      PINA3
//#define  LED5      PINA4
//#define  LED6      PINA5
/*************************************************************************************************/
void LED_init(u8 led);
void LED_ON (u8 LED );
void LED_OFF (u8 LED );
void LED_Toggle (u8 LED );


#endif /* LED_H_ */