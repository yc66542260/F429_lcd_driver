/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-06     SummerGift   first version
 * 2018-11-19     flybreak     add stm32f429-fire-challenger bsp
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

/* defined the LED0 pin: PH10 */
#define LED0_PIN    GET_PIN(D, 12)

int main(void)
{
    int count = 1;
    /* set LED0 pin mode to output */
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);

    LCD_Init();

    LCD_DisplayOn();

    LCD_LayerInit(0, (uint32_t)0xD0000000,LTDC_PIXEL_FORMAT_ARGB8888);
    /* LCD µÚ¶þ²ã³õÊ¼»¯ */
    LCD_LayerInit(1, (uint32_t)0xD0000000+(LCD_GetXSize()*LCD_GetYSize()*4),LTDC_PIXEL_FORMAT_ARGB8888);
    /* Ê¹ÄÜLCD£¬°üÀ¨¿ª±³¹â */
    LCD_DisplayOn();

    LCD_SelectLayer(0);

    /* µÚÒ»²ãÇåÆÁ£¬ÏÔÊ¾È«ºÚ */
    //LCD_Clear((uint32_t)0xFF000000);
    LCD_Clear((uint32_t)0xFFFFA500);
    /* Ñ¡ÔñLCDµÚ¶þ²ã */
    LCD_SelectLayer(1);
    LCD_Clear((uint32_t)0xFFFFA500);
    /* µÚ¶þ²ãÇåÆÁ£¬ÏÔÊ¾È«ºÚ */
    //LCD_Clear(0xFF000000);

    /* ÅäÖÃµÚÒ»ºÍµÚ¶þ²ãµÄÍ¸Ã÷¶È,×îÐ¡ÖµÎª0£¬×î´óÖµÎª255*/
    LCD_SetTransparency(0, 255);
    LCD_SetTransparency(1, 0);

    LCD_Clear((uint32_t)0xFFFFA500);

    while (count++)
    {
        rt_pin_write(LED0_PIN, PIN_HIGH);
        rt_thread_mdelay(1000);
        LCD_SelectLayer(0);

        /* µÚÒ»²ãÇåÆÁ£¬ÏÔÊ¾È«ºÚ */
        //LCD_Clear((uint32_t)0xFFFFFFFF);
        LCD_Clear((uint32_t)0xFFFF0000);
        LCD_SelectLayer(1);

        /* µÚÒ»²ãÇåÆÁ£¬ÏÔÊ¾È«ºÚ */
        //LCD_Clear((uint32_t)0xFF000000);
        LCD_Clear((uint32_t)0xFFFFFFFF);
        rt_pin_write(LED0_PIN, PIN_LOW);
        rt_thread_mdelay(1000);
        LCD_SelectLayer(0);

        /* µÚÒ»²ãÇåÆÁ£¬ÏÔÊ¾È«ºÚ */
        //LCD_Clear((uint32_t)0xFFFFFFFF);
        LCD_Clear((uint32_t)0xFFFFA500);
        LCD_SelectLayer(0);

        /* µÚÒ»²ãÇåÆÁ£¬ÏÔÊ¾È«ºÚ */
        //LCD_Clear((uint32_t)0xFF000000);
        LCD_Clear((uint32_t)0xFFFFFFFF);

        rt_thread_mdelay(1000);
        LCD_SelectLayer(0);

        /* µÚÒ»²ãÇåÆÁ£¬ÏÔÊ¾È«ºÚ */
        //LCD_Clear((uint32_t)0xFFFFFFFF);
        LCD_Clear((uint32_t)0xFFFFFF00);
        LCD_SelectLayer(0);

        rt_thread_mdelay(1000);
        LCD_SelectLayer(0);

        /* µÚÒ»²ãÇåÆÁ£¬ÏÔÊ¾È«ºÚ */
        //LCD_Clear((uint32_t)0xFFFFFFFF);
        LCD_Clear((uint32_t)0xFF0000FF);
        LCD_SelectLayer(0);
    }

    return RT_EOK;
}
