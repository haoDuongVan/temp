/*
 * epprom.h
 *
 *  Created on: Mar 11, 2024
 *      Author: Admin
 */

#ifndef EPPROM_EPPROM_H_
#define EPPROM_EPPROM_H_
// Include section
#include "stm32g0xx_hal.h"
#include "stm32g0xx_hal_flash.h"
#include <stdint.h>

// Macro define
#define DOUBLE_WORD_SIZE 8
#define NULL_WORD 0xFFFF

#define USER_LOG1_START_ADDRESS 	0x0807F000UL // Bank 382 start
#define USER_LOG1_END_ADDRESS 		0x0807F7FFUL // Bank 382 end
#define USER_LOG2_START_ADDRESS 	0x0807F800UL // Bank 383 start
#define USER_LOG2_END_ADDRESS 		0x0807FFFFUL // Bank 383 end

#define EMPTY_SPACE					0xFFFFFFFF
#define NO_EMPTY_SPACE				0x0

void ee_unlock(void);

void ee_lock(void);

uint32_t ee_erase(uint32_t Address);

uint32_t ee_write(uint32_t Address, uint8_t *data, uint32_t len);

void ee_read(uint32_t Address, uint8_t *data, uint32_t len);

uint32_t GetPage(uint32_t Address);

uint32_t GetBank(uint32_t Address);

uint32_t get_empty_space(void);

#endif /* EPPROM_EPPROM_H_ */
