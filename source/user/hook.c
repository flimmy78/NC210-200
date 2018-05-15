/*******************************************************************************
* Description  : 通用系统或文件中定制的内容移到此处（便于移植）
如：操作系统中的钩子函数
* Author       : 2018/5/11 星期五, by redmorningcn
*******************************************************************************/
#include <tasks.h>

extern  void App_ModbusInit(void);
extern  void  App_OS_SetAllHooks (void);

/*******************************************************************************
* Description  : BSP初始化钩子函数，在任务执行前运行(不能有和操作系统有关的操作)
* Author       : 2018/5/11 星期五, by redmorningcn
*******************************************************************************/
void    BSP_Init_Hook(void)
{
    App_ModbusInit();                   //初始化串口及串口控制信息
    
}

/*******************************************************************************
* Description  : 操作系统任务创建钩子函数
* Author       : 2018/5/11 星期五, by redmorningcn
*******************************************************************************/
void OS_TaskCreateHook(void)
{     
    //设备初始化
    BSP_Ds3231Init();                   //初始化时钟芯片

    
    /***********************************************
    * 描述： 设置UCOS钩子函数
    */
    App_OS_SetAllHooks();
    
    App_TaskOsalCreate();               //创建osal任务。           
}

/*******************************************************************************
* Description  : 在创建OSA任务时，调用的钩子函数。
* Author       : 2018/5/11 星期五, by redmorningcn
*******************************************************************************/
void OSAL_TaskCreateHook(void)
{
    osal_add_Task(TaskInitTmr,                      // 任务初始化函数指针
                  TaskTmrEvtProcess,                // 任务事件处理函数指针
                  OS_TASK_PRO_TMR,                  // 任务优先级
                  OS_TASK_ID_TMR);                  // 任务ID
    
    osal_add_Task(TaskInitLed,                  // 任务初始化函数指针
                  TaskLedEvtProcess,                // 任务事件处理函数指针
                  OS_TASK_PRO_LED,                  // 任务优先级
                  OS_TASK_ID_LED);                  // 任务ID    
}

