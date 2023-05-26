

#ifndef UART_H_
#define UART_H_

#include "../../COMMON/MEMMAP.h"
#include "../../COMMON/Standard_Type.h"
#include "../../COMMON/Utlise.h"




#define F8MHZ          8
#define F4MHZ          4
#define SYNCH         'S'
#define ASYNCH        'A'
#define CHANNEL_SIZE   2


#define UART_MODE    ASYNCH  

#define FCPU   F8MHZ                         //The CPU Frequency in MHz
 
#define ERROR_DATA         'E'
#define DATA_OVERRUN       'O'
#define DATA_DONE          'D'



typedef enum {
	Baudrate2400,
	Baudrate4800,
	Baudrate9600,
	Baudrate144K,
	Baudrate_Total
}enu_uart_baud_rate_t;

typedef enum {
	Normal_Speed,
	Double_Speed,
	Speed_Total
}enu_uart_speed_t;

typedef enum{
	Even,
	Odd,
	No_Parity,
	Parity_Total
}enu_uart_parity_t;

typedef enum{
	StopBit_1,
	StopBit_2,
	StopBit_Total
}enu_uart_stopbit_t;

typedef enum{
	DataBit_5,
	DataBit_6,
	DataBit_7,
	DataBit_8,
	DataBit_Total
}enu_uart_databit_t;

typedef enum{
	Transmitter,
	Reciever,
	TR,
	Enable_Total
}enu_uart_enable_t;

typedef enum
{
	Init_Done,
	Init_Error
}enu_uart_status_t;

#if UART_MODE==ASYNCH
typedef struct
{
	enu_uart_baud_rate_t       enu_uart_baud_rate;
	enu_uart_speed_t           enu_uart_speed;
	enu_uart_parity_t          enu_uart_parity;
	enu_uart_stopbit_t         enu_uart_stopbit;
	enu_uart_databit_t         enu_uart_databit;
	enu_uart_enable_t          enu_uart_enable;
}str_uart_config_t;
#elif UART_MODE==SYNCH
typedef struct
{
	enu_uart_speed_t           enu_uart_speed;
	enu_uart_databit_t         enu_uart_databit;
	enu_uart_enable_t          enu_uart_enable;
}str_uart_config_t;
#endif





//void uart_init(enu_uart_baud_rate_t BaudRate,enu_uart_speed_t Speed,enu_uart_mode_t mode,enu_uart_parity_t parity,enu_uart_stopbit_t stopbit,enu_uart_databit_t databit,enu_uart_enable_t enable);

enu_uart_status_t uart_init(void);
/*******************************************************Transmitter************************************************************************************************/
void uart_transmit(u8 data);
void uart_transmitNoBlock(u8 data);
/**************************************************Transmitter Interrupt********************************************************************************************/
void uart_transmitComPlete_InterruptEnable(void);
void uart_transmitComPlete_InterruptDisable(void);
void uart_transmitComPlete_InterruptSetCallback(void(*fptr)(void));

/*******************************************************Receiver*********************************************************************************************/
u8 uart_reciever(void);
u8 uart_recieverNoBlock(void);
/*******************************************Receiver Interrupt***********************************************************************************************/
void uart_recieveComPlete_InterruptEnable(void);
void uart_recieveComPlete_InterruptDisable(void);
void uart_recieveComPlete_InterruptSetCallback(void(*fptr)(void));


#endif /* UART_H_ */