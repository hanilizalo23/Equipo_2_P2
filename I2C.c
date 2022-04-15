/*
 * 	I2C.h
 *		Device driver for the I2C to use with the MCP7940M and 24LC32A
 *
 *  Created on: 14/04/2022
 *      Author: Mauricio Peralta
 */

#include "I2C.h"
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"
#include "Bits.h"
#include "fsl_i2c.h"
#include "stdbool.h"

i2c_master_transfer_t g_master_transfer;

void I2C_config(void)
{
	i2c_master_config_t masterConfig;
	CLOCK_EnableClock(kCLOCK_PortB);

	const port_pin_config_t porte24_pin31_config = {/* Internal pull-up resistor is enabled */
													kPORT_PullUp,
													/* Fast slew rate is configured */
													kPORT_FastSlewRate,
													/* Passive filter is disabled */
													kPORT_PassiveFilterDisable,
													/* Open drain is enabled */
													kPORT_OpenDrainEnable,
													/* Low drive strength is configured */
													kPORT_LowDriveStrength,
													/* Pin is configured as I2C0_SCL */
													kPORT_MuxAlt2,
													/* Pin Control Register fields [15:0] are not locked */
													kPORT_UnlockRegister};

	/* PORTB2 is configured as I2C0_SCL */
	PORT_SetPinConfig(PORTB, 2U, &porte24_pin31_config);

	const port_pin_config_t porte25_pin32_config = {/* Internal pull-up resistor is enabled */
													kPORT_PullUp,
													/* Fast slew rate is configured */
													kPORT_FastSlewRate,
													/* Passive filter is disabled */
													kPORT_PassiveFilterDisable,
													/* Open drain is enabled */
													kPORT_OpenDrainEnable,
													/* Low drive strength is configured */
													kPORT_LowDriveStrength,
													/* Pin is configured as I2C0_SDA */
													kPORT_MuxAlt2,
													/* Pin Control Register fields [15:0] are not locked */
													kPORT_UnlockRegister};

	/* PORTB3 is configured as I2C0_SDA */
	PORT_SetPinConfig(PORTB, 3U, &porte25_pin32_config);

	I2C_MasterGetDefaultConfig(&masterConfig);
	masterConfig.baudRate_Bps = I2C_BAUDRATE;

	I2C_MasterInit(I2C0, &masterConfig, I2C_MASTER_CLK_FREQ);
}
