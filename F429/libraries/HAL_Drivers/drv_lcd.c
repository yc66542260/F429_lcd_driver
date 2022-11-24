/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-01-08     zylx         first version
 */

#include <board.h>

#ifdef BSP_USING_LCD
#include <lcd_port.h>
#include <string.h>

//#define DRV_DEBUG
#define LOG_TAG             "drv.lcd"
#include <drv_log.h>
static LTDC_HandleTypeDef  Ltdc_Handler;
static DMA2D_HandleTypeDef Dma2d_Handler;
static uint32_t            ActiveLayer = 0;

#define LCD_DEVICE(dev)     (struct drv_lcd_device*)(dev)

static LTDC_HandleTypeDef LtdcHandle = {0};

struct drv_lcd_device
{
    struct rt_device parent;

    struct rt_device_graphic_info lcd_info;

    struct rt_semaphore lcd_lock;

    /* 0:front_buf is being used 1: back_buf is being used*/
    rt_uint8_t cur_buf;
    rt_uint8_t *front_buf;
    rt_uint8_t *back_buf;
};

struct drv_lcd_device _lcd;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////

static void LCD_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

  /* Ê¹ÄÜLCDÊ¹ÓÃµ½µÄÒý½ÅÊ±ÖÓ */
                          //ºìÉ«Êý¾ÝÏß
  LTDC_R0_GPIO_CLK_ENABLE();LTDC_R1_GPIO_CLK_ENABLE();LTDC_R2_GPIO_CLK_ENABLE();\
  LTDC_R3_GPIO_CLK_ENABLE();LTDC_R4_GPIO_CLK_ENABLE();LTDC_R5_GPIO_CLK_ENABLE();\
  LTDC_R6_GPIO_CLK_ENABLE();LTDC_R7_GPIO_CLK_ENABLE();LTDC_G0_GPIO_CLK_ENABLE();\
  LTDC_G1_GPIO_CLK_ENABLE();LTDC_G2_GPIO_CLK_ENABLE();LTDC_G3_GPIO_CLK_ENABLE();\
  LTDC_G3_GPIO_CLK_ENABLE();LTDC_G5_GPIO_CLK_ENABLE();LTDC_G6_GPIO_CLK_ENABLE();\
  LTDC_G7_GPIO_CLK_ENABLE();LTDC_B0_GPIO_CLK_ENABLE();LTDC_B1_GPIO_CLK_ENABLE();\
  LTDC_B2_GPIO_CLK_ENABLE();LTDC_B3_GPIO_CLK_ENABLE();LTDC_B4_GPIO_CLK_ENABLE();\
  LTDC_B5_GPIO_CLK_ENABLE();LTDC_B6_GPIO_CLK_ENABLE();LTDC_B7_GPIO_CLK_ENABLE();\
  LTDC_CLK_GPIO_CLK_ENABLE();LTDC_HSYNC_GPIO_CLK_ENABLE();LTDC_VSYNC_GPIO_CLK_ENABLE();\
  LTDC_DE_GPIO_CLK_ENABLE();LTDC_DISP_GPIO_CLK_ENABLE();LTDC_BL_GPIO_CLK_ENABLE();
/* GPIOÅäÖÃ */

 /* ºìÉ«Êý¾ÝÏß */
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;

  GPIO_InitStruct.Pin =   LTDC_R0_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_R0_AF;
  HAL_GPIO_Init(LTDC_R0_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin =   LTDC_R1_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_R1_AF;
  HAL_GPIO_Init(LTDC_R1_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin =   LTDC_R2_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_R2_AF;
  HAL_GPIO_Init(LTDC_R2_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin =   LTDC_R3_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_R3_AF;
  HAL_GPIO_Init(LTDC_R3_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin =   LTDC_R4_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_R4_AF;
  HAL_GPIO_Init(LTDC_R4_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin =   LTDC_R5_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_R5_AF;
  HAL_GPIO_Init(LTDC_R5_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin =   LTDC_R6_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_R6_AF;
  HAL_GPIO_Init(LTDC_R6_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin =   LTDC_R7_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_R7_AF;
  HAL_GPIO_Init(LTDC_R7_GPIO_PORT, &GPIO_InitStruct);

  //ÂÌÉ«Êý¾ÝÏß
  GPIO_InitStruct.Pin =   LTDC_G0_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_G0_AF;
  HAL_GPIO_Init(LTDC_G0_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin =   LTDC_G1_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_G1_AF;
  HAL_GPIO_Init(LTDC_G1_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin =   LTDC_G2_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_G2_AF;
  HAL_GPIO_Init(LTDC_G2_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin =   LTDC_G3_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_G3_AF;
  HAL_GPIO_Init(LTDC_G3_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin =   LTDC_G4_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_G4_AF;
  HAL_GPIO_Init(LTDC_G4_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin =   LTDC_G5_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_G5_AF;
  HAL_GPIO_Init(LTDC_G5_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin =   LTDC_G6_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_G6_AF;
  HAL_GPIO_Init(LTDC_G6_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin =   LTDC_G7_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_G7_AF;
  HAL_GPIO_Init(LTDC_G7_GPIO_PORT, &GPIO_InitStruct);

  //À¶É«Êý¾ÝÏß
  GPIO_InitStruct.Pin =   LTDC_B0_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_B0_AF;
  HAL_GPIO_Init(LTDC_B0_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin =   LTDC_B1_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_B1_AF;
  HAL_GPIO_Init(LTDC_B1_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin =   LTDC_B2_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_B2_AF;
  HAL_GPIO_Init(LTDC_B2_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin =   LTDC_B3_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_B3_AF;
  HAL_GPIO_Init(LTDC_B3_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin =   LTDC_B4_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_B4_AF;
  HAL_GPIO_Init(LTDC_B4_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin =   LTDC_B5_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_B5_AF;
  HAL_GPIO_Init(LTDC_B5_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin =   LTDC_B6_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_B6_AF;
  HAL_GPIO_Init(LTDC_B6_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin =   LTDC_B7_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_B7_AF;
  HAL_GPIO_Init(LTDC_B7_GPIO_PORT, &GPIO_InitStruct);

  //¿ØÖÆÐÅºÅÏß
  GPIO_InitStruct.Pin = LTDC_CLK_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_CLK_AF;
  HAL_GPIO_Init(LTDC_CLK_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LTDC_HSYNC_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_HSYNC_AF;
  HAL_GPIO_Init(LTDC_HSYNC_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LTDC_VSYNC_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_VSYNC_AF;
  HAL_GPIO_Init(LTDC_VSYNC_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LTDC_DE_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_DE_AF;
  HAL_GPIO_Init(LTDC_DE_GPIO_PORT, &GPIO_InitStruct);

  //±³¹âBL ¼°Òº¾§Ê¹ÄÜÐÅºÅDISP
  GPIO_InitStruct.Pin = LTDC_DISP_GPIO_PIN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;

  HAL_GPIO_Init(LTDC_DISP_GPIO_PORT, &GPIO_InitStruct);


  GPIO_InitStruct.Pin = LTDC_BL_GPIO_PIN;
  HAL_GPIO_Init(LTDC_BL_GPIO_PORT, &GPIO_InitStruct);

}

void LCD_Init(void)
{
    RCC_PeriphCLKInitTypeDef  periph_clk_init_struct;
    /* Ê¹ÄÜLTDCÊ±ÖÓ */
    __HAL_RCC_LTDC_CLK_ENABLE();
    /* Ê¹ÄÜDMA2DÊ±ÖÓ */
    __HAL_RCC_DMA2D_CLK_ENABLE();
    /* ³õÊ¼»¯LCDÒý½Å */
    LCD_GPIO_Config();
    /* ³õÊ¼»¯SDRAM ÓÃ×÷LCD ÏÔ´æ*/
    /* ÅäÖÃLTDC²ÎÊý */
    Ltdc_Handler.Instance = LTDC;
    /* ÅäÖÃÐÐÍ¬²½ÐÅºÅ¿í¶È(HSW-1) */
    Ltdc_Handler.Init.HorizontalSync =LCD_HSYNC_WIDTH;
    /* ÅäÖÃ´¹Ö±Í¬²½ÐÅºÅ¿í¶È(VSW-1) */
    Ltdc_Handler.Init.VerticalSync = LCD_VSYNC_HEIGHT;
    /* ÅäÖÃ(HSW+HBP-1) */
    Ltdc_Handler.Init.AccumulatedHBP = LCD_HSYNC_WIDTH+LCD_HBP-1;
    /* ÅäÖÃ(VSW+VBP-1) */
    Ltdc_Handler.Init.AccumulatedVBP = LCD_VSYNC_HEIGHT+LCD_VBP-1;
    /* ÅäÖÃ(HSW+HBP+ÓÐÐ§ÏñËØ¿í¶È-1) */
    Ltdc_Handler.Init.AccumulatedActiveW = LCD_HSYNC_WIDTH+LCD_HBP+LCD_WIDTH-1;
    /* ÅäÖÃ(VSW+VBP+ÓÐÐ§ÏñËØ¸ß¶È-1) */
    Ltdc_Handler.Init.AccumulatedActiveH = LCD_VSYNC_HEIGHT+LCD_VBP+LCD_HEIGHT-1;
    /* ÅäÖÃ×Ü¿í¶È(HSW+HBP+ÓÐÐ§ÏñËØ¿í¶È+HFP-1) */
    Ltdc_Handler.Init.TotalWidth =LCD_HSYNC_WIDTH+LCD_HBP+LCD_WIDTH + LCD_HFP-1;
    /* ÅäÖÃ×Ü¸ß¶È(VSW+VBP+ÓÐÐ§ÏñËØ¸ß¶È+VFP-1) */
    Ltdc_Handler.Init.TotalHeigh =LCD_VSYNC_HEIGHT+LCD_VBP+LCD_HEIGHT + LCD_VFP-1;
    /* Òº¾§ÆÁÊ±ÖÓÅäÖÃ */
    /* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
    /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN = 192 Mhz */
    /* PLLLCDCLK = PLLSAI_VCO Output/PLLSAIR = 192/5 = 38.4 Mhz */
    /* LTDC clock frequency = PLLLCDCLK / LTDC_PLLSAI_DIVR_4 = 38.4/4 = 9.6Mhz */
    periph_clk_init_struct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
    periph_clk_init_struct.PLLSAI.PLLSAIN = 192;
    periph_clk_init_struct.PLLSAI.PLLSAIR = 5;
    periph_clk_init_struct.PLLSAIDivR = RCC_PLLSAIDIVR_4;
    HAL_RCCEx_PeriphCLKConfig(&periph_clk_init_struct);
    /* ³õÊ¼»¯LCDµÄÏñËØ¿í¶ÈºÍ¸ß¶È */
    Ltdc_Handler.LayerCfg->ImageWidth  = LCD_WIDTH;
    Ltdc_Handler.LayerCfg->ImageHeight = LCD_HEIGHT;
    /* ÉèÖÃLCD±³¾°²ãµÄÑÕÉ«£¬Ä¬ÈÏºÚÉ« */
    Ltdc_Handler.Init.Backcolor.Red = 0;
    Ltdc_Handler.Init.Backcolor.Green = 0;
    Ltdc_Handler.Init.Backcolor.Blue = 0;
    /* ¼«ÐÔÅäÖÃ */
    /* ³õÊ¼»¯ÐÐÍ¬²½¼«ÐÔ£¬µÍµçÆ½ÓÐÐ§ */
    Ltdc_Handler.Init.HSPolarity = LTDC_HSPOLARITY_AL;
    /* ³õÊ¼»¯³¡Í¬²½¼«ÐÔ£¬µÍµçÆ½ÓÐÐ§ */
    Ltdc_Handler.Init.VSPolarity = LTDC_VSPOLARITY_AL;
    /* ³õÊ¼»¯Êý¾ÝÓÐÐ§¼«ÐÔ£¬µÍµçÆ½ÓÐÐ§ */
    Ltdc_Handler.Init.DEPolarity = LTDC_DEPOLARITY_AL;
    /* ³õÊ¼»¯ÐÐÏñËØÊ±ÖÓ¼«ÐÔ£¬Í¬ÊäÈëÊ±ÖÓ */
    Ltdc_Handler.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
    HAL_LTDC_Init(&Ltdc_Handler);
    /* ³õÊ¼»¯×ÖÌå */
    //LCD_SetFont(&LCD_DEFAULT_FONT);
}

uint32_t LCD_GetXSize(void)
{
  return Ltdc_Handler.LayerCfg[ActiveLayer].ImageWidth;
}

/**
  * @brief  »ñÈ¡LCDµ±Ç°ÓÐÐ§²ãYÖáµÄ´óÐ¡
  * @retval YÖáµÄ´óÐ¡
  */
uint32_t LCD_GetYSize(void)
{
  return Ltdc_Handler.LayerCfg[ActiveLayer].ImageHeight;
}

/**
  * @brief  ÉèÖÃLCDµ±Ç°ÓÐÐ§²ãYÖáµÄ´óÐ¡
  * @param  imageWidthPixels£ºÍ¼Ïñ¿í¶ÈÏñËØ¸öÊý
  * @retval ÎÞ
  */
void LCD_SetXSize(uint32_t imageWidthPixels)
{
  Ltdc_Handler.LayerCfg[ActiveLayer].ImageWidth = imageWidthPixels;
}

/**
  * @brief  ÉèÖÃLCDµ±Ç°ÓÐÐ§²ãYÖáµÄ´óÐ¡
  * @param  imageHeightPixels£ºÍ¼Ïñ¸ß¶ÈÏñËØ¸öÊý
  * @retval None
  */
void LCD_SetYSize(uint32_t imageHeightPixels)
{
  Ltdc_Handler.LayerCfg[ActiveLayer].ImageHeight = imageHeightPixels;
}


void LCD_LayerInit(uint16_t LayerIndex, uint32_t FB_Address,uint32_t PixelFormat)
{
  LTDC_LayerCfgTypeDef  layer_cfg;

  /* ²ã³õÊ¼»¯ */
  layer_cfg.WindowX0 = 0;               //´°¿ÚÆðÊ¼Î»ÖÃX×ø±ê
  layer_cfg.WindowX1 = LCD_GetXSize();  //´°¿Ú½áÊøÎ»ÖÃX×ø±ê
  layer_cfg.WindowY0 = 0;               //´°¿ÚÆðÊ¼Î»ÖÃY×ø±ê
  layer_cfg.WindowY1 = LCD_GetYSize();  //´°¿Ú½áÊøÎ»ÖÃY×ø±ê
  layer_cfg.PixelFormat = PixelFormat;  //ÏñËØ¸ñÊ½
  layer_cfg.FBStartAdress = FB_Address; //²ãÏÔ´æÊ×µØÖ·
  layer_cfg.Alpha = 255;                //ÓÃÓÚ»ìºÏµÄÍ¸Ã÷¶È³£Á¿£¬·¶Î§£¨0¡ª255£©0ÎªÍêÈ«Í¸Ã÷
  layer_cfg.Alpha0 = 0;                 //Ä¬ÈÏÍ¸Ã÷¶È³£Á¿£¬·¶Î§£¨0¡ª255£©0ÎªÍêÈ«Í¸Ã÷
  layer_cfg.Backcolor.Blue = 0;         //²ã±³¾°ÑÕÉ«À¶É«·ÖÁ¿
  layer_cfg.Backcolor.Green = 0;        //²ã±³¾°ÑÕÉ«ÂÌÉ«·ÖÁ¿
  layer_cfg.Backcolor.Red = 0;          //²ã±³¾°ÑÕÉ«ºìÉ«·ÖÁ¿
  layer_cfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;//²ã»ìºÏÏµÊý1
  layer_cfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;//²ã»ìºÏÏµÊý2
  layer_cfg.ImageWidth = LCD_GetXSize();//ÉèÖÃÍ¼Ïñ¿í¶È
  layer_cfg.ImageHeight = LCD_GetYSize();//ÉèÖÃÍ¼Ïñ¸ß¶È

  HAL_LTDC_ConfigLayer(&Ltdc_Handler, &layer_cfg, LayerIndex); //ÉèÖÃÑ¡ÖÐµÄ²ã²ÎÊý

  //DrawProp[LayerIndex].BackColor = ((uint32_t)0xFFFFFFFF;//ÉèÖÃ²ãµÄ×ÖÌåÑÕÉ«
  //DrawProp[LayerIndex].pFont     = &LCD_DEFAULT_FONT;//ÉèÖÃ²ãµÄ×ÖÌåÀàÐÍ
  //DrawProp[LayerIndex].TextColor = ((uint32_t)0xFF000000); //ÉèÖÃ²ãµÄ×ÖÌå±³¾°ÑÕÉ«

  __HAL_LTDC_RELOAD_CONFIG(&Ltdc_Handler);//ÖØÔØ²ãµÄÅäÖÃ²ÎÊý
}

void LCD_DisplayOn(void)
{
  /* ¿ªÏÔÊ¾ */
  __HAL_LTDC_ENABLE(&Ltdc_Handler);
  HAL_GPIO_WritePin(LTDC_DISP_GPIO_PORT, LTDC_DISP_GPIO_PIN, GPIO_PIN_SET);/* LCD_DISPÊ¹ÄÜ*/
  HAL_GPIO_WritePin(LTDC_BL_GPIO_PORT, LTDC_BL_GPIO_PIN, GPIO_PIN_SET);  /* ¿ª±³¹â*/
}

/**
  * @brief  ½ûÄÜÏÔÊ¾
  * @retval ÎÞ
  */
void LCD_DisplayOff(void)
{
  /* ¹ØÏÔÊ¾ */
  __HAL_LTDC_DISABLE(&Ltdc_Handler);
  HAL_GPIO_WritePin(LTDC_DISP_GPIO_PORT, LTDC_DISP_GPIO_PIN, GPIO_PIN_RESET); /* LCD_DISP½ûÄÜ*/
  HAL_GPIO_WritePin(LTDC_BL_GPIO_PORT, LTDC_BL_GPIO_PIN, GPIO_PIN_RESET);/*¹Ø±³¹â*/
}

void LCD_ClockConfig(LTDC_HandleTypeDef *hltdc, void *Params)
{
  static RCC_PeriphCLKInitTypeDef  periph_clk_init_struct;

  /* LCDÊ±ÖÓÅäÖÃ */
  /* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
  /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN = 192 Mhz */
  /* PLLLCDCLK = PLLSAI_VCO Output/PLLSAIR = 192/5 = 38.4 Mhz */
  /* LTDCÊ±ÖÓÆµÂÊ = PLLLCDCLK / LTDC_PLLSAI_DIVR_4 = 38.4/4 = 9.6Mhz */
  periph_clk_init_struct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  periph_clk_init_struct.PLLSAI.PLLSAIN = 400;
  periph_clk_init_struct.PLLSAI.PLLSAIR = 4;
  periph_clk_init_struct.PLLSAIDivR = RCC_PLLSAIDIVR_2;
  HAL_RCCEx_PeriphCLKConfig(&periph_clk_init_struct);
}

//void LCD_Clear(uint32_t Color)
//{
//  /* ÇåÆÁ */
//  LL_FillBuffer(ActiveLayer, (uint32_t *)(Ltdc_Handler.LayerCfg[ActiveLayer].FBStartAdress), LCD_GetXSize(), LCD_GetYSize(), 0, Color);
//}
void LCD_SelectLayer(uint32_t LayerIndex)
{
  ActiveLayer = LayerIndex;
}


void LCD_SetTransparency(uint32_t LayerIndex, uint8_t Transparency)
{
  HAL_LTDC_SetAlpha(&Ltdc_Handler, Transparency, LayerIndex);
}

static void LL_FillBuffer(uint32_t LayerIndex, void *pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLine, uint32_t ColorIndex)
{

  Dma2d_Handler.Init.Mode         = DMA2D_R2M;
  if(Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB565)
  {
    Dma2d_Handler.Init.ColorMode    = DMA2D_RGB565;
  }
  else if(Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB8888)
  {
    Dma2d_Handler.Init.ColorMode    = DMA2D_ARGB8888;
  }
  else if(Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB888)
  {
    Dma2d_Handler.Init.ColorMode    = DMA2D_RGB888;
  }
  else if(Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB1555)
  {
    Dma2d_Handler.Init.ColorMode    = DMA2D_ARGB1555;
  }
  else if(Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB4444)
  {
    Dma2d_Handler.Init.ColorMode    = DMA2D_ARGB4444;
  }
  Dma2d_Handler.Init.OutputOffset = OffLine;

  Dma2d_Handler.Instance = DMA2D;

  /* DMA2D ³õÊ¼»¯ */
  if(HAL_DMA2D_Init(&Dma2d_Handler) == HAL_OK)
  {
    if(HAL_DMA2D_ConfigLayer(&Dma2d_Handler, LayerIndex) == HAL_OK)
    {
      if (HAL_DMA2D_Start(&Dma2d_Handler, ColorIndex, (uint32_t)pDst, xSize, ySize) == HAL_OK)
      {
        /* DMAÂÖÑ¯´«Êä */
        HAL_DMA2D_PollForTransfer(&Dma2d_Handler, 100);
      }
    }
  }
}
void LCD_Clear(uint32_t Color)
{
  /* ÇåÆÁ */
  LL_FillBuffer(ActiveLayer, (uint32_t *)(Ltdc_Handler.LayerCfg[ActiveLayer].FBStartAdress), LCD_GetXSize(), LCD_GetYSize(), 0, Color);
}



/// //////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////





static rt_err_t drv_lcd_init(struct rt_device *device)
{
    struct drv_lcd_device *lcd = LCD_DEVICE(device);
    /* nothing, right now */
    lcd = lcd;
    return RT_EOK;
}

static rt_err_t drv_lcd_control(struct rt_device *device, int cmd, void *args)
{
    struct drv_lcd_device *lcd = LCD_DEVICE(device);

    switch (cmd)
    {
    case RTGRAPHIC_CTRL_RECT_UPDATE:
    {
        /* update */
        if (_lcd.cur_buf)
        {
            /* back_buf is being used */
            memcpy(_lcd.front_buf, _lcd.lcd_info.framebuffer, LCD_BUF_SIZE);
            /* Configure the color frame buffer start address */
            LTDC_LAYER(&LtdcHandle, 0)->CFBAR &= ~(LTDC_LxCFBAR_CFBADD);
            LTDC_LAYER(&LtdcHandle, 0)->CFBAR = (uint32_t)(_lcd.front_buf);
            _lcd.cur_buf = 0;
        }
        else
        {
            /* front_buf is being used */
            memcpy(_lcd.back_buf, _lcd.lcd_info.framebuffer, LCD_BUF_SIZE);
            /* Configure the color frame buffer start address */
            LTDC_LAYER(&LtdcHandle, 0)->CFBAR &= ~(LTDC_LxCFBAR_CFBADD);
            LTDC_LAYER(&LtdcHandle, 0)->CFBAR = (uint32_t)(_lcd.back_buf);
            _lcd.cur_buf = 1;
        }
        rt_sem_take(&_lcd.lcd_lock, RT_TICK_PER_SECOND / 20);
        HAL_LTDC_Relaod(&LtdcHandle, LTDC_SRCR_VBR);
    }
    break;

    case RTGRAPHIC_CTRL_GET_INFO:
    {
        struct rt_device_graphic_info *info = (struct rt_device_graphic_info *)args;

        RT_ASSERT(info != RT_NULL);
        info->pixel_format  = lcd->lcd_info.pixel_format;
        info->bits_per_pixel = 16;
        info->width         = lcd->lcd_info.width;
        info->height        = lcd->lcd_info.height;
        info->framebuffer   = lcd->lcd_info.framebuffer;
    }
    break;
    }

    return RT_EOK;
}

void HAL_LTDC_ReloadEventCallback(LTDC_HandleTypeDef *hltdc)
{
    /* emable line interupt */
    __HAL_LTDC_ENABLE_IT(&LtdcHandle, LTDC_IER_LIE);
}

void HAL_LTDC_LineEventCallback(LTDC_HandleTypeDef *hltdc)
{
    rt_sem_release(&_lcd.lcd_lock);
}

void LTDC_IRQHandler(void)
{
    rt_enter_critical();

    HAL_LTDC_IRQHandler(&LtdcHandle);

    rt_exit_critical();
}

rt_err_t stm32_lcd_init(struct drv_lcd_device *lcd)
{
    LTDC_LayerCfgTypeDef pLayerCfg = {0};

    /* LTDC Initialization -------------------------------------------------------*/

    /* Polarity configuration */
    /* Initialize the horizontal synchronization polarity as active low */
    LtdcHandle.Init.HSPolarity = LTDC_HSPOLARITY_AL;
    /* Initialize the vertical synchronization polarity as active low */
    LtdcHandle.Init.VSPolarity = LTDC_VSPOLARITY_AL;
    /* Initialize the data enable polarity as active low */
    LtdcHandle.Init.DEPolarity = LTDC_DEPOLARITY_AL;
    /* Initialize the pixel clock polarity as input pixel clock */
    LtdcHandle.Init.PCPolarity = LTDC_PCPOLARITY_IPC;

    /* Timing configuration */
    /* Horizontal synchronization width = Hsync - 1 */
    LtdcHandle.Init.HorizontalSync = LCD_HSYNC_WIDTH - 1;
    /* Vertical synchronization height = Vsync - 1 */
    LtdcHandle.Init.VerticalSync = LCD_VSYNC_HEIGHT - 1;
    /* Accumulated horizontal back porch = Hsync + HBP - 1 */
    LtdcHandle.Init.AccumulatedHBP = LCD_HSYNC_WIDTH + LCD_HBP - 1;
    /* Accumulated vertical back porch = Vsync + VBP - 1 */
    LtdcHandle.Init.AccumulatedVBP = LCD_VSYNC_HEIGHT + LCD_VBP - 1;
    /* Accumulated active width = Hsync + HBP + Active Width - 1 */
    LtdcHandle.Init.AccumulatedActiveW = LCD_HSYNC_WIDTH + LCD_HBP + lcd->lcd_info.width - 1 ;
    /* Accumulated active height = Vsync + VBP + Active Heigh - 1 */
    LtdcHandle.Init.AccumulatedActiveH = LCD_VSYNC_HEIGHT + LCD_VBP + lcd->lcd_info.height - 1;
    /* Total height = Vsync + VBP + Active Heigh + VFP - 1 */
    LtdcHandle.Init.TotalHeigh = LtdcHandle.Init.AccumulatedActiveH + LCD_VFP;
    /* Total width = Hsync + HBP + Active Width + HFP - 1 */
    LtdcHandle.Init.TotalWidth = LtdcHandle.Init.AccumulatedActiveW + LCD_HFP;

    /* Configure R,G,B component values for LCD background color */
    LtdcHandle.Init.Backcolor.Blue = 0;
    LtdcHandle.Init.Backcolor.Green = 0;
    LtdcHandle.Init.Backcolor.Red = 0;

    LtdcHandle.Instance = LTDC;

    /* Layer1 Configuration ------------------------------------------------------*/

    /* Windowing configuration */
    pLayerCfg.WindowX0 = 0;
    pLayerCfg.WindowX1 = lcd->lcd_info.width;
    pLayerCfg.WindowY0 = 0;
    pLayerCfg.WindowY1 = lcd->lcd_info.height;

    /* Pixel Format configuration*/
    if (lcd->lcd_info.pixel_format == RTGRAPHIC_PIXEL_FORMAT_RGB565)
    {
        pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
    }
    else if (lcd->lcd_info.pixel_format == RTGRAPHIC_PIXEL_FORMAT_ARGB888)
    {
        pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
    }
    else if (lcd->lcd_info.pixel_format == RTGRAPHIC_PIXEL_FORMAT_RGB888)
    {
        pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB888;
    }
    else if (lcd->lcd_info.pixel_format == RTGRAPHIC_PIXEL_FORMAT_RGB888)
    {
        pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB888;
    }
    else
    {
        LOG_E("unsupported pixel format");
        return -RT_ERROR;
    }

    /* Start Address configuration : frame buffer is located at FLASH memory */
    pLayerCfg.FBStartAdress = (uint32_t)lcd->front_buf;

    /* Alpha constant (255 totally opaque) */
    pLayerCfg.Alpha = 255;

    /* Default Color configuration (configure A,R,G,B component values) */
    pLayerCfg.Alpha0 = 255;
    pLayerCfg.Backcolor.Blue = 0;
    pLayerCfg.Backcolor.Green = 0;
    pLayerCfg.Backcolor.Red = 0;

    /* Configure blending factors */
    /* Constant Alpha value:  pLayerCfg.Alpha / 255
       C: Current Layer Color
       Cs: Background color
       BC = Constant Alpha x C + (1 - Constant Alpha ) x Cs */
    /* BlendingFactor1: Pixel Alpha x Constant Alpha */
    pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
    /* BlendingFactor2: 1 - (Pixel Alpha x Constant Alpha) */
    pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;

    /* Configure the number of lines and number of pixels per line */
    pLayerCfg.ImageWidth = lcd->lcd_info.width;
    pLayerCfg.ImageHeight = lcd->lcd_info.height;

    /* Configure the LTDC */
    if (HAL_LTDC_Init(&LtdcHandle) != HAL_OK)
    {
        LOG_E("LTDC init failed");
        return -RT_ERROR;
    }

    /* Configure the Background Layer*/
    if (HAL_LTDC_ConfigLayer(&LtdcHandle, &pLayerCfg, 0) != HAL_OK)
    {
        LOG_E("LTDC layer init failed");
        return -RT_ERROR;
    }
    else
    {
        /* enable LTDC interrupt */
        HAL_NVIC_SetPriority(LTDC_IRQn, 1, 0);
        HAL_NVIC_EnableIRQ(LTDC_IRQn);
        LOG_D("LTDC init success");
        return RT_EOK;
    }
}
#if defined(LCD_BACKLIGHT_USING_PWM)
void turn_on_lcd_backlight(void)
{
    struct rt_device_pwm *pwm_dev;

    /* turn on the LCD backlight */
    pwm_dev = (struct rt_device_pwm *)rt_device_find(LCD_PWM_DEV_NAME);
    /* pwm frequency:100K = 10000ns */
    rt_pwm_set(pwm_dev, LCD_PWM_DEV_CHANNEL, 10000, 10000);
    rt_pwm_enable(pwm_dev, LCD_PWM_DEV_CHANNEL);
}
#elif defined(LCD_BACKLIGHT_USING_GPIO)
void turn_on_lcd_backlight(void)
{
    rt_pin_mode(LCD_BL_GPIO_NUM, PIN_MODE_OUTPUT);
    rt_pin_mode(LCD_DISP_GPIO_NUM, PIN_MODE_OUTPUT);

    rt_pin_write(LCD_DISP_GPIO_NUM, PIN_HIGH);
    rt_pin_write(LCD_BL_GPIO_NUM, PIN_HIGH);
}
#else
void turn_on_lcd_backlight(void)
{
    
}
#endif

#ifdef RT_USING_DEVICE_OPS
const static struct rt_device_ops lcd_ops =
{
    drv_lcd_init,
    RT_NULL,
    RT_NULL,
    RT_NULL,
    RT_NULL,
    drv_lcd_control
};
#endif



//struct rt_device
//{
//    struct rt_object          parent;                   /**< inherit from rt_object */
//
//    enum rt_device_class_type type;                     /**< device type */
//    rt_uint16_t               flag;                     /**< device flag */
//    rt_uint16_t               open_flag;                /**< device open flag */
//
//    rt_uint8_t                ref_count;                /**< reference count */
//    rt_uint8_t                device_id;                /**< 0 - 255 */
//
//    /* device call back */
//    rt_err_t (*rx_indicate)(rt_device_t dev, rt_size_t size);
//    rt_err_t (*tx_complete)(rt_device_t dev, void *buffer);
//
//#ifdef RT_USING_DEVICE_OPS
//    const struct rt_device_ops *ops;
//#else
//    /* common device interface */
//    rt_err_t  (*init)   (rt_device_t dev);
//    rt_err_t  (*open)   (rt_device_t dev, rt_uint16_t oflag);
//    rt_err_t  (*close)  (rt_device_t dev);
//    rt_size_t (*read)   (rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size);
//    rt_size_t (*write)  (rt_device_t dev, rt_off_t pos, const void *buffer, rt_size_t size);
//    rt_err_t  (*control)(rt_device_t dev, int cmd, void *args);
//#endif
//
//#ifdef RT_USING_POSIX_DEVIO
//    const struct dfs_file_ops *fops;
//    struct rt_wqueue wait_queue;
//#endif
//
//    void                     *user_data;                /**< device private data */
//};



int drv_lcd_hw_init(void)
{
    rt_err_t result = RT_EOK;
    struct rt_device *device = &_lcd.parent;

    /* memset _lcd to zero */
    memset(&_lcd, 0x00, sizeof(_lcd));

    /* init lcd_lock semaphore */
    result = rt_sem_init(&_lcd.lcd_lock, "lcd_lock", 0, RT_IPC_FLAG_FIFO);
    if (result != RT_EOK)
    {
        LOG_E("init semaphore failed!\n");
        result = -RT_ENOMEM;
        goto __exit;
    }

    /* config LCD dev info */
    _lcd.lcd_info.height = LCD_HEIGHT;
    _lcd.lcd_info.width = LCD_WIDTH;
    _lcd.lcd_info.bits_per_pixel = LCD_BITS_PER_PIXEL;
    _lcd.lcd_info.pixel_format = LCD_PIXEL_FORMAT;

    /* malloc memory for Triple Buffering */
    _lcd.lcd_info.framebuffer = rt_malloc(LCD_BUF_SIZE);
    _lcd.back_buf = rt_malloc(LCD_BUF_SIZE);
    _lcd.front_buf = rt_malloc(LCD_BUF_SIZE);
    if (_lcd.lcd_info.framebuffer == RT_NULL || _lcd.back_buf == RT_NULL || _lcd.front_buf == RT_NULL)
    {
        LOG_E("init frame buffer failed!\n");
        result = -RT_ENOMEM;
        goto __exit;
    }

    /* memset buff to 0xFF */
    memset(_lcd.lcd_info.framebuffer, 0xFF, LCD_BUF_SIZE);
    memset(_lcd.back_buf, 0xFF, LCD_BUF_SIZE);
    memset(_lcd.front_buf, 0xFF, LCD_BUF_SIZE);

    device->type    = RT_Device_Class_Graphic;
#ifdef RT_USING_DEVICE_OPS
    device->ops     = &lcd_ops;
#else
    device->init    = drv_lcd_init;
    device->control = drv_lcd_control;
    device->open    = NULL;
    device->close   = NULL;
    device->read    = NULL;
    device->write   = NULL;
//    rt_err_t  (*init)   (rt_device_t dev);
//    rt_err_t  (*open)   (rt_device_t dev, rt_uint16_t oflag);
//    rt_err_t  (*close)  (rt_device_t dev);
//    rt_size_t (*read)   (rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size);
//    rt_size_t (*write)  (rt_device_t dev, rt_off_t pos, const void *buffer, rt_size_t size);
//    rt_err_t  (*control)(rt_device_t dev, int cmd, void *args);
#endif

    /* register lcd device */
    rt_device_register(device, "lcd", RT_DEVICE_FLAG_RDWR);

    /* init stm32 LTDC */
    if (stm32_lcd_init(&_lcd) != RT_EOK)
    {
        result = -RT_ERROR;
        goto __exit;
    }
    else
    {
        turn_on_lcd_backlight();
    }

__exit:
    if (result != RT_EOK)
    {
        rt_sem_delete(&_lcd.lcd_lock);

        if (_lcd.lcd_info.framebuffer)
        {
            rt_free(_lcd.lcd_info.framebuffer);
        }

        if (_lcd.back_buf)
        {
            rt_free(_lcd.back_buf);
        }

        if (_lcd.front_buf)
        {
            rt_free(_lcd.front_buf);
        }
    }
    return result;
}
INIT_DEVICE_EXPORT(drv_lcd_hw_init);

#ifdef DRV_DEBUG
#ifdef FINSH_USING_MSH
int lcd_test()
{
    struct drv_lcd_device *lcd;
    lcd = (struct drv_lcd_device *)rt_device_find("lcd");

    while (1)
    {
        /* red */
        for (int i = 0; i < LCD_BUF_SIZE / 2; i++)
        {
            lcd->lcd_info.framebuffer[2 * i] = 0x00;
            lcd->lcd_info.framebuffer[2 * i + 1] = 0xF8;
        }
        lcd->parent.control(&lcd->parent, RTGRAPHIC_CTRL_RECT_UPDATE, RT_NULL);
        rt_thread_mdelay(1000);
        /* green */
        for (int i = 0; i < LCD_BUF_SIZE / 2; i++)
        {
            lcd->lcd_info.framebuffer[2 * i] = 0xE0;
            lcd->lcd_info.framebuffer[2 * i + 1] = 0x07;
        }
        lcd->parent.control(&lcd->parent, RTGRAPHIC_CTRL_RECT_UPDATE, RT_NULL);
        rt_thread_mdelay(1000);
        /* blue */
        for (int i = 0; i < LCD_BUF_SIZE / 2; i++)
        {
            lcd->lcd_info.framebuffer[2 * i] = 0x1F;
            lcd->lcd_info.framebuffer[2 * i + 1] = 0x00;
        }
        lcd->parent.control(&lcd->parent, RTGRAPHIC_CTRL_RECT_UPDATE, RT_NULL);
        rt_thread_mdelay(1000);
    }
}
MSH_CMD_EXPORT(lcd_test, lcd_test);
#endif /* FINSH_USING_MSH */
#endif /* DRV_DEBUG */
#endif /* BSP_USING_LCD */
