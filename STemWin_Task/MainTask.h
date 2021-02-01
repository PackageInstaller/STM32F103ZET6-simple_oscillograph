#ifndef  _MAINTASK_H_
#define  _MAINTASK_H_

#include "DSO.h"


typedef struct {                //  �ı���ͨ�ýṹ��
    int Handle;
    const char* stitle;
    char* sinfo;
    WIGET Text;
    int CornerSize;
    int Space;
}TEXTSTRUCT;


typedef struct {                // ����Ԥ��С���ṹ��
    int Handle;
    int TriggerPos;
    int StartPos;
    int CoverLength;
    WIGET size;
}GRAPHPREWIN_STRUCT;

typedef struct {                //  ʾ������ʾ�ı��ṹ��
    char TimeBase[10];
    char VoltageBase[10];
    char XPos[10];
    char YPos[10];
    char TriggerLevel[10];
}DSOALLSTRING;


//  �ұߵ�8���ı������ʾ��ȫ������
#define     RIGHTTEXT_XSIZE             50
#define     RIGHTTEXT_YSIZE             23
#define     RIGHTTEXT_HALF_YSIZE        (int)(RIGHTTEXT_YSIZE / 2)
#define     RIGHTTEXT_CSIZE             4
#define     RIGHTTEXT_SPACE             1
#define     RIGHTTEXT_MAXNUMS           8



// ���·�4���ı�����ʾ��
#define     BOTTOMTEXT_XSIZE             82
#define     BOTTOMTEXT_YSIZE             16
#define     BOTTOMTEXT_SPACE             2
#define     BOTTOMTEXT_MAXNUMS           3

// �Ϸ��ı���
#define     UPTEXT_L_XSIZE             28
#define     UPTEXT_R_XSIZE             60
#define     UPTEXT_YSIZE               16
#define     UPTEXT_SPACE               2
#define     UPTEXT_MAXNUMS             3

// �Ϸ�����Ԥ����
#define     GRAPHPRE_XSIZE               127
#define     GRAPHPRE_YSIZE               16
#define     GRAPHPRE_MAXNUMS             1

enum { channel, tbase, vbase, xpos, ypos, trlevel, mode, trmode };
enum { vpp, freq, period, bottomTBD };
enum { status, upTBD, sps };
enum { R, B, U};

I16 GetTextHandle(I8 Position, I8 Index);


#endif  //end _MAINTASK_H_



