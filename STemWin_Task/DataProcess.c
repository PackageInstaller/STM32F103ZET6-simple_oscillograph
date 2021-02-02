/*	 ADC�������ݴ������������飬ADCԭʼ�������飬WaveBuffʾ�����������飬ShowBuff��ʾ����
*
*
*/

#include <stdio.h>
#include "DSO.h"
#include "stm32f10x.h"
#include "./bsp_adc.h"

WAVEPARAMS WaveParams = 
{
    0, 4095, 0, 0, 0
};


static uint16_t ADC_DMA_Pos;
static uint16_t ADC_DMA_Pos_Last = 0;
static uint16_t DMA_Pos_In_Array;

void CopyDataToWaveBuff(void)
{
    uint16_t i, j;
   
    /* �������ݵ�WaveBuffer������DMA���츲���������� */
    /* ��ȡADC_DMA��ǰλ�� */
    ADC_DMA_Pos = ADC_DataSize - DMA_GetCurrDataCounter(ADC_DMA_CHANNEL);

    //printf("ADC_DMA��ǰλ��1 = %d\n",ADC_DMA_Pos);

    if (ADC_DMA_Pos < WAVE_BUFF_SIZE)
    {
        /* ����У׼��������ʱ��λ�� */
        DMA_Pos_In_Array = WAVE_BUFF_SIZE - ADC_DMA_Pos;

        //printf("pos = %d\n",(ADC_DataSize - Array_DMA_Pos));
        for (j = 0, i = ADC_DataSize - DMA_Pos_In_Array; i < ADC_DataSize; i++, j++)
        {
			WaveBuffer[j] = ADC_ConvertedValue[i]; 
        }
        for (i = 0; i < ADC_DMA_Pos; i++, j++)
        {
			WaveBuffer[j] = ADC_ConvertedValue[i];
        }
    }
    else
    {
        for (j = 0, i = ADC_DMA_Pos - WAVE_BUFF_SIZE; i < ADC_DMA_Pos; i++, j++)
        {
			WaveBuffer[j] = ADC_ConvertedValue[i];
        }
    }


    /* ��WaveBuffer��һЩ����----���ֵ */
    WaveParams.MaxValue = 0;
    WaveParams.MinValue = 4096;
    WaveParams.PPValue = 0;
    for (i = 0; i < WAVE_BUFF_SIZE; i++)
    {

        WaveParams.MaxValue = ( WaveBuffer[i] > WaveParams.MaxValue ) ? WaveBuffer[i] : WaveParams.MaxValue;
        WaveParams.MinValue = ( WaveBuffer[i] < WaveParams.MinValue ) ? WaveBuffer[i] : WaveParams.MinValue;
    }
    WaveParams.PPValue = WaveParams.MaxValue - WaveParams.MinValue;

    

}

