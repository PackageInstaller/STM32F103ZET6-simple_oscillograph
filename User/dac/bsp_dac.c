/**
  ******************************************************************************
  * @file    bsp_xxx.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   adc1 Ӧ��bsp / DMA ģʽ
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��STM32 ָ���� ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://fire-stm32.taobao.com
  *
  ******************************************************************************
  */

#include "./dac/bsp_dac.h"
#include "MainTask.h"

//���Ҳ��������ڵĵ���
#define POINT_NUM 32

/* �������� ---------------------------------------------------------*/
const uint16_t Sine12bit[POINT_NUM] = {
    2048, 2460, 2856, 3218, 3532, 3786, 3969, 4072,
    4093, 4031, 3887, 3668, 3382, 3042, 2661, 2255,
    1841, 1435, 1054, 714, 428, 209, 65, 3,
    24, 127, 310, 564, 878, 1240, 1636, 2048};
uint32_t DualSine12bit[POINT_NUM];

/**
  * @brief  ʹ��DAC��ʱ�ӣ���ʼ��GPIO
  * @param  ��
  * @retval ��
  */
static void DAC_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    DAC_InitTypeDef DAC_InitStructure;

    /* ʹ��GPIOAʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    /* ʹ��DACʱ�� */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

    /* DAC��GPIO���ã�ģ������ */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* ����DAC ͨ��1 */
    DAC_InitStructure.DAC_Trigger = DAC_Trigger_T2_TRGO; //ʹ��TIM2��Ϊ����Դ
    //DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_Triangle; //ʹ�����ǲ�������
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None; //��ʹ�ò��η�����

    //DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_1023;
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable; //��ʹ��DAC�������
    DAC_Init(DAC_Channel_1, &DAC_InitStructure);

    /* ����DAC ͨ��2 */
    DAC_Init(DAC_Channel_2, &DAC_InitStructure);

    /* ʹ��ͨ��1 ��PA4��� */
    DAC_Cmd(DAC_Channel_1, ENABLE);
    /* ʹ��ͨ��2 ��PA5��� */
    DAC_Cmd(DAC_Channel_2, ENABLE);

    /* ʹ��DAC��DMA���� */
    DAC_DMACmd(DAC_Channel_2, ENABLE);
}

/**
  * @brief  ����TIM
  * @param  ��
  * @retval ��
  */
static void DAC_TIM_Config(void)
{

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    /* ʹ��TIM2ʱ�ӣ�TIM2CLK Ϊ72M */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    /* TIM2������ʱ������ */
    // TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = (20 - 1);                 //��ʱ����
    TIM_TimeBaseStructure.TIM_Prescaler = (FrqPscForDAC[9] - 1); //Ԥ��Ƶ 72M / (0+1) = 72M
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;               //ʱ�ӷ�Ƶϵ��
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    /* ����TIM2����Դ */
    TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);

    /* ʹ��TIM2 */
    TIM_Cmd(TIM2, ENABLE);
}

/**
  * @brief  ����DMA
  * @param  ��
  * @retval ��
  */
static void DAC_DMA_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;

    /* ʹ��DMA2ʱ�� */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);

    /* ����DMA2 */
    DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR12RD_ADDRESS;         //�������ݵ�ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&DualSine12bit;        //�ڴ����ݵ�ַ DualSine12bit
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;                      //���ݴ��䷽���ڴ�������
    DMA_InitStructure.DMA_BufferSize = POINT_NUM;                           //�����СΪPOINT_NUM�ֽ�
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        //�������ݵ�ַ�̶�
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 //�ڴ����ݵ�ַ����
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word; //������������Ϊ��λ
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;         //�ڴ���������Ϊ��λ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                         //ѭ��ģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;                     //��DMAͨ�����ȼ�
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                            //���ڴ����ڴ�ģʽ

    DMA_Init(DMA2_Channel4, &DMA_InitStructure);

    /* ʹ��DMA2-14ͨ�� */
    DMA_Cmd(DMA2_Channel4, ENABLE);
}

/**
  * @brief  DAC��ʼ������
  * @param  ��
  * @retval ��
  */
void DAC_Mode_Init(void)
{
    uint32_t Idx = 0;

    DAC_Config();
    DAC_TIM_Config();

    /* ������Ҳ������ݣ�˫ͨ���Ҷ���*/
    for (Idx = 0; Idx < POINT_NUM; Idx++)
    {
        DualSine12bit[Idx] = (Sine12bit[Idx] << 16) + (Sine12bit[Idx]);
    }

    DAC_DMA_Config();
}