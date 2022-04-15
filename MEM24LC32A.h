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
#define DATA_SIZE_MAX			99U
#define DATA_SIZE_DIGS			2U
#define HEX_LETTERS_OFFSET		10U

#define ADDRESS_H1_MASK			0xF000U
#define ADDRESS_H0_MASK			0x0F00U
#define ADDRESS_L1_MASK			0x00F0U
#define ADDRESS_L0_MASK			0x000FU
#define ADDRESS_H1_SHIFT		12U
#define ADDRESS_H0_SHIFT		8U
#define ADDRESS_L1_SHIFT		4U

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
/*!
 	 \brief	 This function converts and array to an address and verifies it is valid.
 	 \param[in]  array that stores the digits, pointer to the variable where the real value
 	 	 	 will be stored.
 	 \return true if the address is valid.
 */
uint8_t Array_to_address(uint8_t *digs_address, uint16_t *real_address);
/*!
 	 \brief	 This function converts and array to a two digits size and verifies it is valid.
 	 \param[in]  array that stores the digits, pointer to the variable where the real value
 	 	 	 	 will be stored.
 	 \return true if the size is valid.
 */
uint8_t Array_to_size(uint8_t *digs, uint8_t* real_size);

/*!
 	 \brief	 This function reads the first address, in order to make sure it is connected.
 	 \param[in]  void
 	 \return true if the k64 had trouble communicating with the EEPROM
 */
uint8_t EEPROM_verify_communication(void);
/*!

#endif /* 24LC256_H_ */
