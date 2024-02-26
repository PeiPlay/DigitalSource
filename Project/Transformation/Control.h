#pragma once 
#include "Channel.h"
#include "main.h"

void Control_Init(void);

void Control_Callback_TIM(void);

Channel_t* Control_GetChannel(uint32_t ch_index);


