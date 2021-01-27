/**
  *********************************************************************
  * @file    MainTask.c
  * @author  fire
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   FreeRTOS v9.0.0 + STM32 ����ģ��
  *********************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F103 ������
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */

/*******************************************************************************
     * ������ͷ�ļ�
     ******************************************************************************/

/* STemWINͷ�ļ� */

#include "MainTask.h"
#include "./bsp_all.h"

typedef struct
{
    int XPos, YPos;
} tDrawBkContext;

/*******************************************************************************
 * ����
 ******************************************************************************/

/* ��������ʾλ�� */
static const char *pT[] = {"SPS:","Fq:"};

const uint16_t SPSPscForADC[SPSPscNums] = {5, 10, 20, 50, 100, 200, 500, 1000};
const char *SPSPscForShow[SPSPscNums] = {"200k*6", "100k*6", "50k*6", "20k*6", "10k*6", "5k*6", "2k*6", "1k*6"};

const uint16_t FrqPscForDAC[FrqPscNums] = {1, 2, 5, 10, 20, 50, 100, 200, 500, 1000};
const char *FrqPscForShow[FrqPscNums] = {"112500", "56250", "22500", "11250", "5625", "2250", "1125", "562.5",
                                         "225", "112.5"};

SHOWPARAMS showParams = { 1, 1, 0 ,0 };

uint16_t t0, delta_t;
static uint32_t paintcount = 0;
static uint32_t xfree = 0;

GUI_RECT GraphFrameWinRect = {8, 18, 255, 205};
GUI_RECT GraphRect = {0, 0, 240, 180};
GUI_RECT DsoLogoRect = {0, 0, 40, 20};

GUI_MEMDEV_Handle hMem_BK;

BUTTON_Handle hButton0;
BUTTON_Handle hButton1;
FRAMEWIN_Handle hFrame0;

TEXT_Handle hText0;
TEXT_Handle hText1;
TEXT_Handle hText2;
TEXT_Handle hText3;
TEXT_Handle hText4;
TEXT_Handle hText5;

#define GraphFrame_ID (GUI_ID_USER + 1)

/*******************************************************************************
 * ����
 ******************************************************************************/

/*
 *********************************************************************************************************
 *	�� �� ��: _cbGraphFrame
 *	����˵��: ���߱����Ļص�����
 *	��    ��: pMsg
 *	�� �� ֵ: ��
 *********************************************************************************************************
 */

void DrawGraph(void)
{
    GUI_SetColor(GUI_YELLOW);
    GUI_DrawGraph((signed short *)ADC_DataShow, GUI_COUNTOF(ADC_DataShow), GraphRect.x0, GraphRect.y1);
}


static void Draw_BK(void *pData);

static void _cbGraphFrame(WM_MESSAGE *pMsg)
{
    int NCode, Id;
    GUI_RECT Rect;
    switch (pMsg->MsgId)
    {
        /* ����ͼ�����ػ� */
    case WM_PAINT:
        WM_GetWindowRectEx(pMsg->hWin, &Rect);
        Draw_BK(NULL);
        DrawGraph();

        break;

    default:
        //printf("Bk_de\n");
        FRAMEWIN_Callback(pMsg);
        break;
    }
}

/*
 *********************************************************************************************************
 *	�� �� ��: _cbButton
 *	����˵��: ��ť�Ļص�����
 *	��    ��: pMsg
 *	�� �� ֵ: ��
 *********************************************************************************************************
 */

static void _cbButton(WM_MESSAGE *pMsg)
{
    WM_HWIN hWin;
    GUI_RECT Rect;
    char buf[5];
    hWin = pMsg->hWin;

    switch (pMsg->MsgId)
    {
    case WM_PAINT:
        WM_GetClientRect(&Rect);
        if (BUTTON_IsPressed(hWin))
        {
            GUI_SetColor(GUI_RED);
            GUI_AA_FillRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 10);

            /* ����Ϊ��װ��� */
            GUI_SetBkColor(GUI_RED);
            GUI_SetColor(GUI_WHITE);
        }
        else
        {
            GUI_SetColor(GUI_LIGHTBLUE);
            GUI_AA_FillRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 10);

            GUI_SetColor(0XEBCD9E);
            GUI_SetPenSize(2);
            GUI_AA_DrawRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 10);

            /* û����Ϊ���׺��� */
            GUI_SetBkColor(GUI_LIGHTBLUE);
            GUI_SetColor(GUI_BLACK);
        }

        BUTTON_GetText(hWin, buf, 20);
        GUI_SetFont(&GUI_Font24B_1);
        BUTTON_SetText(hWin, buf);
        GUI_DispStringInRect(buf, &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
        //printf("Bu_P\n");
        break;

    default:
        //printf("Bu_de\n");
        BUTTON_Callback(pMsg);
        break;
    }
}

/*
 *********************************************************************************************************
 *	�� �� ��: _cbBkWin
 *	����˵��: ���洰�ڵĻص�����
 *	��    ��: pMsg
 *	�� �� ֵ: ��
 *********************************************************************************************************
 */

static void _cbBkWin(WM_MESSAGE *pMsg)
{
    GUI_RECT Rect;
    int NCode, Id;

    switch (pMsg->MsgId)
    {
        /* ���汳���ػ� */
    case WM_PAINT:
        /* ���� */
        GUI_SetBkColor(GUI_BLUE);
        GUI_Clear();

        /* д��DSO */
        GUI_SetBkColor(GUI_BLUE);
        GUI_SetColor(GUI_WHITE);
        GUI_SetFont(&GUI_Font16B_1);
        GUI_DispStringInRect("DSO", &DsoLogoRect, GUI_TA_HCENTER | GUI_TA_VCENTER);
        GUI_DrawLine(240, 0, 240, 240);
        GUI_DrawLine(0, 180, 320, 180);

        paintcount++;
        GUI_DispDecAt(paintcount, 280, 150, 5);
        //printf("Bk_P\n");
        break;

    case WM_NOTIFY_PARENT:
        Id = WM_GetId(pMsg->hWinSrc);
        NCode = pMsg->Data.v;
        printf("Id=%d\n", Id);
        switch (Id)
        {
        case GUI_ID_BUTTON0: // Notifications sent by 'Button0'
            switch (NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                printf("delta t = %dms\n", delta_t);
                showParams.xShift += 10;
                LED1(ON);
                break;
            case WM_NOTIFICATION_RELEASED:
                LED1(OFF);

                break;
            }
            break;

        case GUI_ID_BUTTON1: // Notifications sent by 'Button0'
            switch (NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                printf("Free = %dBytes\n", xfree);
                showParams.yShift += 10;
                LED2(ON);
                break;
            case WM_NOTIFICATION_RELEASED:
                LED2(OFF);
                break;
            }
            break;
        }

    default:
        WM_DefaultProc(pMsg);
        break;
    }
}

/**
 *
  * @brief GUI������
  * @note ��
  * @param ��
  * @retval ��
  */

static void Draw_BK(void *pData)
{
    int x = 0;
    int y = 0;

    int Grid_width = 30;
    int Point_width = 6;

    int i, j, k;

    GUI_SetBkColor(GUI_BLACK);
    GUI_ClearRect(x, y, x + 240, y + 180);

    GUI_SetColor(GUI_WHITE);

    //ˮƽ�ĵ�
    for (k = 0; k < 6; k++)
    {
        for (j = 0; j < 8; j++)
        {
            for (i = 0; i < 5; i++)
            {
                GUI_DrawPoint(x + j * Grid_width + i * Point_width, y + k * 30);
            }
        }
    }

    //��ֱ�ĵ�
    for (k = 0; k < 8; k++)
    {
        for (j = 0; j < 6; j++)
        {
            for (i = 0; i < 5; i++)
            {
                GUI_DrawPoint(x + k * 30, y + j * Grid_width + i * Point_width);
            }
        }
    }

    for (j = 0; j < 6; j++)
    {
        for (i = 0; i < 5; i++)
        {
            GUI_DrawPoint(x + 240, y + j * Grid_width + i * Point_width);
        }
    }

    for (j = 0; j < 8; j++)
    {
        for (i = 0; i < 5; i++)
        {
            GUI_DrawPoint(x + j * Grid_width + i * Point_width, y + 180);
        }
    }

    GUI_DrawPoint(x + 240, y + 180);
}

static void Widget_Init()
{

    hFrame0 = FRAMEWIN_CreateEx(GraphFrameWinRect.x0,
                                GraphFrameWinRect.y0,
                                GraphFrameWinRect.x1 - GraphFrameWinRect.x0,
                                GraphFrameWinRect.y1 - GraphFrameWinRect.y0,
                                0,
                                WM_CF_SHOW,
                                0,
                                GraphFrame_ID,
                                "",
                                _cbGraphFrame);
    FRAMEWIN_SetSkinClassic(hFrame0);
    FRAMEWIN_SetTitleVis(hFrame0, 0);

    hButton0 = BUTTON_CreateEx(270, 20, 40, 40, 0, WM_CF_SHOW, 0, GUI_ID_BUTTON0);
    BUTTON_SetText(hButton0, "M");
    WM_SetCallback(hButton0, _cbButton);

    hButton1 = BUTTON_CreateEx(270, 80, 40, 40, 0, WM_CF_SHOW, 0, GUI_ID_BUTTON1);
    BUTTON_SetText(hButton1, "AD");
    WM_SetCallback(hButton1, _cbButton);

    //hText0 = TEXT_CreateEx(20, 205, 100, 30, 0, WM_CF_SHOW, GUI_TA_HCENTER | GUI_TA_VCENTER, GUI_ID_TEXT0 ,pT);
    hText0 = TEXT_Create(20, 205, 50, 30, GUI_ID_TEXT0, WM_CF_SHOW, NULL, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hText0, GUI_WHITE);
    TEXT_SetText(hText0,SPSPscForShow[SRS_Select]);

    hText1 = TEXT_Create(70, 205, 50, 30, GUI_ID_TEXT1, WM_CF_SHOW, NULL, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hText1, GUI_WHITE);
    TEXT_SetText(hText1,FrqPscForShow[Frq_Select]);

    hText2 = TEXT_Create(120, 205, 50, 30, GUI_ID_TEXT1, WM_CF_SHOW, NULL, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hText2, GUI_WHITE);
    TEXT_SetDec(hText2, PPValue, 4, 0, 0, 0);

    WM_SetCallback(WM_HBKWIN, &_cbBkWin);
}

static void DSO_Init(void)
{
    AutoTriggerLevel = 1024;
    Frq_Select = 4;
    
}

/*  ʹ�÷ֶ΃ȴ��O�仭2dͼ�η�����57ms������ */
static void Graph_Refresh_2(void *pData) 
{
    Draw_BK(pData);
    DrawGraph();
}

static void Graph_Refresh(void)
{
    WM_HWIN hWin;
    GUI_RECT Rect;

    /* ��������ݺ�����������ʱ�� 0ms */
    ShowBufferProcess(DataProcess(), showParams.xSacleFactor, showParams.ySacleFactor, showParams.xShift, showParams.yShift);

    TEXT_SetDec(hText2, PPValue, 4, 0, 0, 0);

    /* �п���ػ棬39ms,�ȷֶ��ڴ��豸��ܶ� */
    hWin = WM_GetClientWindow(hFrame0);
    WM_Paint(hWin);
}

void MainTask(void)
{
    WM_SetCreateFlags(WM_CF_MEMDEV);
    GUI_Init();
    GUI_CURSOR_Show();
    GUI_SetBkColor(GUI_BLUE_98);
    GUI_Clear();

    //DSO_Init();
    Widget_Init();

    while (1)
    {
        xfree = GUI_ALLOC_GetNumFreeBytes();
        //t0 = GUI_GetTime();

        Graph_Refresh();

        //delta_t = GUI_GetTime() - t0;

#if 1
        GUI_Delay(50);

#elif 0
        /* �ȽϾ��Ե���ʱ */
        if (delta_t > 101)
        {
            printf("��ʱ����gg");
            while (1)
                ;
        }
        else
        {
            GUI_Delay(200 - delta_t);
        }
#endif
    }
}
