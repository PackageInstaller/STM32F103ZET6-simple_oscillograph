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

#include "GUI.h"
#include "DIALOG.h"
#include "./bsp_all.h"
#include "MainTask.h"


void MainTask(void)
{   
    GUI_Init();
    GUI_CURSOR_Show();
    WM_SetCreateFlags(WM_CF_MEMDEV);
    GUI_SetBkColor(GUI_BLUE_98);
    GUI_Clear();

    GUI_DispStringAt("Hello World",10,10);
    while (1)
    {
        GUI_Delay(50);
    }
    
}
