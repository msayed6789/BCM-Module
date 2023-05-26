


#ifndef APP_H_
#define APP_H_


#include "../../SERVICE/bcm/bcm.h"
#include "../../ECUAL/led/led.h"
#include "../../ECUAL/lcd/LCD.h"


#define  Send_LED         PINA0
#define  Recieve_LED      PINA1




void app_init(void);
void app_start(void);


#endif /* APP_H_ */