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
/*!
 	 \brief	 This function reads data from the address device received with the specifications given.
 	 \param[in]  uint8_t dev_address = address of the device
 	 	 	 	 uint32_t subaddress = subaddress to read
 	 	 	 	 uint8_t subaddress_size = size of the subaddress, in bytes
 	 	 	 	 uint8_t *data = pointer to the array where the data will be stored
 	 	 	 	 size_t data_size = length of the data to be read (in bytes)
 	 \return 	true if the transference was successful
 */
uint8_t I2C_read(uint8_t dev_address, uint32_t subaddress, uint8_t subaddress_size, uint8_t *data, size_t data_size);
/*!
 	 \brief	 This function writes data on the address device received with the specifications given.
 	 \param[in]  uint8_t dev_address = address of the device
 	 	 	 	 uint32_t subaddress = subaddress to write
 	 	 	 	 uint8_t subaddress_size = size of the subaddress, in bytes
 	 	 	 	 uint8_t *data = pointer to the array where the data is stored
 	 	 	 	 size_t data_size = length of the data to write (in bytes)
 	 \return 	 true if the transference was successful
 */
uint8_t I2C_write(uint8_t dev_address, uint32_t subaddress, uint8_t subaddress_size, uint8_t *data, size_t data_size);
 
#endif /* I2C_H_ */
