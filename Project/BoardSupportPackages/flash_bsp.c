#include "flash_bsp.h"
#define SIZE_OF_FLASHWORD 32   //每个flash word是32byte
void FlashBsp_Refresh(FlashBsp_t* flashBsp)
{
	HAL_FLASH_Unlock();  
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_SNECCERR | FLASH_FLAG_WRPERR | FLASH_FLAG_BSY);
    uint8_t* from = (uint8_t*)(0x08000000 + (uint32_t)flashBsp->sector*128*1024 + 127*1024);
    for(uint32_t i = 0; i < 1024; i++)
    {
        flashBsp->sram_buf[i] = from[i];
    }
	HAL_FLASH_Lock();
}
//H750有8个128kb的sector, 每个sector有4k个flash word, 每个flash word是32byte
void FlashBsp_Init(FlashBsp_t* flashBsp)
{
    FlashBsp_Refresh(flashBsp);
}


void FlashBsp_Program(FlashBsp_t* flashBsp)
{
    volatile uint8_t* dest = (uint8_t*)(0x08000000 + (uint32_t)flashBsp->sector*128*1024 + 127*1024);
	HAL_FLASH_Unlock();               //解锁Flash
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_SNECCERR | FLASH_FLAG_WRPERR | FLASH_FLAG_BSY);
    for(uint32_t i = 0; i < 1024 / SIZE_OF_FLASHWORD; i++)
    {
		if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, (uint32_t)(dest + i * SIZE_OF_FLASHWORD), (uint32_t)(flashBsp->sram_buf + i * SIZE_OF_FLASHWORD)) != HAL_OK) 
		{
            uint8_t row_index = FLASH_NB_32BITWORD_IN_FLASHWORD;
			__IO uint32_t *dest_addr = (__IO uint32_t *)(uint32_t)(dest + i * SIZE_OF_FLASHWORD);
			__IO uint32_t *src_addr = (__IO uint32_t*)(uint32_t)(flashBsp->sram_buf + i * SIZE_OF_FLASHWORD);
			uint32_t bank = FLASH_BANK_1;
			HAL_StatusTypeDef status = FLASH_WaitForLastOperation((uint32_t)50000U, bank);
            SET_BIT(FLASH->CR1, FLASH_CR_PG);
            __ISB();
            __DSB();
            do
            {
                *dest_addr = *src_addr;
                dest_addr++;
                src_addr++;
                row_index--;
            } while (row_index != 0U);
            status = FLASH_WaitForLastOperation((uint32_t)50000U, bank);
            CLEAR_BIT(FLASH->CR1, FLASH_CR_PG);
            __HAL_UNLOCK(&pFlash);
		}
	}	
//	for(uint32_t i = 0; i < 1024; i++)
//	{
//		*(dest + i) = flashBsp->sram_buf[i];
//	}
	
	HAL_FLASH_Lock();
	FlashBsp_Refresh(flashBsp);
}
void FlashBsp_Write(FlashBsp_t* flashBsp, uint8_t* data, uint32_t sram_addr, uint32_t size)
{
    if(sram_addr + size > 1024)
        Error_Handler();
    for(uint32_t i = 0; i < size; i++)
    {
        flashBsp->sram_buf[sram_addr + i] = data[i];
    }

}
void FlashBsp_Read(FlashBsp_t* flashBsp, uint8_t* data, uint32_t sram_addr, uint32_t size)
{
    if(sram_addr + size > 1024)
        Error_Handler();
    for(uint32_t i = 0; i < size; i++)
    {
        data[i] = flashBsp->sram_buf[sram_addr + i];
    }
}

