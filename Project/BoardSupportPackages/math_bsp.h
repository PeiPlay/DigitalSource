#pragma once 
#include "main.h"

#define MathBsp_Abs(x) ((x) < 0 ? -(x) : (x))
#define MathBsp_Min(x, y) ((x) < (y) ? (x) : (y))
#define MathBsp_Max(x, y) ((x) > (y) ? (x) : (y))
#define MathBsp_Clamp(x, min, max) MathBsp_Min(MathBsp_Max((x), (min)), (max))


const uint16_t* MathBsp_Crc16Table(void);
uint16_t MathBsp_Crc16(void* pdata, uint32_t byte_len);


