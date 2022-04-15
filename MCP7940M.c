/*
 * MCP7940M.c
 *		Device driver for the Real-Time Clock/Calendar with SRAM
 *
 *  Created on: 14/04/2022
 *      Author: Mauricio Peralta
 */

#include "MCP7940M.h"
#include <stdio.h>
#include "board.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"
#include "Bits.h"
#include "fsl_i2c.h"

const uint8_t g_address_time[3] =
{
		0x00U,
		0x01U,
		0x02U
};
const uint8_t g_address_date[3] =
{
		0x04U,
		0x05U,
		0x06U
};
