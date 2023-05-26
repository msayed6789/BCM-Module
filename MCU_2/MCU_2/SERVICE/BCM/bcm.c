#include "bcm.h"


/***********************************************************************Transmitter*********************************************************************/
static void uart_send_isr(void);
static volatile u8 u8_gl_TFlag_S=1;
static volatile u8 u8_gl_TFlag_d=0;
static volatile u8 u8_gl_row_send =0;
static volatile u8 gl_u8_dispatcher_send_start=0;
static volatile u8 u8_G_Tstr[QUEUE_SIZE][MAX_DATA_LENGTH]={0};
/***********************************************************************Reciever*****************************************************************************/
static void uart_receive_isr(void);
static volatile u8 u8_gl_RFlag_S=1;
static volatile u8 u8_gl_RFlag_d=0;
static volatile u8 u8_gl_row_recieve =0;
static volatile u8 gl_u8_dispatcher_receieve_start=0;
static volatile u8 *u8_gl_ptr_Rstr[ARRAY_OF_POINTER_SIZE]={0};
	
/***********************************************************************Pointer to function*************************************************************/
static void (*Tfptr)(void)=NULLPTR;
static void (*Rfptr)(void)=NULLPTR;
/************************************************************************SetCallback function***********************************************************/
void bcm_send_End_Setcallback(void (*fptr)(void))
{
	Tfptr=fptr;
}

void bcm_recieve_End_Setcallback(void (*fptr)(void))
{
	Rfptr=fptr;
}
/*************************************************************************BCM_Init & deinit**************************************************************/

enu_system_status_t bcm_init(enu_protocol_t protocol)
{
	u8 status=Done;
	switch (protocol)
	{
		case uart:
		status=uart_init();
		break;
		case spi:
		//do nothing
		break;
		case i2c:
		//do nothing
		break;
	}
	return status;
}


enu_system_status_t bcm_deinit(enu_protocol_t protocol)
{
	u8 status=Done;
	switch (protocol)
	{
		case uart:
		u8_gl_TFlag_S=0;
		u8_gl_RFlag_S=0;
		uart_transmitComPlete_InterruptDisable();
		uart_recieveComPlete_InterruptDisable();
		break;
		case spi:
		//do nothing
		break;
		case i2c:
		//do nothing
		break;
	}
	return status;
}

/*********************************************************************************************************************************************************/

void bcm_send(u8 data)
{
	uart_transmit(data);
}


void bcm_send_Blocking(u8*str)
{
	u8 u8_index=0;
	for (u8_index=0;str[u8_index];u8_index++)
	{
		uart_transmit(str[u8_index]);
	}
	uart_transmit(END_SEND);
}

void bcm_send_Non_Blocking(u8*str,u8 length, u8 start)
{
	static u8 st_u8_row=0;
	  u8 u8_index=0;
		if (u8_gl_TFlag_S==1)
		{
			st_u8_row=0;
			gl_u8_dispatcher_send_start=1;
			u8_gl_TFlag_S=0;
		}
		for (u8_index=0;u8_index<length;u8_index++)
		{
			u8_G_Tstr[st_u8_row][u8_index]=str[start];
			start++;
		}
		st_u8_row++;
		if (st_u8_row==QUEUE_SIZE)
		{
			st_u8_row=0;
		}
}
void uart_bcm_send_dispatcher(void)
{
	if (gl_u8_dispatcher_send_start==1)
	{
		uart_transmit(u8_G_Tstr[u8_gl_row_send][0]);
		uart_transmitComPlete_InterruptSetCallback(uart_send_isr);
		uart_transmitComPlete_InterruptEnable();
		gl_u8_dispatcher_send_start=0;
	}
	
	if (u8_gl_TFlag_d==1)
	{
		u8_gl_TFlag_d=0;
		u8_gl_row_send++;
		if (Tfptr!=NULLPTR)
		{
			Tfptr();
		}
		else
		{
			//do nothing
		}
		uart_transmit(END_SEND);
		if (isqueueempty()==0)
		{
			uart_transmitComPlete_InterruptDisable();
			u8_gl_TFlag_S=1;
			u8_gl_row_send=0;
		}
		else 
		{
			uart_transmit(u8_G_Tstr[u8_gl_row_send][0]);
		}
	}
	else
	{
		//do nothing
	}
}
static void uart_send_isr(void)
{
	static u8 st_u8_index=1;
	uart_transmitNoBlock(u8_G_Tstr[u8_gl_row_send][st_u8_index]);
	st_u8_index++;
	if (!isstringfinish())
	{
		st_u8_index=1;
		u8_gl_TFlag_d=1;
	}
	else
	{
		//do nothing
	}
}

/*******************************************************Receiver******************************************************************************************/
void bcm_recieve(u8*data)
{
	*data=uart_reciever();
}

void bcm_recieve_Blocking(u8*str)
{
	u8 u8_index=0;
	str[u8_index]=uart_reciever();
	while (str[u8_index]!=END_SEND)
	{
		u8_index++;
		str[u8_index]=uart_reciever();
	}
	str[u8_index]=NULL;
}

void bcm_recieve_Non_Blocking(u8*str)
{
	static u8 st_u8_index=0;
	if (u8_gl_RFlag_S==1)
	{
		st_u8_index=0;
		gl_u8_dispatcher_receieve_start=1;
		u8_gl_RFlag_S=0;
	}
	u8_gl_ptr_Rstr[st_u8_index]=str;
	st_u8_index++;
	if (st_u8_index==ARRAY_OF_POINTER_SIZE)
	{
		st_u8_index=0;
	}
}
void uart_bcm_recieve_dispatcher()
{
	if (gl_u8_dispatcher_receieve_start==1)
	{
		uart_recieveComPlete_InterruptSetCallback(uart_receive_isr);
		uart_recieveComPlete_InterruptEnable();
		gl_u8_dispatcher_receieve_start=0;
	}
	if (u8_gl_RFlag_d==1)
	{
		u8_gl_RFlag_d=0;
		u8_gl_row_recieve++;
		if (u8_gl_ptr_Rstr[u8_gl_row_recieve]==0)
		{
			uart_recieveComPlete_InterruptDisable();
			u8_gl_row_recieve=0;
			u8_gl_RFlag_S=1;
		}
		else
		{
			//do nothing
		}
		if (Rfptr!=NULLPTR)
		{
			Rfptr();
		}
		else
		{
			//do nothing
		}
	}
	
}

static void uart_receive_isr(void)
{
	static u8 st_u8_index=0;
	*(u8_gl_ptr_Rstr[u8_gl_row_recieve]+st_u8_index)=uart_recieverNoBlock();
	
	if (*(u8_gl_ptr_Rstr[u8_gl_row_recieve]+st_u8_index)==END_SEND)
	{
		*((u8_gl_ptr_Rstr[u8_gl_row_recieve])+st_u8_index)=NULL;
		st_u8_index=0;
		u8_gl_RFlag_d=1;
	}
	else
	{
		st_u8_index++;
	}
}


