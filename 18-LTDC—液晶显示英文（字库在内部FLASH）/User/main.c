/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2017-xx-xx
  * @brief   GPIO���--ʹ�ù̼������LED��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� STM32 F429 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://fire-stm32.taobao.com
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx.h"
#include "./usart/bsp_debug_usart.h" 
#include "./sdram/bsp_sdram.h"
#include <stdlib.h>
#include "./led/bsp_led.h" 
#include "./lcd/bsp_lcd.h"
#include "./touch/bsp_touch_gtxx.h"

void Delay(__IO uint32_t nCount); 

void LCD_Test(void);
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
    /* ϵͳʱ�ӳ�ʼ����216 MHz */
    SystemClock_Config();
    /* LED �˿ڳ�ʼ�� */
    LED_GPIO_Config();	 
    /* ���Դ��ڳ�ʼ�� */
    DEBUG_USART_Config();
    /* �����ȳ�ʼ��������, ��ȡ����оƬID���жϲ�ͬ�ߴ����͵���Ļ */
    GTP_Init_ReadID();
    /* LCD �˿ڳ�ʼ�� */ 
    LCD_Init();
    /* LCD ��һ���ʼ�� */ 
    LCD_LayerInit(0, LCD_FB_START_ADDRESS,ARGB8888);
	/* LCD �ڶ����ʼ�� */ 
    LCD_LayerInit(1, LCD_FB_START_ADDRESS+(LCD_GetXSize()*LCD_GetYSize()*4),ARGB8888);
    /* ʹ��LCD������������ */ 
    LCD_DisplayOn(); 

    /* ѡ��LCD��һ�� */
    LCD_SelectLayer(0);

    /* ��һ����������ʾȫ�� */ 
    LCD_Clear(LCD_COLOR_BLACK);  

    /* ѡ��LCD�ڶ��� */
    LCD_SelectLayer(1);

    /* �ڶ�����������ʾȫ�� */ 
    LCD_Clear(LCD_COLOR_TRANSPARENT);

    /* ���õ�һ�͵ڶ����͸����,��СֵΪ0�����ֵΪ255*/
    LCD_SetTransparency(0, 255);
    LCD_SetTransparency(1, 0);
	
    while(1)
    {
      LCD_Test(); 
    }
}

/*���ڲ��Ը���Һ���ĺ���*/
void LCD_Test(void)
{
	/*��ʾ��ʾ����*/
	static uint8_t testCNT = 0;	
	char dispBuff[100];
	
    /* ѡ��LCD��һ�� */
    LCD_SelectLayer(0);
	
	/* ��������ʾȫ�� */
	LCD_Clear(LCD_COLOR_BLACK);	
	/*����������ɫ������ı�����ɫ(�˴��ı�������ָLCD�ı����㣡ע������)*/
	LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
	/*ѡ������*/
	LCD_SetFont(&LCD_DEFAULT_FONT);

	LCD_DisplayStringLine(1,(uint8_t* )"BH 5.0 inch LCD para:");
	LCD_DisplayStringLine(2,(uint8_t* )"Image resolution:800x480 px");
	LCD_DisplayStringLine(3,(uint8_t* )"Touch pad:5 point touch supported");
	LCD_DisplayStringLine(4,(uint8_t* )"Use STM32-LTDC directed driver,");
	LCD_DisplayStringLine(5,(uint8_t* )"no need extern driver,RGB888,24bits data bus");
	LCD_DisplayStringLine(6,(uint8_t* )"Touch pad use IIC to communicate");
	//while(1)
	{		
	testCNT++;
	/*ʹ��c��׼��ѱ���ת�����ַ���*/
	sprintf(dispBuff,"Display value demo: testCount = %d ",testCNT);
	LCD_ClearLine(7);
	/*����������ɫ������ı�����ɫ(�˴��ı�������ָLCD�ı����㣡ע������)*/
	LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
	/*Ȼ����ʾ���ַ������ɣ���������Ҳ����������*/
	LCD_DisplayStringLine(7,(uint8_t* )dispBuff);


	/* ��ֱ�� */
	LCD_SetTextColor(LCD_COLOR_BLUE);

	LCD_ClearLine(8);
	LCD_DisplayStringLine(8,(uint8_t* )"Draw line:");

	LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);
	LCD_DrawLine(50,250,750,250);  
	LCD_DrawLine(50,300,750,300);

	LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
	LCD_DrawLine(300,250,400,400);  
	LCD_DrawLine(600,250,600,400);

	Delay(0xFFFFFF);

	LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
	LCD_FillRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);


	/*������*/
	LCD_SetTextColor(LCD_COLOR_BLUE);
	LCD_ClearLine(8);
	LCD_DisplayStringLine(8,(uint8_t* )"Draw Rect:");

	LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);
	LCD_DrawRect(200,250,200,100);

	LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
	LCD_DrawRect(350,250,200,50);

	LCD_SetColors(LCD_COLOR_BLUE,LCD_COLOR_BLACK);
	LCD_DrawRect(200,350,50,200);

	Delay(0xFFFFFF);


	LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
	LCD_FillRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);


	/*������*/
	LCD_SetTextColor(LCD_COLOR_BLUE);
	LCD_ClearLine(8);
	LCD_DisplayStringLine(8,(uint8_t* )"Draw Full Rect:");

	LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);
	LCD_FillRect(200,250,200,100);

	LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
	LCD_FillRect(350,250,200,50);

	LCD_SetColors(LCD_COLOR_BLUE,LCD_COLOR_BLACK);
	LCD_FillRect(200,350,50,200);

	Delay(0xFFFFFF);

	LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
	LCD_FillRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);

	/* ��Բ */
	LCD_SetTextColor(LCD_COLOR_BLUE);
	LCD_ClearLine(8);
	LCD_DisplayStringLine(8,(uint8_t* )"Draw circle:");

	LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_RED);
	LCD_DrawCircle(200,350,50);

	LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_GREEN);
	LCD_DrawCircle(350,350,75);

	Delay(0xFFFFFF);

	LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
	LCD_FillRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);


	/*���Բ*/
	LCD_SetTextColor(LCD_COLOR_BLUE);
	LCD_ClearLine(8);
	LCD_DisplayStringLine(8,(uint8_t* )"Draw full circle:");

	LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);
	LCD_FillCircle(300,350,50);

	LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
	LCD_FillCircle(450,350,75);

	Delay(0xFFFFFF);

	LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
	LCD_FillRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);

	LCD_ClearLine(8);
	
	/*͸��Ч�� ǰ�������*/
	LCD_SetTextColor(LCD_COLOR_BLUE);
	LCD_ClearLine(8);
	LCD_DisplayStringLine(8,(uint8_t* )"Transparency effect:");
	
	/*����ǰ���㲻͸����*/
	LCD_SetTransparency(1, 128);

    /* ѡ��LCD��һ�� */
    LCD_SelectLayer(1);
	
	/* ��������ʾȫ�� */
	LCD_Clear(LCD_COLOR_BLACK);	
	/*��ǰ����һ����ɫԲ*/
	LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_TRANSPARENT);
	LCD_FillCircle(400,350,75);
	
	Delay(0xFFFFFF);
	
	/*͸��Ч�� ���������*/

	/* ѡ��LCD������ */
	LCD_SelectLayer(0);	
	LCD_Clear(LCD_COLOR_BLACK);		
	/*���ñ����㲻͸��*/
	LCD_SetTransparency(1, 0);
	

	/*�ڱ�����һ����ɫԲ*/
	LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
	LCD_FillCircle(450,350,75);
	
	/*�ڱ�����һ����ɫԲ*/
	LCD_SetColors(LCD_COLOR_BLUE,LCD_COLOR_BLACK);
	LCD_FillCircle(350,350,75);
	
	Delay(0xFFFFFF);
	LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
	LCD_FillRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);
	}
}


/**
  * @brief  ϵͳʱ������ 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 180000000
  *            HCLK(Hz)                       = 180000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 360
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  ��
  * @retval ��
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* ʹ��HSE������HSEΪPLL��ʱ��Դ������PLL�ĸ��ַ�Ƶ����M N P Q 
	 * PLLCLK = HSE/M*N/P = 25M / 25 *432 / 2 = 216M
	 */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
   while(1) {}
  }

  /* ���� OverDrive ģʽ */
  HAL_PWREx_EnableOverDrive();
 
  /* ѡ��PLLCLK��ΪSYSCLK�������� HCLK, PCLK1 and PCLK2 ��ʱ�ӷ�Ƶ���� 
	 * SYSCLK = PLLCLK     = 180M
	 * HCLK   = SYSCLK / 1 = 180M
	 * PCLK2  = SYSCLK / 2 = 90M
	 * PCLK1  = SYSCLK / 4 = 45M
	 */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    while(1) {}
  }
}

void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
