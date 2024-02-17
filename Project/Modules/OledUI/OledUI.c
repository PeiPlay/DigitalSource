#include "OledUI.h"
#include "Config.h"
#include <stdio.h>
#include "DwtClock.h"


static I2C_HandleTypeDef* oled_hi2c;
uint8_t u8g2_gpio_and_delay_stm32(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
{
    switch(msg){		
	    case U8X8_MSG_GPIO_AND_DELAY_INIT:
	        break;
	    case U8X8_MSG_DELAY_MILLI:
		    DwtClock_DelayMs(arg_int);
	        break;
	    case U8X8_MSG_GPIO_I2C_CLOCK:		
            break;
        case U8X8_MSG_GPIO_I2C_DATA:			
            break;
	    default:	
		    return 0;
	}
	return 1; // command processed successfully.
}
uint8_t u8x8_byte_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    static uint8_t buffer[32];		/* u8g2/u8x8 will never send more than 32 bytes between START_TRANSFER and END_TRANSFER */
    static uint8_t buf_idx;
    uint8_t *data;
    
    switch(msg){
        case U8X8_MSG_BYTE_SEND:
            data = (uint8_t *)arg_ptr;      
            while( arg_int > 0 ){
				buffer[buf_idx++] = *data;
				data++;
				arg_int--;
			}      
            break;
        case U8X8_MSG_BYTE_INIT:
            /* add your custom code to init i2c subsystem */
            break;
        case U8X8_MSG_BYTE_START_TRANSFER:
            buf_idx = 0;
            break;
        case U8X8_MSG_BYTE_END_TRANSFER:
            HAL_I2C_Master_Transmit(oled_hi2c, u8x8_GetI2CAddress(u8x8), buffer, buf_idx, 0xFF);
            break;
        default:
    return 0;
    }
    return 1;
}

void OledUI_Init(OledUI_t* oled_ui)
{
    oled_hi2c = oled_ui->hi2c;
    u8g2_Setup_ssd1306_i2c_128x64_noname_f(&oled_ui->u8g2, U8G2_R0, u8x8_byte_hw_i2c,u8g2_gpio_and_delay_stm32);
    u8g2_InitDisplay(&oled_ui->u8g2);
    u8g2_SetPowerSave(&oled_ui->u8g2, 0);
    u8g2_SetContrast(&oled_ui->u8g2, 0xFF);
    u8g2_ClearDisplay(&oled_ui->u8g2);
    u8g2_ClearBuffer(&oled_ui->u8g2);
    u8g2_SendBuffer(&oled_ui->u8g2);
}
void OledUI_Display(OledUI_t* oled_ui)
{
    
    static uint32_t lasttime = 0;
    static uint32_t frame_cnt = 0;
    if(frame_cnt >= 10)
    {        
        oled_ui->fps = 10000 / (HAL_GetTick() - lasttime);
        lasttime = HAL_GetTick();
        frame_cnt = 0;
    }
    u8g2_ClearBuffer(&oled_ui->u8g2);
    oled_ui->pages[oled_ui->page_current]();
    u8g2_SendBuffer(&oled_ui->u8g2);
    frame_cnt++;
}
void OledUI_SetPageNext(OledUI_t* oled_ui)
{
    oled_ui->page_current = (oled_ui->page_current + 1) % oled_ui->page_count;
}
void OledUI_SetPagePrev(OledUI_t* oled_ui)
{
    oled_ui->page_current = (oled_ui->page_current + oled_ui->page_count - 1) % oled_ui->page_count;
}
void OledUI_SetPage(OledUI_t* oled_ui, uint8_t page)
{
    oled_ui->page_current = page % oled_ui->page_count;
}
uint8_t OledUI_GetPage(OledUI_t* oled_ui)
{
    return oled_ui->page_current;
}
void OledUI_AddPage(OledUI_t* oled_ui, OledUI_Pages_t newpage)
{
    if (oled_ui->page_count >= 32)
        return;
    oled_ui->pages[oled_ui->page_count] = newpage;
    oled_ui->page_count++;
}
void OledUI_RemovePage(OledUI_t* oled_ui, OledUI_Pages_t oldpage)
{
    uint32_t i;
    for (i = 0; i < oled_ui->page_count; i++)
    {
        if (oled_ui->pages[i] == oldpage)
        {
            oled_ui->pages[i] = NULL;
            break;
        }
    }
    for (; i < oled_ui->page_count - 1; i++)
    {
        oled_ui->pages[i] = oled_ui->pages[i + 1];
    }
    oled_ui->page_count--;
}
void u8g2_DrawProgressBar_Horizontal1(u8g2_t* u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t width, u8g2_uint_t height, float progress)
{    
    u8g2_SetDrawColor(u8g2, 1);
    u8g2_DrawRBox(u8g2, x, y, width*progress, height,0);
    u8g2_DrawRFrame(u8g2, x, y, width, height,1);
}
void u8g2_DrawProgressBar_Horizontal2(u8g2_t* u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t width, u8g2_uint_t height, float progress)
{
    u8g2_SetDrawColor(u8g2, 1);
    u8g2_DrawBox(u8g2, x, y, width*progress, height);
    u8g2_DrawFrame(u8g2, x, y, width, height);
}
void u8g2_DrawProgressBar_Horizontal3(u8g2_t* u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t width, u8g2_uint_t height, float progress)
{
    u8g2_SetDrawColor(u8g2, 1);
    u8g2_DrawBox(u8g2, x+2, y+2, width*progress-4, height-4);
    u8g2_DrawFrame(u8g2, x, y, width, height);
}

void u8g2_DrawProgressBar_HorizontalNumbered1(u8g2_t* u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t width, float progress)
{
    char buffer[6];
    u8g2_SetDrawColor(u8g2, 1);
    u8g2_SetFont(u8g2, u8g2_font_4x6_tf);
    sprintf(buffer, "%.1f%%", progress * 100);
    uint32_t str_width = u8g2_GetUTF8Width(u8g2, buffer);
    u8g2_DrawStr(u8g2, x + width / 2 - str_width / 2 + 5, y + 7, buffer);
    u8g2_SetDrawColor(u8g2, 2);
    u8g2_DrawRBox(u8g2, x, y, width*progress, 9,0);
    u8g2_SetDrawColor(u8g2, 1);
    u8g2_DrawRFrame(u8g2, x, y, width, 9,2);
}
void u8g2_DrawProgressBar_HorizontalNumbered2(u8g2_t* u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t width, float progress)
{
    char buffer[6];
    u8g2_SetDrawColor(u8g2, 1);
    u8g2_SetFont(u8g2, u8g2_font_5x8_tf);
    sprintf(buffer, "%.1f%%", progress * 100);
    uint32_t str_width = u8g2_GetUTF8Width(u8g2, buffer);
    u8g2_DrawStr(u8g2, x + width / 2 - str_width / 2, y + 8, buffer);
    u8g2_SetDrawColor(u8g2, 2);
    u8g2_DrawRBox(u8g2, x, y, width*progress, 10,0);
    u8g2_SetDrawColor(u8g2, 1);
    u8g2_DrawRFrame(u8g2, x, y, width, 10,2);
}
void u8g2_DrawProgressBar_HorizontalNumbered3(u8g2_t* u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t width, float progress)
{
    char buffer[6];
    u8g2_SetDrawColor(u8g2, 1);
    u8g2_DrawRFrame(u8g2, x, y, width, 11, 0);
    u8g2_SetFont(u8g2, u8g2_font_4x6_tf);
    sprintf(buffer, "%.1f%%", progress * 100);
    uint32_t str_width = u8g2_GetUTF8Width(u8g2, buffer);
    u8g2_DrawStr(u8g2, x + width / 2 - str_width / 2, y + 8, buffer);
    u8g2_SetDrawColor(u8g2, 2);
    u8g2_DrawBox(u8g2, x+2, y+2, width*progress-4, 7);
    u8g2_SetDrawColor(u8g2, 1);
}
void u8g2_DrawProgressBar_VerticalNumbered1(u8g2_t* u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t height, float progress)
{
    char buffer[6];
    u8g2_SetDrawColor(u8g2, 1);
    u8g2_SetFont(u8g2, u8g2_font_4x6_tf);
    sprintf(buffer, "%.0f", progress * 100);
    uint32_t str_width = u8g2_GetUTF8Width(u8g2, buffer);
    u8g2_DrawStr(u8g2, x + 11 / 2 - str_width / 2, y + height / 2 + 1, buffer);
    u8g2_SetDrawColor(u8g2, 2);
    u8g2_DrawRBox(u8g2, x, height - height*progress + y, 11, height*progress,0);
    u8g2_SetDrawColor(u8g2, 1);
    u8g2_DrawRFrame(u8g2, x, y, 11, height,0);
}
void u8g2_DrawProgressBar_VerticalNumbered2(u8g2_t* u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t height, float progress)
{
    char buffer[6];
    u8g2_SetDrawColor(u8g2, 1);
    u8g2_SetFont(u8g2, u8g2_font_4x6_tf);
    sprintf(buffer, "%.0f", progress * 100);
    uint32_t str_width = u8g2_GetUTF8Width(u8g2, buffer);
    u8g2_DrawStr(u8g2, x + 11 / 2 - str_width / 2, y + height / 2 + 1, buffer);
    u8g2_SetDrawColor(u8g2, 2);
    u8g2_DrawRBox(u8g2, x, height - height*progress + y, 11, height*progress,0);
    u8g2_SetDrawColor(u8g2, 1);
    u8g2_DrawRFrame(u8g2, x, y, 11, height,2);
}
void u8g2_DrawProgressBar_Vertical1(u8g2_t* u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t width, u8g2_uint_t height, float progress)
{
    u8g2_SetDrawColor(u8g2, 1);
    if(height * progress > 2.0f) u8g2_DrawRBox(u8g2, x, height - height * progress + y, width, height * progress, 1);
    u8g2_DrawRFrame(u8g2, x, y, width, height,2);
}
void u8g2_DrawProgressBar_Vertical2(u8g2_t* u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t width, u8g2_uint_t height, float progress)
{
    u8g2_SetDrawColor(u8g2, 1);
    u8g2_DrawBox(u8g2, x, height - height * progress + y, width, height * progress);
    u8g2_DrawFrame(u8g2, x, y, width, height);
}
void u8g2_DrawProgressBar_Vertical3(u8g2_t* u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t width, u8g2_uint_t height, float progress)
{
    u8g2_SetDrawColor(u8g2, 1);
    u8g2_DrawBox(u8g2, x, y, width, height);
    u8g2_DrawFrame(u8g2, x - 2, y - 2, width + 4, height + 4);
    u8g2_SetDrawColor(u8g2, 0);
    u8g2_DrawBox(u8g2, x, y, width, height*(1.0f - progress));
    u8g2_SetDrawColor(u8g2, 1);
}

