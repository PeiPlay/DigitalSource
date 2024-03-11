#include "flash_bsp.h"
#define SIZE_OF_FLASHWORD 32   //每个flash word是32byte

void FlashBsp_Refresh(FlashBsp_t* flashBsp)
{	
    HAL_FLASH_Unlock();
    __IO uint8_t* from = (uint8_t*)(0x08000000 + (uint32_t)(flashBsp->sector)*128*1024);
    for(uint32_t i = 0; i < FLASHBSP_SRAMBUF_SIZE; i++)
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
    HAL_FLASH_Unlock();
    FLASH_EraseInitTypeDef EraseInitStruct;
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.Sector = flashBsp->sector;
    EraseInitStruct.NbSectors = 1;
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
	EraseInitStruct.Banks = FLASH_BANK_1;
    uint32_t SectorError = 0;
    if(HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)
    {
        Error_Handler();
    }
	FLASH_WaitForLastOperation(1000, FLASH_BANK_1);
    __IO uint8_t* dest = (uint8_t*)(0x08000000 + (uint32_t)(flashBsp->sector)*128*1024);
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_SNECCERR | FLASH_FLAG_WRPERR | FLASH_FLAG_BSY);
    for(uint32_t i = 0; i < FLASHBSP_SRAMBUF_SIZE / SIZE_OF_FLASHWORD; i++)
    {
		if(HAL_FLASH_Program(
            FLASH_TYPEPROGRAM_FLASHWORD, 
            (uint32_t)(dest + i * SIZE_OF_FLASHWORD), 
            (uint32_t)(flashBsp->sram_buf + i * SIZE_OF_FLASHWORD)) != HAL_OK && 
           HAL_FLASH_Program(
            FLASH_TYPEPROGRAM_FLASHWORD, 
            (uint32_t)(dest + i * SIZE_OF_FLASHWORD), 
            (uint32_t)(flashBsp->sram_buf + i * SIZE_OF_FLASHWORD)  != HAL_OK))
		{
            Error_Handler();
		}
	}
	HAL_FLASH_Lock();
	
	FlashBsp_Refresh(flashBsp);
}
void FlashBsp_Write(FlashBsp_t* flashBsp, uint8_t* data, uint32_t sram_addr, uint32_t size)
{
    if(sram_addr + size > FLASHBSP_SRAMBUF_SIZE)
        Error_Handler();
    for(uint32_t i = 0; i < size; i++)
    {
        flashBsp->sram_buf[sram_addr + i] = data[i];
    }

}
void FlashBsp_Read(FlashBsp_t* flashBsp, uint8_t* data, uint32_t sram_addr, uint32_t size)
{
    if(sram_addr + size > FLASHBSP_SRAMBUF_SIZE)
        Error_Handler();
    for(uint32_t i = 0; i < size; i++)
    {
        data[i] = flashBsp->sram_buf[sram_addr + i];
    }
}

void FlashBsp_Erase(FlashBsp_t* flashBsp)
{
    HAL_FLASH_Unlock();
    FLASH_EraseInitTypeDef EraseInitStruct;
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.Sector = flashBsp->sector;
    EraseInitStruct.NbSectors = 1;
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
	EraseInitStruct.Banks = FLASH_BANK_1;
    uint32_t SectorError = 0;
    if(HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)
    {
        Error_Handler();
    }
    HAL_FLASH_Lock();
    FlashBsp_Refresh(flashBsp);
}

void* FlashBsp_GetAddr(FlashBsp_t* flashBsp, uint32_t sram_addr)
{
    return (void*)&flashBsp->sram_buf[sram_addr];
}
