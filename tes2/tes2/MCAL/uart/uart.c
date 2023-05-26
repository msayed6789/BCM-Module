
 #include "uart.h"
 #include "uart_config.h"
 /************************************************************Pointer to function************************************************************************/
 static void (*Tfptr)(void)=NULLPTR;
 static void (*Rfptr)(void)=NULLPTR;
 /*******************************************************************************************************************************************************/
 #if FCPU==8
 static u16 BaudRate_arr[Baudrate_Total][Speed_Total]={
	 {207,416},
     {103,207},
	 {51,103},
	 {34,68},	 
 };
 #elif FCPU==16
 static u16 BaudRate_arr[Baudrate_Total][Speed_Total]={
	 {103,207},
	 {51,103},
	 {25,51},
	 {16,34},
 };
 #endif
 
 enu_uart_status_t uart_init()
 {
	 u8 i=0;
	 for (i=0;i<CHANNEL_SIZE;i++)
	 {
		 if (gstr_uart_config_ch[i].enu_uart_speed<Speed_Total)
		 {
			 // Baud rate
			 if (BaudRate_arr[gstr_uart_config_ch[i].enu_uart_baud_rate][gstr_uart_config_ch[i].enu_uart_speed]>255)
			 {
				 CLR_Bit(UCSRC,URSEL);
				 UBRRH=(u8)(BaudRate_arr[gstr_uart_config_ch[i].enu_uart_baud_rate][gstr_uart_config_ch[i].enu_uart_speed]>>8);
				 UBRRL=(u8)BaudRate_arr[gstr_uart_config_ch[i].enu_uart_baud_rate][gstr_uart_config_ch[i].enu_uart_speed];
			 }
			 else
			 {
				 UBRRL=BaudRate_arr[gstr_uart_config_ch[i].enu_uart_baud_rate][gstr_uart_config_ch[i].enu_uart_speed];
			 }
			 //speed
			 if (gstr_uart_config_ch[i].enu_uart_speed==Normal_Speed)
			 {
				 CLR_Bit(UCSRA,U2X);
			 }
			 else
			 {
				 Set_Bit(UCSRA,U2X);
			 }
			 //uart Frame
			 //parity
			 if (gstr_uart_config_ch[i].enu_uart_parity==Even)
			 {
				 Set_Bit(UCSRC,URSEL);
				 CLR_Bit(UCSRC,UPM0);
				 Set_Bit(UCSRC,UPM1);
			 }
			 else if (gstr_uart_config_ch[i].enu_uart_parity==Odd)
			 {
				 Set_Bit(UCSRC,URSEL);
				 Set_Bit(UCSRC,UPM0);
				 Set_Bit(UCSRC,UPM1);
			 }
			 else
			 {
				 Set_Bit(UCSRC,URSEL);
				 CLR_Bit(UCSRC,UPM0);
				 CLR_Bit(UCSRC,UPM1);
			 }
			 //stop bit
			 if (gstr_uart_config_ch[i].enu_uart_stopbit==StopBit_1)
			 {
				 Set_Bit(UCSRC,URSEL);
				 CLR_Bit(UCSRC,USBS);
			 }
			 else
			 {
				 Set_Bit(UCSRC,URSEL);
				 Set_Bit(UCSRC,USBS);
			 }
			 //data
			 switch(gstr_uart_config_ch[i].enu_uart_databit)
			 {
				 case DataBit_5:
				 Set_Bit(UCSRC,URSEL);
				 CLR_Bit(UCSRC,UCSZ0);
				 CLR_Bit(UCSRC,UCSZ1);
				 CLR_Bit(UCSRB,UCSZ2);
				 break;
				 case DataBit_6:
				 Set_Bit(UCSRC,URSEL);
				 Set_Bit(UCSRC,UCSZ0);
				 CLR_Bit(UCSRC,UCSZ1);
				 CLR_Bit(UCSRB,UCSZ2);
				 break;
				 case DataBit_7:
				 Set_Bit(UCSRC,URSEL);
				 CLR_Bit(UCSRC,UCSZ0);
				 Set_Bit(UCSRC,UCSZ1);
				 CLR_Bit(UCSRB,UCSZ2);
				 break;
				 case DataBit_8:
				 Set_Bit(UCSRC,URSEL);
				 Set_Bit(UCSRC,UCSZ0);
				 Set_Bit(UCSRC,UCSZ1);
				 CLR_Bit(UCSRB,UCSZ2);
				 break;
			 }
			 //Enable
			 switch(gstr_uart_config_ch[i].enu_uart_enable)
			 {
				 case Transmitter:
				 Set_Bit(UCSRB,TXEN);
				 break;
				 case Reciever:
				 Set_Bit(UCSRB,RXEN);
				 break;
				 case TR:
				 Set_Bit(UCSRB,TXEN);
				 Set_Bit(UCSRB,RXEN);
				 break;
			 } 
		 }
		 else
		 {
			//do nothing 
		 }
	 }
	 if (i>0)
	 {
		 return Init_Done;
	 }
	 else
	 {
		 return Init_Error;
	 }
	
 }
 /*******************************************************Transmitter************************************************************************************************/
 void uart_transmit(u8 data)
 {
	 while (!Read_Bit(UCSRA,UDRE));
	 UDR=data;
 }
 
 void uart_transmitNoBlock(u8 data)
 {
	 UDR=data;
 }
 /**************************************************Transmitter Interrupt********************************************************************************************/
 void uart_transmitComPlete_InterruptEnable(void)
 {
	 Set_Bit(UCSRB,TXCIE);
 }
 void uart_transmitComPlete_InterruptDisable(void)
 {
	  CLR_Bit(UCSRB,TXCIE);
 }
 void uart_transmitComPlete_InterruptSetCallback(void(*fptr)(void))
 {
	 Tfptr=fptr;
 }
 
 ISR(UART_TX_vect)
 {
	 if (Tfptr!=NULLPTR)
	 {
		 Tfptr();
	 }
	 else
	 {
		 //do nothing
	 }
 }
 
 /*******************************************************Receiver*********************************************************************************************/
 
 u8 uart_reciever(void)
 {
	 while (!Read_Bit(UCSRA,RXC));
	 if (Read_Bit(UCSRA,FE)||Read_Bit(UCSRA,PE))
	 {
		 return ERROR_DATA;
	 }
	 else
	 {
		 return UDR;
	 }
	 
 }
 
 u8 uart_recieverNoBlock(void)
 {
	  return UDR;
 }
 
 /*******************************************Receiver Interrupt*****************************************************************************/
 
 void uart_recieveComPlete_InterruptEnable(void)
 {
	 Set_Bit(UCSRB,RXCIE);
 }
 
 void uart_recieveComPlete_InterruptDisable(void)
 {
	 CLR_Bit(UCSRB,RXCIE);
 }
 
 void uart_recieveComPlete_InterruptSetCallback(void(*fptr)(void))
 {
	 Rfptr=fptr;
 }
 
 
 
 
 ISR(UART_RX_vect)
 {
	 if (Rfptr!=NULLPTR)
	 {
		 Rfptr();
	 }
	 else
	 {
		 //do nothing
	 }
 }