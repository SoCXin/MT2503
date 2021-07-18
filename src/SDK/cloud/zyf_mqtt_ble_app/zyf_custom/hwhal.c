#include "ql_type.h"
#include "ql_trace.h"
#include "ql_uart.h"
#include "ql_system.h"
#include "ql_stdlib.h"
#include "ql_error.h"
#include "ql_gpio.h"
#include "sys.h"
#include "ql_socket.h"
#include "tiem.h"
#include "uart.h"
#include "new_socket.h"
#include "ql_adc.h"
#include "flash.h"
#include "gps.h"
#include "ril.h"
#include "hwhal.h"


void hwhalinit(void)
{
	 Ql_GPIO_Init(PINNAME_SD_CMD, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	 Ql_GPIO_Init(PINNAME_SD_CLK, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	 Ql_GPIO_Init(PINNAME_SD_DATA, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	 Ql_GPIO_Init(PINNAME_NETLIGHT, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	 Ql_GPIO_Init(PINNAME_CTS, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	 Ql_GPIO_Init(PINNAME_RTS, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	// Ql_GPIO_Init(PINNAME_DTR, PINDIRECTION_IN, 0, PINPULLSEL_PULLUP);
	 Ql_GPIO_Init(PINNAME_DCD, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
}


