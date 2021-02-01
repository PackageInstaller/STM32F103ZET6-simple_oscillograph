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
	RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK | KEY2_GPIO_CLK, ENABLE);

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
static const uint8_t MulClickState[] = 
{
	KEY_1ClICK, KEY_2ClICK, KEY_3ClICK,
};

static KEY_TypeDef Key[KEY_NUMS] =
	{{KEY_OFF, KEY_OFF, 0, 0, 0, KEY_1ClICK},
	 {KEY_OFF, KEY_OFF, 0, 0, 0, KEY_1ClICK}};

uint8_t Key_Scan(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	KEY_TypeDef *KeyTemp;

	//��鰴�µ�����һ����ť
	switch ((uint32_t)GPIOx)
	{
	case ((uint32_t)KEY1_GPIO_PORT):
		switch (GPIO_Pin)
		{
		case KEY1_GPIO_PIN:
			KeyTemp = &Key[0];
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
			KeyTemp = &Key[1];
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

	KeyTemp->KeyPhysic = GPIO_ReadInputDataBit(GPIOx, GPIO_Pin);



	/* ��ⰴ�¡��ɿ������� */
	switch (KeyTemp->KeyLogic)
	{
	case KEY_ON:
		switch (KeyTemp->KeyPhysic)
		{
		//��1��1���н��رռ������㣬���Կ��������ۼ�ֱ���л����߼�����״̬
		case KEY_ON:
			KeyTemp->KeyOFFCounts = 0;
			KeyTemp->KeyONCounts++;
			if (KeyTemp->KeyONCounts >= HOLD_COUNTS)
			{
				KeyTemp->KeyONCounts = 0;
				KeyTemp->KeyLogic = KEY_HOLD;
				return KEY_HOLD;
			}


			return KEY_IDLE;

		//��1��0���жԹرռ����ۼ�ֱ���л����߼��ر�״̬
		case KEY_OFF:
			KeyTemp->KeyOFFCounts++;
			
			if (KeyTemp->KeyOFFCounts >= SHAKES_COUNTS)
			{
				KeyTemp->KeyLogic = KEY_OFF;
				KeyTemp->KeyOFFCounts = 0;
				
				return KEY_OFF;
			}
			return KEY_IDLE;

		default:
			break;
		}

	case KEY_OFF:
		switch (KeyTemp->KeyPhysic)
		{
		//��0��1���жԿ��������ۼ�ֱ���л����߼�����״̬
		case KEY_ON:
			(KeyTemp->KeyONCounts)++;
			if (KeyTemp->KeyONCounts >= SHAKES_COUNTS)
			{
				//KeyTemp->KeyLogic = KEY_ON;
				KeyTemp->KeyLogic = KEY_ON;			
				KeyTemp->KeyONCounts = 0;

				if(KeyTemp->MulClickFlag == KEY_1ClICK)
				{
					KeyTemp->MulClickFlag = KEY_2ClICK;  	//Ԥ��˫��״̬
					return KEY_ON;
				}
				else
				{
					if(KeyTemp->MulClickFlag != KEY_MAX_MULCLICK)
					{
						KeyTemp->MulClickFlag++;
						KeyTemp->MulClickCounts = 0;
						return (KeyTemp->MulClickFlag - 1);
					}
					else
					{
						KeyTemp->MulClickFlag = KEY_1ClICK;
						return KEY_MAX_MULCLICK;
					}	
				}				
			}
			return KEY_IDLE;

		//��0��0���н������������㣬�Զ������
		case KEY_OFF:
			(KeyTemp->KeyONCounts) = 0;
			if(KeyTemp->MulClickFlag != KEY_1ClICK)
			{
				if(KeyTemp->MulClickCounts++ > MULTIPLE_CLICK_COUNTS) 	//������������ʱ�䣬�رն��״̬
				{
					KeyTemp->MulClickCounts = 0;
					KeyTemp->MulClickFlag = KEY_1ClICK;				
				}
			}
			
			return KEY_IDLE;
		default:
			break;
		}

	case KEY_HOLD:
		switch (KeyTemp->KeyPhysic)
		{
		//��2��1���Թرռ�������
		case KEY_ON:
			KeyTemp->KeyOFFCounts = 0;
			return KEY_HOLD;
		//��2��0���Թرռ����ۼ�ֱ���л����߼��ر�״̬
		case KEY_OFF:
			(KeyTemp->KeyOFFCounts)++;
			if (KeyTemp->KeyOFFCounts >= SHAKES_COUNTS)
			{
				KeyTemp->KeyLogic = KEY_OFF;
				KeyTemp->KeyOFFCounts = 0;
				return KEY_OFF;
			}
			return KEY_IDLE;

		default:
			break;
		}

	
	default:
		break;
	}
	return KEY_ERROR;
}
/*********************************************END OF FILE**********************/
