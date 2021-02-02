#ifndef  _DSO_H_
#define  _DSO_H_

#include "DIALOG.h"


typedef struct { I16 width, height, pointspace; } GRID;
typedef struct { I16 x0, y0, xsize, ysize; } WIGET;

#define     NUMS_PER_mV               ((double)4096 / (double)3300)

#define     SAMPLE_DEPTH              2048          //  �洢���1000 * 2�ֽڣ�short��
#define     WAVE_BUFF_SIZE            (SAMPLE_DEPTH / 2)    
#define     SHOW_BUFF_SIZE            250           //  һ����Ļ��ʾ250����
#define     MID_POS_WAVEBUF           (WAVE_BUFF_SIZE / 2)  
#define     MID_POS_SHOWBUF           (SHOW_BUFF_SIZE / 2)  

#define     POINTS_ONE_MOVE           (5l)          //  ÿ��������һС���� 5 ����

#define     VOLTAGEMAXGRADE            8           // ��ѹ��λ 0-11,      ��1mVΪ��λ1
#define     VOLTAGE_DEFAULT_GRADE      8

#define     SPSMAXGRADE                10           // �����ʵ�λ 0-10,    ��5usΪ��λ1
#define     SPS_DEFAULT_GRADE          5

#define     DACMAXGRADE                5           // DAC��λ 0-5
#define     DAC_DEFAULT_GRADE          0    


#define     LEFT_LIMIT                MID_POS_SHOWBUF 
#define     RIGHT_LIMIT               (WAVE_BUFF_SIZE - MID_POS_SHOWBUF)

#define     START_FROM_TRIGGER                  0
#define     INCREASE_XPOS                       1
#define     TRIGGER_AS_END                      2
#define     DECREASE_XPOS                       3
#define     ENOUGH                              10

typedef struct {                //  �����ʺ�ʱ����Ӧ
    I32 SPS;
    I32 TIMEBASE;
}DSO_MATCH_2PARAMS;


typedef struct {                //  ʾ�����������
    //I32 SampleRate;
    const I16 Depth;
    char TriggerMode;
    I16 TriggerLevel;
    I32 XPos;
    I32 YPos;
    I8 TimeBaseGrade;
    I8 VoltageBaseGrade;
}DSOPARAMS;


typedef struct {                //  ʾ������ʾ����
    I16 TriggerPos;
    double XBufPos;
    I16 ShowStartPos;

    double YRatio;
    I16 YStartPos;
}DSOSHOWPARAMS;

typedef struct {                //  ���β���
    I16 MaxValue;
    I16 MinValue;
    I16 PPValue;

    double Freq;
    I32 Period;
}WAVEPARAMS;

typedef struct {                //  DAC����
    I32 DACFreqGrade;
    I8 DACMode;
}DACPARAMS;

// �ⲿ����
extern long WaveBuffer[WAVE_BUFF_SIZE];
extern long MagArray[WAVE_BUFF_SIZE/2];                         //fft��ֵ   

extern short GraphShowBuffer[SHOW_BUFF_SIZE];

extern DSOPARAMS DSOParams;
extern DSOSHOWPARAMS DSOShowParams;
extern WAVEPARAMS WaveParams;
extern DACPARAMS DACParams;

extern const DSO_MATCH_2PARAMS _tgrade[SPSMAXGRADE + 1];
extern const I16 _vgrade[VOLTAGEMAXGRADE + 1];
extern const I32 _DACgrade[DACMAXGRADE + 1];


void CalShowStartPos(void);
void CopyDataToWaveBuff(void);

void FFT_GetFreq(I32 SPS);


#endif  //end _DSO_H_

