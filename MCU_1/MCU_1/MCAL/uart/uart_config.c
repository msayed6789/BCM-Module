#include "uart.h"
#if UART_MODE==ASYNCH
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
