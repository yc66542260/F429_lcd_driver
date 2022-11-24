/**
  ******************************************************************************
  * @file    bsp_touch_gtxx.c
  * @brief   ���ļ�����������ȡGTxxx����оƬ��ID��,�������ֲ�ͬ��RGB��Ļ
  ******************************************************************************
  */ 
#include <stdio.h>
#include <string.h>
#include "./touch/bsp_touch_gtxx.h"
#include "./touch/bsp_i2c_touch.h"
#include "./lcd/bsp_lcd.h"


/* ����IC����Ĭ��Ϊ5������ic */
TOUCH_IC touchIC = GT9157;		

const TOUCH_PARAM_TypeDef touch_param[TOUCH_TYPE_NUM] = 
{
  /* GT9157,5���� */
  {
    .max_width = 800,
    .max_height = 480,
    .config_reg_addr = 0x8047,
  },
  /* GT911,7���� */
  {
    .max_width = 800,
    .max_height = 480,
    .config_reg_addr = 0x8047,
  },
  /* GT5688,4.3���� */
  {
    .max_width = 480,
    .max_height = 272,
    .config_reg_addr = 0x8050,
  },
	 /* GT917S,5���� */
  {
    .max_width = 800,
    .max_height = 480,
    .config_reg_addr = 0x8050,
  },
  /* GT615,7���� */
  {
    .max_width = 800,
    .max_height = 480,
    .config_reg_addr = 0x8047, //����ʼ��ַ��GT911��һ��
  },
};				

static int8_t GTP_I2C_Test(void);


/**
  * @brief   ʹ��IIC�������ݴ���
  * @param
  *		@arg i2c_msg:���ݴ���ṹ��
  *		@arg num:���ݴ���ṹ��ĸ���
  * @retval  ������ɵĴ���ṹ��������������������0xff
  */
static int I2C_Transfer( struct i2c_msg *msgs,int num)
{
	int im = 0;
	int ret = 0;

	GTP_DEBUG_FUNC();

	for (im = 0; ret == 0 && im != num; im++)
	{
		if ((msgs[im].flags&I2C_M_RD))																//����flag�ж��Ƕ����ݻ���д����
		{
			ret = I2C_ReadBytes(msgs[im].addr, msgs[im].buf, msgs[im].len);		//IIC��ȡ����
		} else
		{
			ret = I2C_WriteBytes(msgs[im].addr,  msgs[im].buf, msgs[im].len);	//IICд������
		}
	}
	if(ret)
		return ret;

	return im;   													//������ɵĴ���ṹ����
}

/**
  * @brief   ��IIC�豸�ж�ȡ����
  * @param
  *		@arg client_addr:�豸��ַ
  *		@arg  buf[0~1]: ��ȡ���ݼĴ�������ʼ��ַ
  *		@arg buf[2~len-1]: �洢���������ݵĻ���buffer
  *		@arg len:    GTP_ADDR_LENGTH + read bytes count���Ĵ�����ַ����+��ȡ�������ֽ�����
  * @retval  i2c_msgs����ṹ��ĸ�����2Ϊ�ɹ�������Ϊʧ��
  */
static int32_t GTP_I2C_Read(uint8_t client_addr, uint8_t *buf, int32_t len)
{
    struct i2c_msg msgs[2];
    int32_t ret=-1;
    int32_t retries = 0;

    GTP_DEBUG_FUNC();
    /*һ�������ݵĹ��̿��Է�Ϊ�����������:
     * 1. IIC  д�� Ҫ��ȡ�ļĴ�����ַ
     * 2. IIC  ��ȡ  ����
     * */

    msgs[0].flags = !I2C_M_RD;					//д��
    msgs[0].addr  = client_addr;					//IIC�豸��ַ
    msgs[0].len   = GTP_ADDR_LENGTH;	//�Ĵ�����ַΪ2�ֽ�(��д�����ֽڵ�����)
    msgs[0].buf   = &buf[0];						//buf[0~1]�洢����Ҫ��ȡ�ļĴ�����ַ
    
    msgs[1].flags = I2C_M_RD;					//��ȡ
    msgs[1].addr  = client_addr;					//IIC�豸��ַ
    msgs[1].len   = len - GTP_ADDR_LENGTH;	//Ҫ��ȡ�����ݳ���
    msgs[1].buf   = &buf[GTP_ADDR_LENGTH];	//buf[GTP_ADDR_LENGTH]֮��Ļ������洢����������

    while(retries < 5)
    {
        ret = I2C_Transfer( msgs, 2);					//����IIC���ݴ�����̺�������2���������
        if(ret == 2)break;
        retries++;
    }
    if((retries >= 5))
    {
        GTP_ERROR("I2C Read: 0x%04X, %d bytes failed, errcode: %d! Process reset.", (((uint16_t)(buf[0] << 8)) | buf[1]), len-2, ret);
    }
    return ret;
}



/**
  * @brief   ��IIC�豸д������
  * @param
  *		@arg client_addr:�豸��ַ
  *		@arg  buf[0~1]: Ҫд������ݼĴ�������ʼ��ַ
  *		@arg buf[2~len-1]: Ҫд�������
  *		@arg len:    GTP_ADDR_LENGTH + write bytes count���Ĵ�����ַ����+д��������ֽ�����
  * @retval  i2c_msgs����ṹ��ĸ�����1Ϊ�ɹ�������Ϊʧ��
  */
static int32_t GTP_I2C_Write(uint8_t client_addr,uint8_t *buf,int32_t len)
{
    struct i2c_msg msg;
    int32_t ret = -1;
    int32_t retries = 0;

    GTP_DEBUG_FUNC();
    /*һ��д���ݵĹ���ֻ��Ҫһ���������:
     * 1. IIC���� д�� ���ݼĴ�����ַ������
     * */
    msg.flags = !I2C_M_RD;			//д��
    msg.addr  = client_addr;			//���豸��ַ
    msg.len   = len;							//����ֱ�ӵ���(�Ĵ�����ַ����+д��������ֽ���)
    msg.buf   = buf;						//ֱ������д�뻺�����е�����(�����˼Ĵ�����ַ)

    while(retries < 5)
    {
        ret = I2C_Transfer(&msg, 1);	//����IIC���ݴ�����̺�����1���������
        if (ret == 1)break;
        retries++;
    }
    if((retries >= 5))
    {

        GTP_ERROR("I2C Write: 0x%04X, %d bytes failed, errcode: %d! Process reset.", (((uint16_t)(buf[0] << 8)) | buf[1]), len-2, ret);

    }
    return ret;
}





static int32_t GTP_Get_Info(void)
{
    uint8_t opr_buf[6] = {0};
    int32_t ret = 0;

    uint16_t abs_x_max = GTP_MAX_WIDTH;
    uint16_t abs_y_max = GTP_MAX_HEIGHT;
    uint8_t int_trigger_type = GTP_INT_TRIGGER;
        
    opr_buf[0] = (uint8_t)((GTP_REG_CONFIG_DATA+1) >> 8);
    opr_buf[1] = (uint8_t)((GTP_REG_CONFIG_DATA+1) & 0xFF);
    
    ret = GTP_I2C_Read(GTP_ADDRESS, opr_buf, 6);
    if (ret < 0)
    {
        return FAIL;
    }
    
    abs_x_max = (opr_buf[3] << 8) + opr_buf[2];
    abs_y_max = (opr_buf[5] << 8) + opr_buf[4];
    
    opr_buf[0] = (uint8_t)((GTP_REG_CONFIG_DATA+6) >> 8);
    opr_buf[1] = (uint8_t)((GTP_REG_CONFIG_DATA+6) & 0xFF);
    
    ret = GTP_I2C_Read(GTP_ADDRESS, opr_buf, 3);
    if (ret < 0)
    {
        return FAIL;
    }
    int_trigger_type = opr_buf[2] & 0x03;
    
    GTP_INFO("X_MAX = %d, Y_MAX = %d, TRIGGER = 0x%02x",
            abs_x_max,abs_y_max,int_trigger_type);
    
    return SUCCESS;    
}


/* ��ʼ������оƬ,Ȼ���ȡ����оƬID���жϲ�ͬ�ߴ����͵���Ļ
   ���浱ǰ��Һ�������͵����� cur_lcd */
  
int32_t GTP_Init_ReadID(void)
{
    int32_t ret = -1;

    GTP_DEBUG_FUNC();


    I2C_Touch_Init();

    ret = GTP_I2C_Test();
    if (ret < 0)
    {
        GTP_ERROR("I2C communication ERROR!");
				return ret;
    } 
		
		//��ȡ����IC���ͺ�
    GTP_Read_Version(); 
    
    GTP_Get_Info();

    return 0;
}


/*******************************************************
Function:
    Read chip version.
Input:
    client:  i2c device
    version: buffer to keep ic firmware version
Output:
    read operation return.
        2: succeed, otherwise: failed
*******************************************************/
int32_t GTP_Read_Version(void)
{
    int32_t ret = -1;
    uint8_t buf[8] = {GTP_REG_VERSION >> 8, GTP_REG_VERSION & 0xff};    //�Ĵ�����ַ

    GTP_DEBUG_FUNC();

    ret = GTP_I2C_Read(GTP_ADDRESS, buf, sizeof(buf));
    if (ret < 0)
    {
        GTP_ERROR("GTP read version failed");
        return ret;
    }

    if (buf[4] == '1')
    {				
				//GT911оƬ
				if(buf[2] == '9' && buf[3] == '1' && buf[4] == '1')
        {
          GTP_INFO("IC1 Version: %c%c%c_%02x%02x", buf[2], buf[3], buf[4], buf[7], buf[6]);

					touchIC = GT911;
          /* ���õ�ǰ��Һ�������� */
          cur_lcd = INCH_7;
        }
        //GT9157оƬ
        else
           GTP_INFO("Unknown IC Version: %c%c%c%c_%02x%02x", buf[2], buf[3], buf[4], buf[5], buf[7], buf[6]);
		}
		else if (buf[4] == '5')
		{
			if( buf[2] == '9' && buf[3] == '1' && buf[4] == '5' && buf[5] == '7')
        {
          GTP_INFO("IC2 Version: %c%c%c%c_%02x%02x", buf[2], buf[3], buf[4], buf[5], buf[7], buf[6]);

					touchIC = GT9157;
          /* ���õ�ǰ��Һ�������� */
          cur_lcd = INCH_5;
        }
        //GT615оƬ
        else if( buf[2] == '6' && buf[3] == '1' && buf[4] == '5')
        {
          GTP_INFO("IC3 Version: %c%c%c%c_%02x%02x", buf[2], buf[3], buf[4], buf[5], buf[7], buf[6]);

					touchIC = GT615;
          /* ���õ�ǰ��Һ�������� */
          cur_lcd = INCH_7;
        }
        else
           GTP_INFO("Unknown IC Version: %c%c%c%c_%02x%02x", buf[2], buf[3], buf[4], buf[5], buf[7], buf[6]);

    }    
    else if (buf[4] == '8')
    {	
				//GT5688оƬ
				if(buf[2] == '5' && buf[3] == '6' && buf[4] == '8' && buf[5] == '8')
        {
          GTP_INFO("IC4 Version: %c%c%c%c_%02x%02x", buf[2], buf[3], buf[4], buf[5], buf[7], buf[6]);

					touchIC = GT5688;
          /* ���õ�ǰ��Һ�������� */
          cur_lcd = INCH_4_3;
        }
        else
           GTP_INFO("Unknown IC Version: %c%c%c%c_%02x%02x", buf[2], buf[3], buf[4], buf[5], buf[7], buf[6]);

    }
		else if(buf[4] == '7')
    {
			  //GT917SоƬ
         GTP_INFO("IC5 Version: %c%c%c%c_%02x%02x", buf[2], buf[3], buf[4], buf[5], buf[7], buf[6]);
				
				if(buf[2] == '9' && buf[3] == '1' && buf[4] == '7' && buf[5] == 'S')
				{	
					touchIC = GT917S; 
					/* ���õ�ǰ��Һ�������� */
          cur_lcd = INCH_5;	
				}					
    }
    else 
       GTP_INFO("Unknown IC Version: %c%c%c%c_%02x%02x", buf[2], buf[3], buf[4], buf[5], buf[7], buf[6]);

    return ret;
}

/*******************************************************
Function:
    I2c test Function.
Input:
    client:i2c client.
Output:
    Executive outcomes.
        2: succeed, otherwise failed.
*******************************************************/
static int8_t GTP_I2C_Test( void)
{
    uint8_t test[3] = {GTP_REG_CONFIG_DATA >> 8, GTP_REG_CONFIG_DATA & 0xff};
    uint8_t retry = 0;
    int8_t ret = -1;

    GTP_DEBUG_FUNC();
  
    while(retry++ < 5)
    {
        ret = GTP_I2C_Read(GTP_ADDRESS, test, 3);
        if (ret > 0)
        {
            return ret;
        }
        GTP_ERROR("GTP i2c test failed time %d.",retry);
    }
    return ret;
}

