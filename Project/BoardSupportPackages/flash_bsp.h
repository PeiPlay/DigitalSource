#pragma once 
#include "main.h"
//提供内部flash的sector管理，使用该模块时不能开启编译器优化

//sram_buf的大小必须是32的整数倍，最大为128kb，这里取kb的整数倍
#define FLASHBSP_SRAMBUF_SIZE_KB 1

#define FLASHBSP_SRAMBUF_SIZE (FLASHBSP_SRAMBUF_SIZE_KB * 1024)
typedef struct
{

    __IO uint32_t sector;  
    __IO uint8_t sram_buf[FLASHBSP_SRAMBUF_SIZE];
} FlashBsp_t;

void FlashBsp_Init(FlashBsp_t* flashBsp);
void FlashBsp_Program(FlashBsp_t* flashBsp);
void FlashBsp_Write(FlashBsp_t* flashBsp, uint8_t* data, uint32_t sram_addr, uint32_t size);
void FlashBsp_Read(FlashBsp_t* flashBsp, uint8_t* data, uint32_t sram_addr, uint32_t size);
void FlashBsp_Erase(FlashBsp_t* flashBsp);
void FlashBsp_Refresh(FlashBsp_t* flashBsp);

