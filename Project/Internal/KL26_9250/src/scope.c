#include "stdint.h"
#include "stdio.h"
#include "scope.h"
#include "uart.h"

#define SCOPE_UART_MAP		UART0_RX_PA01_TX_PA02
#define SCOPE_UART_CH		HW_UART0
#define SCOPE_BAUDRATE		(115200U)
#define SCOPE_WRITE_BYTE(a)	UART_WriteByte(HW_UART0,a)	

/**scopeInit func*/
void scopeInit(void) {
	UART_QuickInit(SCOPE_UART_MAP, SCOPE_BAUDRATE);
}
/**
line 0	data0
line 1	data1
line 2	data2
...
*/
void scopeDrawLine(int16_t data0,int16_t data1,int16_t data2) {
	uint8_t data0_L = (uint8_t)data0;
	uint8_t data0_H = (uint8_t)(data0 >> 8);
	uint8_t data1_L = (uint8_t)data1;
	uint8_t data1_H = (uint8_t)(data1 >> 8);
	uint8_t data2_L = (uint8_t)data2;
	uint8_t data2_H = (uint8_t)(data2 >> 8);
	uint8_t sum = 0x88 + 0xa1 + 0x06 + data0_H + data0_L + data1_H + data1_L + data2_H + data2_L;
		
	SCOPE_WRITE_BYTE(0x88);
	SCOPE_WRITE_BYTE(0xa1);
	SCOPE_WRITE_BYTE(0x06);	
	SCOPE_WRITE_BYTE(data0_H);
	SCOPE_WRITE_BYTE(data0_L);
	SCOPE_WRITE_BYTE(data1_H);
	SCOPE_WRITE_BYTE(data1_L);
	SCOPE_WRITE_BYTE(data2_H);
	SCOPE_WRITE_BYTE(data2_L);
	SCOPE_WRITE_BYTE(sum);
}
