#pragma once 
#include "main.h"

//提供内部flash的sector管理


typedef struct
{
    uint32_t sector;
    uint8_t sram_buf[1024];
} FlashBsp_t;

void FlashBsp_Init(FlashBsp_t* flashBsp);
void FlashBsp_Program(FlashBsp_t* flashBsp);
void FlashBsp_Write(FlashBsp_t* flashBsp, uint8_t* data, uint32_t sram_addr, uint32_t size);
void FlashBsp_Read(FlashBsp_t* flashBsp, uint8_t* data, uint32_t sram_addr, uint32_t size);
void FlashBsp_Erase(FlashBsp_t* flashBsp);
void FlashBsp_Refresh(FlashBsp_t* flashBsp);




