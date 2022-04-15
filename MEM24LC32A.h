/*
 * MEM24LC32A.h
 *		Device driver for memory
 *
 *  Created on: 14/04/2022
 *      Author: Mauricio Peralta
 */

#ifndef MEM24LC32A_H_
#define MEM24LC32A_H_

#include "MK64F12.h"
#define EEPROM_ADDRESS			0x50U
#define EEPROM_SUBA_SIZE		2U
#define EEPROM_MAX_ADDRESS		0x7FFFU
#define ADRESS_DIGS_MAX			6U


typedef struct
{
	uint16_t address;
	uint8_t  dataSize;
	uint8_t  *data;
}eeprom_transfer_t;

/*!
 	 \brief	 This function configures the EEPROM writing on the first address, in order to make
 	 	 	 sure it is connected.
 	 \param[in]  void
 	 \return true if the k64 had trouble communicating with the EEPROM
 */
uint8_t EEPROM_config(void);

/*!
 	 \brief	 This function writes the specified address of the memory.
 	 \param[in]  transfer buffer, that includes the address, data and data size.
 	 \return true if the k64 had trouble communicating with the EEPROM
 */
uint8_t EEPROM_write(eeprom_transfer_t transfer);
/*!
 	 \brief	 This function reads the specified address of the memory.
 	 \param[in]  transfer buffer, that includes the address, data and data size.
 	 \return true if the k64 had trouble communicating with the EEPROM
 */
uint8_t EEPROM_read(eeprom_transfer_t transfer);
