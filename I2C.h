/*
 * 	I2C.h
 *		Device driver for the I2C to use with the MCP7940M and 24LC32A
 *
 *  Created on: 14/04/2022
 *      Author: Mauricio Peralta
 */

#ifndef I2C_H_
#define I2C_H_

#include "MK64F12.h"
#include "fsl_i2c.h"

#define I2C_MASTER_CLK_FREQ         CLOCK_GetFreq(I2C0_CLK_SRC)
#define I2C_ 						I2C0
#define I2C_BAUDRATE               	1600000U

/*!
 	 \brief	 This function configures the I2C0 for its use.
 	 \param[in]  void
 	 \return 	void
 */
void I2C_config(void);

#endif /* I2C_H_ */
