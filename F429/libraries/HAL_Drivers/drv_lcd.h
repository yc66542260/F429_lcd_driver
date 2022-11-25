/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-01-08     zylx         first version
 * 2022-11-25     YeC          add 5 inch LCD screen driver
 */

#ifndef __DRV_LCD_H__
#define __DRV_LCD_H__

/* fire 5.0 inch screen, 800 * 480 */
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

// LCD设备基本参数
struct drv_lcd_device
{
    struct rt_device parent;
    struct rt_device_graphic_info lcd_info;
    struct rt_device_graphic_ops lcd_ops;
    struct rt_device_rect_info lcd_rect_info;

    rt_uint8_t lcd_hbp;
    rt_uint8_t lcd_vbp;

    rt_uint8_t lcd_hsw;
    rt_uint8_t lcd_vsw;

    rt_uint8_t lcd_hfp;
    rt_uint8_t lcd_vfp;

    rt_uint8_t clock_2byte;
    rt_uint8_t clock_4byte;

    struct rt_semaphore lcd_lock;

    /* 0:front_buf is being used 1: back_buf is being used*/
    rt_uint8_t cur_buf;
    rt_uint8_t *front_buf;
    rt_uint8_t *back_buf;
};
typedef struct drv_lcd_device * drv_lcd_dev_t;
typedef struct drv_lcd_device drv_lcd_dev;


// LCD 液晶屏尺寸
typedef enum
{
    INCH_5,
    INCH_7,
    LCD_TYPE_NUM
} LCD_TypeDef;

// 外部定义
extern LCD_TypeDef cur_lcd;

extern const drv_lcd_dev lcd_param[];

#define LCD_WIDTH           lcd_param[cur_lcd].lcd_info.width
#define LCD_HEIGHT          lcd_param[cur_lcd].lcd_info.height
#define LCD_BITS_PER_PIXEL  lcd_param[cur_lcd].lcd_info.bits_per_pixel
#define LCD_BUF_SIZE        lcd_param[cur_lcd].lcd_info.smem_len
#define LCD_PIXEL_FORMAT    lcd_param[cur_lcd].lcd_info.pixel_format

#define LCD_HSYNC_WIDTH     lcd_param[cur_lcd].lcd_hsw
#define LCD_VSYNC_HEIGHT    lcd_param[cur_lcd].lcd_vsw
#define LCD_HBP             lcd_param[cur_lcd].lcd_hbp
#define LCD_VBP             lcd_param[cur_lcd].lcd_vbp
#define LCD_HFP             lcd_param[cur_lcd].lcd_hfp
#define LCD_VFP             lcd_param[cur_lcd].lcd_vfp

// LCD屏幕颜色相关
/**
  * @brief  LCD color
  */
#define LCD_COLOR_BLUE          ((uint32_t)0xFF0000FF)
#define LCD_COLOR_GREEN         ((uint32_t)0xFF00FF00)
#define LCD_COLOR_RED           ((uint32_t)0xFFFF0000)
#define LCD_COLOR_CYAN          ((uint32_t)0xFF00FFFF)
#define LCD_COLOR_MAGENTA       ((uint32_t)0xFFFF00FF)
#define LCD_COLOR_YELLOW        ((uint32_t)0xFFFFFF00)
#define LCD_COLOR_LIGHTBLUE     ((uint32_t)0xFF8080FF)
#define LCD_COLOR_LIGHTGREEN    ((uint32_t)0xFF80FF80)
#define LCD_COLOR_LIGHTRED      ((uint32_t)0xFFFF8080)
#define LCD_COLOR_LIGHTCYAN     ((uint32_t)0xFF80FFFF)
#define LCD_COLOR_LIGHTMAGENTA  ((uint32_t)0xFFFF80FF)
#define LCD_COLOR_LIGHTYELLOW   ((uint32_t)0xFFFFFF80)
#define LCD_COLOR_DARKBLUE      ((uint32_t)0xFF000080)
#define LCD_COLOR_DARKGREEN     ((uint32_t)0xFF008000)
#define LCD_COLOR_DARKRED       ((uint32_t)0xFF800000)
#define LCD_COLOR_DARKCYAN      ((uint32_t)0xFF008080)
#define LCD_COLOR_DARKMAGENTA   ((uint32_t)0xFF800080)
#define LCD_COLOR_DARKYELLOW    ((uint32_t)0xFF808000)
#define LCD_COLOR_WHITE         ((uint32_t)0xFFFFFFFF)
#define LCD_COLOR_LIGHTGRAY     ((uint32_t)0xFFD3D3D3)
#define LCD_COLOR_GRAY          ((uint32_t)0xFF808080)
#define LCD_COLOR_DARKGRAY      ((uint32_t)0xFF404040)
#define LCD_COLOR_BLACK         ((uint32_t)0xFF000000)
#define LCD_COLOR_BROWN         ((uint32_t)0xFFA52A2A)
#define LCD_COLOR_ORANGE        ((uint32_t)0xFFFFA500)
#define LCD_COLOR_TRANSPARENT   ((uint32_t)0xFF000000)


/**
  * @brief LCD default font
  */

//ºìÉ«Êý¾ÝÏß
#define LTDC_R0_GPIO_PORT           GPIOH
#define LTDC_R0_GPIO_CLK_ENABLE()   __GPIOH_CLK_ENABLE()
#define LTDC_R0_GPIO_PIN            GPIO_PIN_2
#define LTDC_R0_AF                  GPIO_AF14_LTDC

#define LTDC_R1_GPIO_PORT           GPIOH
#define LTDC_R1_GPIO_CLK_ENABLE()   __GPIOH_CLK_ENABLE()
#define LTDC_R1_GPIO_PIN            GPIO_PIN_3
#define LTDC_R1_AF                  GPIO_AF14_LTDC

#define LTDC_R2_GPIO_PORT           GPIOH
#define LTDC_R2_GPIO_CLK_ENABLE()   __GPIOH_CLK_ENABLE()
#define LTDC_R2_GPIO_PIN            GPIO_PIN_8
#define LTDC_R2_AF                  GPIO_AF14_LTDC

#define LTDC_R3_GPIO_PORT           GPIOB
#define LTDC_R3_GPIO_CLK_ENABLE()   __GPIOB_CLK_ENABLE()
#define LTDC_R3_GPIO_PIN            GPIO_PIN_0
#define LTDC_R3_AF                  GPIO_AF9_LTDC

#define LTDC_R4_GPIO_PORT           GPIOA
#define LTDC_R4_GPIO_CLK_ENABLE()   __GPIOA_CLK_ENABLE()
#define LTDC_R4_GPIO_PIN            GPIO_PIN_11
#define LTDC_R4_AF                  GPIO_AF14_LTDC

#define LTDC_R5_GPIO_PORT           GPIOA
#define LTDC_R5_GPIO_CLK_ENABLE()   __GPIOA_CLK_ENABLE()
#define LTDC_R5_GPIO_PIN            GPIO_PIN_12
#define LTDC_R5_AF                  GPIO_AF14_LTDC

#define LTDC_R6_GPIO_PORT           GPIOB
#define LTDC_R6_GPIO_CLK_ENABLE()   __GPIOB_CLK_ENABLE()
#define LTDC_R6_GPIO_PIN            GPIO_PIN_1
#define LTDC_R6_AF                  GPIO_AF9_LTDC

#define LTDC_R7_GPIO_PORT           GPIOG
#define LTDC_R7_GPIO_CLK_ENABLE()   __GPIOG_CLK_ENABLE()
#define LTDC_R7_GPIO_PIN            GPIO_PIN_6
#define LTDC_R7_AF                  GPIO_AF14_LTDC
//ÂÌÉ«Êý¾ÝÏß
#define LTDC_G0_GPIO_PORT           GPIOE
#define LTDC_G0_GPIO_CLK_ENABLE()   __GPIOE_CLK_ENABLE()
#define LTDC_G0_GPIO_PIN            GPIO_PIN_5
#define LTDC_G0_AF                  GPIO_AF14_LTDC

#define LTDC_G1_GPIO_PORT           GPIOE
#define LTDC_G1_GPIO_CLK_ENABLE()   __GPIOE_CLK_ENABLE()
#define LTDC_G1_GPIO_PIN            GPIO_PIN_6
#define LTDC_G1_AF                  GPIO_AF14_LTDC

#define LTDC_G2_GPIO_PORT           GPIOH
#define LTDC_G2_GPIO_CLK_ENABLE()   __GPIOH_CLK_ENABLE()
#define LTDC_G2_GPIO_PIN            GPIO_PIN_13
#define LTDC_G2_AF                  GPIO_AF14_LTDC

#define LTDC_G3_GPIO_PORT           GPIOG
#define LTDC_G3_GPIO_CLK_ENABLE()   __GPIOG_CLK_ENABLE()
#define LTDC_G3_GPIO_PIN            GPIO_PIN_10
#define LTDC_G3_AF                  GPIO_AF9_LTDC

#define LTDC_G4_GPIO_PORT           GPIOH
#define LTDC_G4_GPIO_CLK_ENABLE()   __GPIOH_CLK_ENABLE()
#define LTDC_G4_GPIO_PIN            GPIO_PIN_15
#define LTDC_G4_AF                  GPIO_AF14_LTDC

#define LTDC_G5_GPIO_PORT           GPIOI
#define LTDC_G5_GPIO_CLK_ENABLE()   __GPIOI_CLK_ENABLE()
#define LTDC_G5_GPIO_PIN            GPIO_PIN_0
#define LTDC_G5_AF                  GPIO_AF14_LTDC

#define LTDC_G6_GPIO_PORT           GPIOC
#define LTDC_G6_GPIO_CLK_ENABLE()   __GPIOC_CLK_ENABLE()
#define LTDC_G6_GPIO_PIN            GPIO_PIN_7
#define LTDC_G6_AF                  GPIO_AF14_LTDC

#define LTDC_G7_GPIO_PORT           GPIOI
#define LTDC_G7_GPIO_CLK_ENABLE()   __GPIOI_CLK_ENABLE()
#define LTDC_G7_GPIO_PIN            GPIO_PIN_2
#define LTDC_G7_AF                  GPIO_AF14_LTDC

//À¶É«Êý¾ÝÏß
#define LTDC_B0_GPIO_PORT           GPIOE
#define LTDC_B0_GPIO_CLK_ENABLE()   __GPIOE_CLK_ENABLE()
#define LTDC_B0_GPIO_PIN            GPIO_PIN_4
#define LTDC_B0_AF                  GPIO_AF14_LTDC

#define LTDC_B1_GPIO_PORT           GPIOG
#define LTDC_B1_GPIO_CLK_ENABLE()   __GPIOG_CLK_ENABLE()
#define LTDC_B1_GPIO_PIN            GPIO_PIN_12
#define LTDC_B1_AF                  GPIO_AF14_LTDC

#define LTDC_B2_GPIO_PORT           GPIOD
#define LTDC_B2_GPIO_CLK_ENABLE()   __GPIOD_CLK_ENABLE()
#define LTDC_B2_GPIO_PIN            GPIO_PIN_6
#define LTDC_B2_AF                  GPIO_AF14_LTDC

#define LTDC_B3_GPIO_PORT           GPIOG
#define LTDC_B3_GPIO_CLK_ENABLE()   __GPIOD_CLK_ENABLE()
#define LTDC_B3_GPIO_PIN            GPIO_PIN_11
#define LTDC_B3_AF                  GPIO_AF14_LTDC

#define LTDC_B4_GPIO_PORT           GPIOI
#define LTDC_B4_GPIO_CLK_ENABLE()   __GPIOI_CLK_ENABLE()
#define LTDC_B4_GPIO_PIN            GPIO_PIN_4
#define LTDC_B4_AF                  GPIO_AF14_LTDC

#define LTDC_B5_GPIO_PORT           GPIOA
#define LTDC_B5_GPIO_CLK_ENABLE()   __GPIOA_CLK_ENABLE()
#define LTDC_B5_GPIO_PIN            GPIO_PIN_3
#define LTDC_B5_AF                  GPIO_AF14_LTDC

#define LTDC_B6_GPIO_PORT           GPIOB
#define LTDC_B6_GPIO_CLK_ENABLE()   __GPIOB_CLK_ENABLE()
#define LTDC_B6_GPIO_PIN            GPIO_PIN_8
#define LTDC_B6_AF                  GPIO_AF14_LTDC

#define LTDC_B7_GPIO_PORT           GPIOB
#define LTDC_B7_GPIO_CLK_ENABLE()   __GPIOB_CLK_ENABLE()
#define LTDC_B7_GPIO_PIN            GPIO_PIN_9
#define LTDC_B7_AF                  GPIO_AF14_LTDC

//¿ØÖÆÐÅºÅÏß
/*ÏñËØÊ±ÖÓCLK*/
#define LTDC_CLK_GPIO_PORT              GPIOG
#define LTDC_CLK_GPIO_CLK_ENABLE()      __GPIOG_CLK_ENABLE()
#define LTDC_CLK_GPIO_PIN               GPIO_PIN_7
#define LTDC_CLK_AF                     GPIO_AF14_LTDC
/*Ë®Æ½Í¬²½ÐÅºÅHSYNC*/
#define LTDC_HSYNC_GPIO_PORT            GPIOI
#define LTDC_HSYNC_GPIO_CLK_ENABLE()    __GPIOI_CLK_ENABLE()
#define LTDC_HSYNC_GPIO_PIN             GPIO_PIN_10
#define LTDC_HSYNC_AF                   GPIO_AF14_LTDC
/*´¹Ö±Í¬²½ÐÅºÅVSYNC*/
#define LTDC_VSYNC_GPIO_PORT            GPIOI
#define LTDC_VSYNC_GPIO_CLK_ENABLE()    __GPIOI_CLK_ENABLE()
#define LTDC_VSYNC_GPIO_PIN             GPIO_PIN_9
#define LTDC_VSYNC_AF                   GPIO_AF14_LTDC

/*Êý¾ÝÊ¹ÄÜÐÅºÅDE*/
#define LTDC_DE_GPIO_PORT               GPIOF
#define LTDC_DE_GPIO_CLK_ENABLE()       __GPIOF_CLK_ENABLE()
#define LTDC_DE_GPIO_PIN                GPIO_PIN_10
#define LTDC_DE_AF                      GPIO_AF14_LTDC
/*Òº¾§ÆÁÊ¹ÄÜÐÅºÅDISP£¬¸ßµçÆ½Ê¹ÄÜ*/
#define LTDC_DISP_GPIO_PORT             GPIOD
#define LTDC_DISP_GPIO_CLK_ENABLE()     __GPIOD_CLK_ENABLE()
#define LTDC_DISP_GPIO_PIN              GPIO_PIN_4
/*Òº¾§ÆÁ±³¹âÐÅºÅ£¬¸ßµçÆ½Ê¹ÄÜ*/
#define LTDC_BL_GPIO_PORT               GPIOD
#define LTDC_BL_GPIO_CLK_ENABLE()       __GPIOD_CLK_ENABLE()
#define LTDC_BL_GPIO_PIN                GPIO_PIN_7

uint32_t LCD_GetXSize(void);
uint32_t LCD_GetYSize(void);



#endif
