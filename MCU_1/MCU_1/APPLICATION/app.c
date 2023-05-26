 

#include "app.h"

u8 str[]="BCM Operating";
u8 str2[30]={0};
	u32 f=0;
u8 str_Check[]="Confirm BCM Operating";

u8 complete_flag=0;

static void BCM_Send_End(void)
{
	LED_Toggle(Send_LED);
}
static void BCM_recieve_End(void)
{
	u8 i=0;
	u8 check_flag=0;
	for (i=0;str_Check[i];i++)
	{
		if (str_Check[i]!=str2[i])
		{
			check_flag=1;
		}
		else
		{
			//do nothing
		}
	}
	if (check_flag==0)
	{
		LED_Toggle(Recieve_LED);
		
	}
	else
	{
		//do nothing
	}
}

void app_init(void)
{
	sei();
	LED_init(Send_LED);
	LED_init(Recieve_LED);
	bcm_init(uart);
}
void app_start(void)
{
	bcm_send_Non_Blocking(str,13,0);
	bcm_send_End_Setcallback(BCM_Send_End);
	bcm_recieve_Non_Blocking(str2);
	bcm_recieve_End_Setcallback(BCM_recieve_End);
	while(1)
	{
		uart_bcm_send_dispatcher();
		uart_bcm_recieve_dispatcher();	
	}
	
}
