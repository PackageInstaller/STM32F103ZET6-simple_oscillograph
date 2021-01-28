/**
  ******************************************************************************
  * @file    bsp_key.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ����Ӧ��bsp��ɨ��ģʽ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:���� F103-�Ե� STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "bsp_key.h"  
#include "bsp_usart.h"

/**
  * @brief  ���ð����õ���I/O��
  * @param  ��
  * @retval ��
  */
void Key_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure = {0};
	
	/*���������˿ڵ�ʱ��*/
	RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK|KEY2_GPIO_CLK,ENABLE);
	
	//ѡ�񰴼�������
	GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN; 
	// ���ð���������Ϊ��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	//ʹ�ýṹ���ʼ������
	GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);
	
	//ѡ�񰴼�������
	GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN; 
	//���ð���������Ϊ��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	//ʹ�ýṹ���ʼ������
	GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);	
}

 /*
 * ��������Key_Scan
 * ����  ������Ƿ��а�������
 * ����  ��GPIOx��x ������ A��B��C��D���� E
 *		     GPIO_Pin������ȡ�Ķ˿�λ 	
 * ���  ��KEY_OFF(û���°���)��KEY_ON�����°�����
 */


//static KEY_TypeDef Key[2] = {{0,0,0},{0,0,0}};

static uint8_t Key1Logic = 0;
static uint8_t Key1ONCounts = 0;
static uint8_t Key1OFFCounts = 0;

static uint8_t Key2Logic = 0;
static uint8_t Key2ONCounts = 0;
static uint8_t Key2OFFCounts = 0;

uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{	
	uint8_t* KeyLogic;
	uint8_t* KeyONCounts;
	uint8_t* KeyOFFCounts;

	//��鰴�µ�����һ����ť
	switch ((uint32_t)GPIOx)
	{
	case ((uint32_t)KEY1_GPIO_PORT):
		switch (GPIO_Pin)
		{
		case KEY1_GPIO_PIN:
			KeyLogic = &Key1Logic;
			KeyONCounts = &Key1ONCounts;
			KeyOFFCounts = &Key1OFFCounts;
			break;
		
		//port��pin��ƥ��
		default:
			printf("error: GPIO port pin not match\r\n");
			return KEY_IDLE;
			
		}
		break;

	case ((uint32_t)KEY2_GPIO_PORT):
		switch (GPIO_Pin)
		{
		case KEY2_GPIO_PIN:
			KeyLogic = &Key2Logic;
			KeyONCounts = &Key2ONCounts;
			KeyOFFCounts = &Key2OFFCounts;
			break;

		//port��pin��ƥ��
		default:
			printf("error: GPIO port pin not match\r\n");
			return KEY_IDLE;
			
		}
		break;

	default:
		printf("error: key do not exist\r\n");
		return KEY_IDLE;
	}


	/* ��ⰴ�¡��ɿ������� */
	switch (*KeyLogic)
	{
	case KEY_ON:
		switch (GPIO_ReadInputDataBit(GPIOx,GPIO_Pin))
		{
		case KEY_ON:
			(*KeyOFFCounts) = 0;
			(*KeyONCounts)++;
			if(*KeyONCounts >= HOLD_COUNTS)
			{
				*KeyONCounts = 0;
				*KeyLogic = KEY_HOLD;
				return KEY_HOLD;
			}
			return KEY_IDLE;

		case KEY_OFF:
			(*KeyOFFCounts)++;
			if(*KeyOFFCounts >= SHAKES_COUNTS)
			{
				*KeyLogic = KEY_OFF;
				*KeyONCounts = 0;
				*KeyOFFCounts = 0;
				return 	KEY_OFF;	
			}
			return KEY_IDLE;

		default:
			break;
		}



	case KEY_OFF:
		switch (GPIO_ReadInputDataBit(GPIOx,GPIO_Pin))
		{
		case KEY_ON:
			(*KeyONCounts)++;
			if(*KeyONCounts >= SHAKES_COUNTS)
			{
				*KeyLogic = KEY_ON;
				*KeyONCounts = 0;
				*KeyOFFCounts = 0;
				return 	KEY_ON;	
			}
			return KEY_IDLE; 

		case KEY_OFF:
			(*KeyONCounts) = 0;
			return KEY_IDLE; 
		default:
			break;
		}



	case KEY_HOLD:
		switch (GPIO_ReadInputDataBit(GPIOx,GPIO_Pin))
		{
		case KEY_ON:
			return KEY_HOLD;

		case KEY_OFF:
			(*KeyOFFCounts)++;
			if(*KeyOFFCounts >= SHAKES_COUNTS)
			{
				*KeyLogic = KEY_OFF;
				*KeyONCounts = 0;
				*KeyOFFCounts = 0;
				return 	KEY_OFF;	
			}
			return KEY_IDLE;

		default:
			break;
		}


	default:
		break;
	}

	return 10;
		
}
/*********************************************END OF FILE**********************/
