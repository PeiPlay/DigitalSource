#pragma once 
#include "main.h"
//提供内部flash的sector管理
//sram_buf的大小必须是32的整数倍，最大为128kb，这里取kb的整数倍
#define FLASHBSP_SRAMBUF_SIZE_KB 1

#define FLASHBSP_SRAMBUF_SIZE (FLASHBSP_SRAMBUF_SIZE_KB * 1024)
typedef struct
{
    __IO uint32_t sector;                           //flash扇区
    __IO uint8_t sram_buf[FLASHBSP_SRAMBUF_SIZE];   //sram缓冲区
} FlashBsp_t;

void FlashBsp_Init(FlashBsp_t* flashBsp);   //初始化flash_bsp，须给出sector(FLASH_SECTOR_X)
void FlashBsp_Write(FlashBsp_t* flashBsp, uint8_t* data, uint32_t sram_addr, uint32_t size);    //将数据写入sram缓冲区，sram_addr为相对于sram_buf缓冲区的偏移地址
void FlashBsp_Read(FlashBsp_t* flashBsp, uint8_t* data, uint32_t sram_addr, uint32_t size);     //从sram缓冲区读取数据，sram_addr为相对于sram_buf缓冲区的偏移地址
void FlashBsp_Program(FlashBsp_t* flashBsp);                                                    //将sram缓冲区的数据写入flash
void FlashBsp_Erase(FlashBsp_t* flashBsp);                                                      //擦除flash并清空sram缓冲区
void FlashBsp_Refresh(FlashBsp_t* flashBsp);                                                    //将flash的数据刷新到sram缓冲区
void* FlashBsp_GetAddr(FlashBsp_t* flashBsp, uint32_t sram_addr);                               //获取相对于sram_buf的偏移地址为sram_addr的内存地址

