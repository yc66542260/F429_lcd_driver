/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-01-08     zylx         first version
 */

#ifndef __LCD_PORT_H__
#define __LCD_PORT_H__

/* fire 5 inch screen, 800 * 480 */
#define LCD_WIDTH           800
#define LCD_HEIGHT          480
#define LCD_BITS_PER_PIXEL  16
#define LCD_BUF_SIZE        (LCD_WIDTH * LCD_HEIGHT * LCD_BITS_PER_PIXEL / 8)
#define LCD_PIXEL_FORMAT    RTGRAPHIC_PIXEL_FORMAT_RGB565

#define LCD_HSYNC_WIDTH     1
#define LCD_VSYNC_HEIGHT    1
#define LCD_HBP             46
#define LCD_VBP             23
#define LCD_HFP             22
#define LCD_VFP             22

#define LCD_BACKLIGHT_USING_GPIO
#define LCD_BL_GPIO_NUM     GET_PIN(D, 7)
#define LCD_DISP_GPIO_NUM   GET_PIN(D, 4)
/* fire 5 inch screen, 800 * 480 */

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


#endif /* __LCD_PORT_H__ */

