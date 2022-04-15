/*
 * \file 	HT16K33.h
 *		This source file includes the functions to init and manipulate the led matrix
 *
 *  Created on: 14/04/2022
 *      Author: Mauricio Peralta
 */

#ifndef HT16K33_H_
#define HT16K33_H_

#include "stdint.h"
#include "MK64F12.h"
#include "fsl_i2c.h"
#include "fsl_debug_console.h"
#include "I2C.h"
#include "Bits.h"

#define LED_MATRIX_SLAVE_ADDR	(0x70U)
#define SUB_ADDR_SIZE			(0x01U)
#define LED_MATRIX_ADDR			(0x00)

/*!
 	 \brief	 Functions to initialize LCD address, write data and make a copy of an array
 */
uint16_t Led_Matrix_config(uint8_t SLAVE_ADDR_init);
uint16_t Led_Matrix_Write(uint8_t *arr_ptr);
void Led_Matrix_Copy_Str(uint8_t *original,uint8_t *arr_ptr, uint16_t lenght);

#endif /* HT16K33_H_ */
