/**
  *********************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   FreeRTOS V9.0.0  + STM32 �̼�������
  *********************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� STM32 ȫϵ�п����� 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  **********************************************************************
  */

/*
*************************************************************************
*                             ������ͷ�ļ�
*************************************************************************
*/
/* FreeRTOSͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
/* ������Ӳ��bspͷ�ļ� */
#include "./bsp_all.h"
/* STemWINͷ�ļ� */
#include "GUI.h"
#include "DIALOG.h"
#include "MainTask.h"

/* FATFS */
#include "ff.h"
#include "diskio.h"
#include "integer.h"

/**************************** ������ ********************************/
/* 
 * ��������һ��ָ�룬����ָ��һ�����񣬵����񴴽���֮�����;�����һ��������
 * �Ժ�����Ҫ��������������Ҫͨ��������������������������������Լ�����ô
 * ����������ΪNULL��
 */
/* ���������� */
static TaskHandle_t AppTaskCreate_Handle = NULL;
/* LED������ */
static TaskHandle_t DataProcess_Task_Handle = NULL;
/* Touch������ */
static TaskHandle_t Touch_Task_Handle = NULL;
/* GUI������ */
static TaskHandle_t GUI_Task_Handle = NULL;
static TaskHandle_t CPU_Task_Handle = NULL;

/********************************** �ں˶����� *********************************/
/*
 * �ź�������Ϣ���У��¼���־�飬������ʱ����Щ�������ں˵Ķ���Ҫ��ʹ����Щ�ں�
 * ���󣬱����ȴ����������ɹ�֮��᷵��һ����Ӧ�ľ����ʵ���Ͼ���һ��ָ�룬������
 * �ǾͿ���ͨ��������������Щ�ں˶���
 *
 * �ں˶���˵���˾���һ��ȫ�ֵ����ݽṹ��ͨ����Щ���ݽṹ���ǿ���ʵ��������ͨ�ţ�
 * �������¼�ͬ���ȸ��ֹ��ܡ�������Щ���ܵ�ʵ��������ͨ��������Щ�ں˶���ĺ���
 * ����ɵ�
 * 
 */

/******************************* ȫ�ֱ������� ************************************/
/*
 * ��������дӦ�ó����ʱ�򣬿�����Ҫ�õ�һЩȫ�ֱ�����
 */
FATFS fs; /* FatFs�ļ�ϵͳ���� */
FIL file; /* file objects */
UINT bw;  /* File R/W count */
FRESULT result;
FILINFO fno;
DIR dir;

int8_t SRS_Select = 5;
int8_t Frq_Select = 4;

/*
*************************************************************************
*                             ��������
*************************************************************************
*/
static void AppTaskCreate(void); /* ���ڴ������� */

static void DataProcess_Task(void *parameter); /* DataProcess_Task */
static void GUI_Task(void *parameter);         /* GUI_Task����ʵ�� */
static void Touch_Task(void *parameter);
static void CPU_Task(void *pvParameters); /* CPU_Task����ʵ�� */

static void BSP_Init(void); /* ���ڳ�ʼ�����������Դ */

/*****************************************************************
  * @brief  ������
  * @param  ��
  * @retval ��
  * @note   ��һ����������Ӳ����ʼ�� 
            �ڶ���������APPӦ������
            ������������FreeRTOS����ʼ���������
  ****************************************************************/
int main(void)
{
    BaseType_t xReturn = pdPASS; /* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */

    /* ������Ӳ����ʼ�� */
    BSP_Init();

    printf("\r\n ********** emwin DEMO *********** \r\n");

    /* ����AppTaskCreate���� */
    xReturn = xTaskCreate((TaskFunction_t)AppTaskCreate,          /* ������ں��� */
                          (const char *)"AppTaskCreate",          /* �������� */
                          (uint16_t)512,                          /* ����ջ��С */
                          (void *)NULL,                           /* ������ں������� */
                          (UBaseType_t)1,                         /* ��������ȼ� */
                          (TaskHandle_t *)&AppTaskCreate_Handle); /* ������ƿ�ָ�� */
    /* ����������� */
    if (pdPASS == xReturn)
        vTaskStartScheduler(); /* �������񣬿������� */
    else
        return -1;

    while (1)
        ; /* ��������ִ�е����� */
}

/***********************************************************************
  * @ ������  �� AppTaskCreate
  * @ ����˵���� Ϊ�˷�����������е����񴴽����������������������
  * @ ����    �� ��  
  * @ ����ֵ  �� ��
  **********************************************************************/
static void AppTaskCreate(void)
{
    BaseType_t xReturn = pdPASS; /* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */

    taskENTER_CRITICAL(); //�����ٽ���

    xReturn = xTaskCreate((TaskFunction_t)DataProcess_Task,        /* ������ں��� */
                          (const char *)"DataProcess_Task",        /* �������� */
                          (uint16_t)256,                           /* ����ջ��С */
                          (void *)NULL,                            /* ������ں������� */
                          (UBaseType_t)10,                         /* ��������ȼ� */
                          (TaskHandle_t)&DataProcess_Task_Handle); /* ������ƿ�ָ�� */
    if (pdPASS == xReturn)
        printf("����DataProcess_Task����ɹ���\r\n");

    xReturn = xTaskCreate((TaskFunction_t)Touch_Task,        /* ������ں��� */
                          (const char *)"Touch_Task",        /* �������� */
                          (uint16_t)256,                     /* ����ջ��С */
                          (void *)NULL,                      /* ������ں������� */
                          (UBaseType_t)4,                    /* ��������ȼ� */
                          (TaskHandle_t)&Touch_Task_Handle); /* ������ƿ�ָ�� */
    if (pdPASS == xReturn)
        printf("����Touch_Task����ɹ���\r\n");

    xReturn = xTaskCreate((TaskFunction_t)GUI_Task,        /* ������ں��� */
                          (const char *)"GUI_Task",        /* �������� */
                          (uint16_t)2048,                  /* ����ջ��С */
                          (void *)NULL,                    /* ������ں������� */
                          (UBaseType_t)3,                  /* ��������ȼ� */
                          (TaskHandle_t)&GUI_Task_Handle); /* ������ƿ�ָ�� */
    if (pdPASS == xReturn)
        printf("����GUI_Task����ɹ���\r\n");

    xReturn = xTaskCreate((TaskFunction_t)CPU_Task,          /* ������ں��� */
                          (const char *)"CPU_Task",          /* �������� */
                          (uint16_t)256,                     /* ����ջ��С */
                          (void *)NULL,                      /* ������ں������� */
                          (UBaseType_t)5,                    /* ��������ȼ� */
                          (TaskHandle_t *)&CPU_Task_Handle); /* ������ƿ�ָ�� */
    if (pdPASS == xReturn)
        printf("����CPU_Task����ɹ�!\r\n");

    vTaskDelete(AppTaskCreate_Handle); //ɾ��AppTaskCreate����

    taskEXIT_CRITICAL(); //�˳��ٽ���
}

/**
  * @brief LED��������
  * @note ��
  * @param ��
  * @retval ��
  */


static void DataProcess_Task(void *parameter)
{
    
    while (1)
    {
        /* ������ */
        if (Key_Scan(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == KEY_ON)
        {
            SRS_Select++;
            if (SRS_Select == SPSPscNums)
            {
                SRS_Select = 0;
            }

            TIM_PrescalerConfig(TIM3, SPSPscForADC[SRS_Select] - 1, TIM_PSCReloadMode_Immediate);
            TEXT_SetText(hText0,SPSPscForShow[SRS_Select]);
            //TEXT_SetDec(hText0, 5555, 6, 0, 0, 0);
            //WM_SendMessageNoPara(hText0, WM_PAINT);
        }

        /* DACƵ�� */
        if (Key_Scan(KEY2_GPIO_PORT, KEY2_GPIO_PIN) == KEY_ON)
        {
            Frq_Select++;
            if (Frq_Select == FrqPscNums)
            {
                Frq_Select = 0;
            }

            TIM_PrescalerConfig(TIM2, FrqPscForDAC[Frq_Select] - 1, TIM_PSCReloadMode_Immediate);
            TEXT_SetText(hText1,FrqPscForShow[Frq_Select]);
        }
        //DataToDisplay();
        vTaskDelay(20);
    }
}

/**
  * @brief ���������������
  * @note ��
  * @param ��
  * @retval ��
  */
static void Touch_Task(void *parameter)
{
    while (1)
    {
        GUI_TOUCH_Exec(); //��������ʱɨ��
        vTaskDelay(10);
    }
}

/**
  * @brief GUI��������
  * @note ��
  * @param ��
  * @retval ��
  */
static void GUI_Task(void *parameter)
{
    /* ��ʼ��STemWin */

    while (1)
    {
        MainTask();
    }
}

static void CPU_Task(void *parameter)
{
    uint8_t CPU_RunInfo[400]; //������������ʱ����Ϣ

    while (1)
    {
        memset(CPU_RunInfo, 0, 400); //��Ϣ����������

        vTaskList((char *)&CPU_RunInfo); //��ȡ��������ʱ����Ϣ

        printf("---------------------------------------------\r\n");
        printf("������      ����״̬ ���ȼ�   ʣ��ջ �������\r\n");
        printf("%s", CPU_RunInfo);
        printf("---------------------------------------------\r\n");

        memset(CPU_RunInfo, 0, 400); //��Ϣ����������

        vTaskGetRunTimeStats((char *)&CPU_RunInfo);

        printf("������       ���м���         ������\r\n");
        printf("%s", CPU_RunInfo);
        printf("---------------------------------------------\r\n\n");
        vTaskDelay(20000); /* ��ʱ500��tick */
    }
}

/**
  * @brief �弶�����ʼ��
  * @note ���а����ϵĳ�ʼ�����ɷ��������������
  * @param ��
  * @retval ��
  */
static void BSP_Init(void)
{
    /* CRC��emWinû�й�ϵ��ֻ������Ϊ�˿�ı���������
   * ����STemWin�Ŀ�ֻ������ST��оƬ���棬���оƬ���޷�ʹ�õġ�
   */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);

    /*
	 * STM32�ж����ȼ�����Ϊ4����4bit��������ʾ��ռ���ȼ�����ΧΪ��0~15
	 * ���ȼ�����ֻ��Ҫ����һ�μ��ɣ��Ժ������������������Ҫ�õ��жϣ�
	 * ��ͳһ��������ȼ����飬ǧ��Ҫ�ٷ��飬�мɡ�
	 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    DAC_Mode_Init();

    ADCx_Init();
    /* LED ��ʼ�� */
    LED_GPIO_Config();

    /* ��������ʼ�� */
    Beep_Init();

    /* ���ڳ�ʼ��	*/
    USART_Config();

    /* ������ʼ��	*/
    Key_GPIO_Config();

    /* ��������ʼ�� */
    XPT2046_Init();

    BASIC_TIM_Init();
}

/********************************END OF FILE****************************/