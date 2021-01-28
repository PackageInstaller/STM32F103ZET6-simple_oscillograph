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

static uint8_t Key1Flag = 0;
static uint8_t Key2Flag = 0;

uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{	
	uint8_t* KeyFlag;

	switch ((uint32_t)GPIOx)
	{
	case ((uint32_t)KEY1_GPIO_PORT):
		switch (GPIO_Pin)
		{
		case KEY1_GPIO_PIN:
			KeyFlag = &Key1Flag;
			break;
		
		default:
			printf("error: GPIO port pin not match\r\n");
			return KEY_OFF;
			
		}
		break;

	case ((uint32_t)KEY2_GPIO_PORT):
		switch (GPIO_Pin)
		{
		case KEY2_GPIO_PIN:
			KeyFlag = &Key2Flag;
			break;
		
		default:
			printf("error: GPIO port pin not match\r\n");
			return KEY_OFF;
			
		}
		break;

	default:
		printf("error: key do not exist\r\n");
		return KEY_OFF;
	}

	/*����Ƿ��а������� */
	if( !(*KeyFlag) && (GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON ))  
	{
		*KeyFlag = 1;
		return 	KEY_ON;	 		
	}
	else if( (*KeyFlag) && (GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_OFF))
	{
		*KeyFlag = 0;
		return 	KEY_OFF;
	}
	else
	{
		return KEY_OFF;
	}
		
}
/*********************************************END OF FILE**********************/
