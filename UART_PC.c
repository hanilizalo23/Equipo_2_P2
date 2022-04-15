/*
 * \file 	UART_PC.c
 *		This is the source file to control the Serial Port
**		connected from the k64 to the PC
**		
 *		
 *  Created on: 14/04/2022
 *      Author: Nelida Hernández
 */

#include "UART_PC.h"
#include "MK64F12.h"
#include "stdbool.h"
#include "fsl_uart.h"
#include "fsl_port.h"
#include "Bits.h"

#define NOTHING 0U
#define ONE_LENGHT 1U

#define UART0_RX_PIN	(16U)
#define UART0_TX_PIN	(17U)

uart_handle_t g_uartPCHandle;
static size_t g_receivedBytes;
static uint8_t g_rxRingBuffer[RX_RING_BUFFER_SIZE] = {NOTHING}; /* RX ring buffer. */
static uart_transfer_t g_rxbuffer;

//Flag to know when a character has been received
volatile uint8_t g_rx_flag = false;

uint8_t AM_str [] = " am\n";
uint8_t PM_str [] = " pm\n";

void UART_PC_callback(UART_Type *base, uart_handle_t *handle, status_t status, void *userData)
{
	if(kStatus_UART_RxIdle == status)
	{
		//The flag value changes when the interruption enters
		g_rx_flag = true;
	}
}

void UART_PC_configure_port(void)
{
	uart_config_t uart_config;

	/* Port B Clock Gate Control: Clock enabled */
	CLOCK_EnableClock(kCLOCK_PortB);

	/* PORTB16 (pin 62) is configured as UART0_RX */
	PORT_SetPinMux(PORTB, UART0_RX_PIN, kPORT_MuxAlt3);

	/* PORTB17 (pin 63) is configured as UART0_TX */
	PORT_SetPinMux(PORTB, UART0_TX_PIN, kPORT_MuxAlt3);

	//Get default configuration for UART
	UART_GetDefaultConfig(&uart_config);
	uart_config.baudRate_Bps = BAUD_RATE_PC;
	uart_config.enableTx = true;
	uart_config.enableRx = true;

	//Configure UART
	UART_Init(UART_PC, &uart_config, UART_CLK_FREQ_PC);
	UART_TransferCreateHandle(UART_PC, &g_uartPCHandle, UART_PC_callback, NULL);
	UART_TransferStartRingBuffer(UART_PC, &g_uartPCHandle, g_rxRingBuffer, RX_RING_BUFFER_SIZE);
}

void UART_PC_write(uint8_t *data, size_t length)
{
	UART_WriteBlocking(UART_PC, data, length);
}

void UART_PC_read(uint8_t *data, size_t length)
{
	g_rxbuffer.data = data;
	g_rxbuffer.dataSize = length;
	UART_TransferReceiveNonBlocking(UART_PC, &g_uartPCHandle, &g_rxbuffer, &g_receivedBytes);
}

uint8_t UART_PC_get_flag(void)
{
	return(g_rx_flag);
}

void UART_PC_clear_flag(void)
{
	g_rx_flag = false;
}

void UART_PC_print_time(time_store_t time)
{

}

void UART_PC_print_date(date_store_t date)
{

}
