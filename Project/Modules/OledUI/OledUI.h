//#pragma once
//#include "main.h"
//#include "u8g2.h"
//#include "i2c.h"

//// 函数指针
//typedef void (*OledUI_Pages_t)(void);

//typedef struct
//{
//    I2C_HandleTypeDef* hi2c;  // I2C句柄
//    struct {
//        GPIO_TypeDef* port;  // GPIO端口
//        uint16_t pin;        // GPIO引脚
//    } gpio_reset;
//    u8g2_t u8g2;              // u8g2结构体
//    OledUI_Pages_t pages[32]; // 页面函数指针组
//    uint8_t page_count;       // 页面数量
//    uint8_t page_current;     // 当前页面
//    uint32_t fps;             // 屏幕刷新率
//    uint16_t addr;
//} OledUI_t;

//void OledUI_Init(OledUI_t *oled_ui);
//void OledUI_SetPageNext(OledUI_t *oled_ui);
//void OledUI_SetPagePrev(OledUI_t *oled_ui);
//void OledUI_SetPage(OledUI_t *oled_ui, uint8_t page);
//uint8_t OledUI_GetPage(OledUI_t *oled_ui);
//void OledUI_AddPage(OledUI_t *oled_ui, OledUI_Pages_t newpage);
//void OledUI_RemovePage(OledUI_t *oled_ui, OledUI_Pages_t oldpage);
//void OledUI_Display(OledUI_t *oled_ui);

//void u8g2_DrawProgressBar_Horizontal1(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t width, u8g2_uint_t height, float progress);
//void u8g2_DrawProgressBar_Horizontal2(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t width, u8g2_uint_t height, float progress);
//void u8g2_DrawProgressBar_Horizontal3(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t width, u8g2_uint_t height, float progress);
//void u8g2_DrawProgressBar_HorizontalNumbered1(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t width, float progress);
//void u8g2_DrawProgressBar_HorizontalNumbered2(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t width, float progress);
//void u8g2_DrawProgressBar_HorizontalNumbered3(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t width, float progress);
//void u8g2_DrawProgressBar_VerticalNumbered1(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t height, float progress);
//void u8g2_DrawProgressBar_VerticalNumbered2(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t height, float progress);
//void u8g2_DrawProgressBar_Vertical1(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t width, u8g2_uint_t height, float progress);
//void u8g2_DrawProgressBar_Vertical2(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t width, u8g2_uint_t height, float progress);
//void u8g2_DrawProgressBar_Vertical3(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t width, u8g2_uint_t height, float progress);

