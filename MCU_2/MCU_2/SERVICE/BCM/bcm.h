


#ifndef BCM_H_
#define BCM_H_

#include "../../MCAL/uart/uart.h"
#include "../../ECUAL/lcd/LCD.h"

#define END_SEND                       0x0D
#define MAX_DATA_LENGTH                50
#define QUEUE_SIZE                     30
#define ARRAY_OF_POINTER_SIZE          10

#define isstringfinish()    u8_G_Tstr[u8_gl_row_send][st_u8_index]
#define isqueueempty()      u8_G_Tstr[u8_gl_row_send][0]

typedef enum
{
	uart,
	spi,
	i2c
}enu_protocol_t;

typedef enum
{
	Done,
	Error
}enu_system_status_t;

enu_system_status_t bcm_init(enu_protocol_t protocol);
enu_system_status_t bcm_deinit(enu_protocol_t protocol);

/*******************************************************Transmitter************************************************************************************************/
void bcm_send(u8 data);//Blocking Function 
void bcm_send_Blocking(u8*str);// Blocking function
void bcm_send_Non_Blocking(u8*str,u8 length, u8 start);  //Non Blocking function
void uart_bcm_send_dispatcher(void);
void bcm_send_End_Setcallback(void (*fptr)(void));

/*******************************************************Receiver*********************************************************************************************/
void bcm_recieve(u8*data);
void bcm_recieve_Blocking(u8*str);
void bcm_recieve_Non_Blocking(u8*str);
void uart_bcm_recieve_dispatcher(void);
void bcm_recieve_End_Setcallback(void (*fptr)(void));







#endif /* BCM_H_ */