#ifndef      __BSP_ILI9341_LCD_H
#define	     __BSP_ILI9341_LCD_H


#include "stm32f10x.h"


/***************************************************************************************
2^26 =0X0400 0000 = 64MB,ÿ�� BANK ��4*64MB = 256MB
64MB:FSMC_Bank1_NORSRAM1:0X6000 0000 ~ 0X63FF FFFF
64MB:FSMC_Bank1_NORSRAM2:0X6400 0000 ~ 0X67FF FFFF
64MB:FSMC_Bank1_NORSRAM3:0X6800 0000 ~ 0X6BFF FFFF
64MB:FSMC_Bank1_NORSRAM4:0X6C00 0000 ~ 0X6FFF FFFF

ѡ��BANK1-BORSRAM4 ���� TFT����ַ��ΧΪ0X6C00 0000 ~ 0X6FFF FFFF
FSMC_A23 ��LCD��DC(�Ĵ���/����ѡ��)��
�Ĵ�������ַ = 0X6C00 0000
RAM����ַ = 0X6D00 0000 = 0X6C00 0000+2^23*2 = 0X6C00 0000 + 0X100 0000 = 0X6D00 0000
��ѡ��ͬ�ĵ�ַ��ʱ����ַҪ���¼���  
****************************************************************************************/

/******************************* ILI9341 ��ʾ���� FSMC �������� ***************************/
//FSMC_Bank1_NORSRAM����LCD��������ĵ�ַ
#define      FSMC_Addr_ILI9341_CMD         ( ( uint32_t ) 0x60000000 )

//FSMC_Bank1_NORSRAM����LCD���ݲ����ĵ�ַ      
#define      FSMC_Addr_ILI9341_DATA        ( ( uint32_t ) 0x60020000 )

//��Ƭѡ���ž�����NOR/SRAM��
#define      FSMC_Bank1_NORSRAMx           FSMC_Bank1_NORSRAM1

//����emWinʹ��
#define 		macFSMC_ILI9341_REG								*(__IO uint16_t *)(0x60000000)
#define 		macFSMC_ILI9341_RAM								*(__IO uint16_t *)(0x60020000)

/******************************* ILI9341 ��ʾ��8080ͨѶ���Ŷ��� ***************************/
/******�����ź���******/
//Ƭѡ��ѡ��NOR/SRAM��
#define      ILI9341_CS_CLK                RCC_APB2Periph_GPIOD   
#define      ILI9341_CS_PORT               GPIOD
#define      ILI9341_CS_PIN                GPIO_Pin_7

//DC���ţ�ʹ��FSMC�ĵ�ַ�źſ��ƣ������ž����˷���LCDʱʹ�õĵ�ַ
//PD11ΪFSMC_A16
#define      ILI9341_DC_CLK                RCC_APB2Periph_GPIOD   
#define      ILI9341_DC_PORT               GPIOD
#define      ILI9341_DC_PIN                GPIO_Pin_11

//дʹ��
#define      ILI9341_WR_CLK                RCC_APB2Periph_GPIOD   
#define      ILI9341_WR_PORT               GPIOD
#define      ILI9341_WR_PIN                GPIO_Pin_5

//��ʹ��
#define      ILI9341_RD_CLK                RCC_APB2Periph_GPIOD   
#define      ILI9341_RD_PORT               GPIOD
#define      ILI9341_RD_PIN                GPIO_Pin_4

//��λ����
#define      ILI9341_RST_CLK               RCC_APB2Periph_GPIOE
#define      ILI9341_RST_PORT              GPIOE
#define      ILI9341_RST_PIN               GPIO_Pin_1

//��������
#define      ILI9341_BK_CLK                RCC_APB2Periph_GPIOD    
#define      ILI9341_BK_PORT               GPIOD
#define      ILI9341_BK_PIN                GPIO_Pin_12

/********�����ź���***************/
#define      ILI9341_D0_CLK                RCC_APB2Periph_GPIOD   
#define      ILI9341_D0_PORT               GPIOD
#define      ILI9341_D0_PIN                GPIO_Pin_14

#define      ILI9341_D1_CLK                RCC_APB2Periph_GPIOD   
#define      ILI9341_D1_PORT               GPIOD
#define      ILI9341_D1_PIN                GPIO_Pin_15

#define      ILI9341_D2_CLK                RCC_APB2Periph_GPIOD   
#define      ILI9341_D2_PORT               GPIOD
#define      ILI9341_D2_PIN                GPIO_Pin_0

#define      ILI9341_D3_CLK                RCC_APB2Periph_GPIOD  
#define      ILI9341_D3_PORT               GPIOD
#define      ILI9341_D3_PIN                GPIO_Pin_1

#define      ILI9341_D4_CLK                RCC_APB2Periph_GPIOE   
#define      ILI9341_D4_PORT               GPIOE
#define      ILI9341_D4_PIN                GPIO_Pin_7

#define      ILI9341_D5_CLK                RCC_APB2Periph_GPIOE   
#define      ILI9341_D5_PORT               GPIOE
#define      ILI9341_D5_PIN                GPIO_Pin_8

#define      ILI9341_D6_CLK                RCC_APB2Periph_GPIOE   
#define      ILI9341_D6_PORT               GPIOE
#define      ILI9341_D6_PIN                GPIO_Pin_9

#define      ILI9341_D7_CLK                RCC_APB2Periph_GPIOE  
#define      ILI9341_D7_PORT               GPIOE
#define      ILI9341_D7_PIN                GPIO_Pin_10

#define      ILI9341_D8_CLK                RCC_APB2Periph_GPIOE   
#define      ILI9341_D8_PORT               GPIOE
#define      ILI9341_D8_PIN                GPIO_Pin_11

#define      ILI9341_D9_CLK                RCC_APB2Periph_GPIOE   
#define      ILI9341_D9_PORT               GPIOE
#define      ILI9341_D9_PIN                GPIO_Pin_12

#define      ILI9341_D10_CLK                RCC_APB2Periph_GPIOE   
#define      ILI9341_D10_PORT               GPIOE
#define      ILI9341_D10_PIN                GPIO_Pin_13

#define      ILI9341_D11_CLK                RCC_APB2Periph_GPIOE   
#define      ILI9341_D11_PORT               GPIOE
#define      ILI9341_D11_PIN                GPIO_Pin_14

#define      ILI9341_D12_CLK                RCC_APB2Periph_GPIOE   
#define      ILI9341_D12_PORT               GPIOE
#define      ILI9341_D12_PIN                GPIO_Pin_15

#define      ILI9341_D13_CLK                RCC_APB2Periph_GPIOD   
#define      ILI9341_D13_PORT               GPIOD
#define      ILI9341_D13_PIN                GPIO_Pin_8

#define      ILI9341_D14_CLK                RCC_APB2Periph_GPIOD   
#define      ILI9341_D14_PORT               GPIOD
#define      ILI9341_D14_PIN                GPIO_Pin_9

#define      ILI9341_D15_CLK                RCC_APB2Periph_GPIOD   
#define      ILI9341_D15_PORT               GPIOD
#define      ILI9341_D15_PIN                GPIO_Pin_10

/*************************************** ����Ԥ�� ******************************************/
#define      DEBUG_DELAY()                

/***************************** ILI934 ��ʾ�������ʼ������������� ***************************/
#define      ILI9341_DispWindow_X_Star		    0     //��ʼ���X����
#define      ILI9341_DispWindow_Y_Star		    0     //��ʼ���Y����

#define 			ILI9341_LESS_PIXEL	  							240			//Һ�����϶̷�������ؿ���
#define 			ILI9341_MORE_PIXEL	 								320			//Һ�����ϳ���������ؿ���
//����Һ��ɨ�跽����仯��XY���ؿ���
//����ILI9341_GramScan�������÷���ʱ���Զ�����
extern uint16_t LCD_X_LENGTH,LCD_Y_LENGTH; 
//Һ����ɨ��ģʽ
//������ѡֵΪ0-7
extern uint8_t LCD_SCAN_MODE;

/***************** ILI934 ��ʾ��ȫ��Ĭ�ϣ�ɨ�跽��Ϊ1ʱ�������Ⱥ����߶� *********************/
#define      macILI9341_Default_Max_Width		  240     //��ʼ���X����
#define      macILI9341_Default_Max_Heigth		320     //��ʼ���Y����

/********************************** ���� ILI934 ���� ***************************************/
//��Һ�������⣬�������û���������б�����һ�Σ��������
#define			LCD_BK_EN		  GPIO_ResetBits(ILI9341_BK_PORT, ILI9341_BK_PIN)	
//�ر�Һ��������	
#define			LCD_BK_DIS		GPIO_SetBits(ILI9341_BK_PORT, ILI9341_BK_PIN)		

/******************************* ���� ILI934 �������� ********************************/
#define      CMD_SetCoordinateX		 		    0x2A	     //����X����
#define      CMD_SetCoordinateY		 		    0x2B	     //����Y����
#define      CMD_SetPixel		 		          0x2C	     //�������


/********************************** ���� ILI934 ���� ***************************************/
void                     ILI9341_Init                    ( void );
void                     ILI9341_Rst                     ( void );
void                     ILI9341_BackLed_Control         ( FunctionalState enumState );
void                     ILI9341_GramScan                ( uint8_t ucOtion );
void                     ILI9341_OpenWindow              ( uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight );
void                     ILI9341_Clear                   ( uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight );


#endif /* __BSP_ILI9341_ILI9341_H */

