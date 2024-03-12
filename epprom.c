/*
 * epprom.c
 *
 *  Created on: Mar 11, 2024
 *      Author: Admin
 */
#include "epprom.h"

void ee_unlock(void)
{
	HAL_FLASH_Unlock();
}

void ee_lock(void)
{
	HAL_FLASH_Lock();
}

uint32_t ee_erase(uint32_t Address)
{
	uint32_t PageError;
	FLASH_EraseInitTypeDef eraseInit;

	/* Clear OPTVERR bit set on virgin samples */
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);

	eraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
	eraseInit.Banks = GetBank(Address);
	eraseInit.Page = GetPage(Address);
	eraseInit.NbPages = 1;

	if (HAL_FLASHEx_Erase(&eraseInit, &PageError) != HAL_OK) {
		return HAL_FLASH_GetError();
	}
	return  HAL_OK;
}

uint32_t ee_write(uint32_t Address, uint8_t *data, uint32_t len)
{
	for (uint32_t i = 0; i < len; i+= 8) {
		uint8_t DoubleWord[8] = { data[i + 0], data[i + 1], data[i + 2], data[i + 3], data[i + 4], data[i + 5], data[i + 6], data[i + 7] };
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, Address + i, (uint64_t)DoubleWord) != HAL_OK)
		{
		   HAL_FLASH_Lock();
		   return HAL_FLASH_GetError();
		}
		DoubleWord[8] = 0;
	}
	return  HAL_OK;
}

void ee_read(uint32_t Address, uint8_t *data, uint32_t len)
{
	for (uint32_t i = 0; i < len; i++) {
		*(data + i) = *(__IO uint8_t *)(Address + i);
	}
}

uint32_t get_empty_space(void)
{
	for (uint32_t Address = USER_LOG1_START_ADDRESS; Address < USER_LOG2_END_ADDRESS; Address += 8) {
		if(*(__IO uint64_t *)Address == EMPTY_SPACE) {
			return Address;
		}
	}
	return NO_EMPTY_SPACE;
}

uint32_t GetPage(uint32_t Address)
{
	uint32_t page = 0;

	if (Address < (FLASH_BASE + FLASH_BANK_SIZE))
	{
	    /* Bank 1 */
		page = (Address - FLASH_BASE) / FLASH_PAGE_SIZE;
	}
	else
	{
	    /* Bank 2 */
	    page = (Address - (FLASH_BASE + FLASH_BANK_SIZE)) / FLASH_PAGE_SIZE + 1;
	}

	return page;
}

uint32_t GetBank(uint32_t Address)
{
	if (Address < FLASH_BASE + FLASH_BANK_SIZE)
	{
		return FLASH_BANK_1;
	}
	else
	{
		return FLASH_BANK_2;
	}
}
