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

//���Ҳ��������ڵĵ���
#define POINT_NUM 128

/* �������� ---------------------------------------------------------*/
const uint16_t Sine12bit[POINT_NUM] = {
  2000,2073,2147,2220,2292,2364,
  2435,2505,2574,2641,2707,2771,
  2833,2893,2951,3007,3060,3111,
  3159,3204,3247,3286,3322,3355,
  3385,3412,3435,3455,3471,3483,
  3492,3498,3499,3498,3492,3483,
  3471,3455,3435,3412,3385,3355,
  3322,3286,3247,3204,3159,3111,
  3060,3007,2951,2893,2833,2771,
  2707,2641,2574,2505,2435,2364,
  2292,2220,2147,2073,2000,1926,
  1852,1779,1707,1635,1564,1494,
  1425,1358,1292,1228,1166,1106,
  1048,992,939,888,840,795,
  752,713,677,644,614,587,
  564,544,528,516,507,501,
  500,501,507,516,528,544,
  564,587,614,644,677,713,
  752,795,840,888,939,992,
  1048,1106,1166,1228,1292,1358,
  1425,1494,1564,1635,1707,1779,
  1852,1926,
};
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
    TIM_TimeBaseStructure.TIM_Period = (3 - 1);                 //��ʱ����
    TIM_TimeBaseStructure.TIM_Prescaler = (75 - 1); //Ԥ��Ƶ 72M / (0+1) = 72M
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


/**
* @brief  ����DACƵ��
* @param  ��
* @retval ��
*/
uint32_t SetDACFreq(uint32_t Freq)
{
    TIM_PrescalerConfig(TIM2, 187500 / Freq, TIM_PSCReloadMode_Immediate);
    return Freq;
}

