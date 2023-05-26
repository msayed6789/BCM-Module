
#include "uart.h"
#if UART_MODE==ASYNCH
//const str_uart_config_t  gstr_uart_config_ch_0=
//{
	//.enu_uart_baud_rate =Baudrate9600,
	//.enu_uart_speed     =Normal_Speed,
	//.enu_uart_parity    =No_Parity,
	//.enu_uart_stopbit   =StopBit_1,
	//.enu_uart_databit   =DataBit_8,
	//.enu_uart_enable    =TR
//}
//const str_uart_config_t  gstr_uart_config_ch_1=
//{
	//.enu_uart_baud_rate =Baudrate_Total,
	//.enu_uart_speed     =Speed_Total,
	//.enu_uart_parity    =Parity_Total,
	//.enu_uart_stopbit   =StopBit_Total,
	//.enu_uart_databit   =DataBit_Total,
	//.enu_uart_enable    =Enable_Total
//}

const str_uart_config_t gstr_uart_config_ch[CHANNEL_SIZE]=
{
	{
	  Baudrate9600,
	  Normal_Speed,
	  No_Parity,
	  StopBit_1,
	  DataBit_8,
	  TR
	},
	{
      Baudrate_Total,
	  Speed_Total,
	  Parity_Total,
	  StopBit_Total,
	  DataBit_Total,
	  Enable_Total
	}
};
#elif UART_MODE==SYNCH
const str_uart_config_t gstr_uart_config_ch[CHANNEL_SIZE]=
{
	{
		Normal_Speed,
		DataBit_8,
		TR
	},
	{
		Speed_Total,
		DataBit_Total,
		Enable_Total
	}
};
#endif
