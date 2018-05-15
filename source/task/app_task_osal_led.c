/*******************************************************************************
* Description  : LEDָʾ������
* Author       : 2018/5/15 ���ڶ�, by redmorningcn
*******************************************************************************/


/*******************************************************************************
* INCLUDES
*/
#include <includes.h>

#ifdef VSC_INCLUDE_SOURCE_FILE_NAMES
const  CPU_CHAR  *app_task_led__c = "$Id: $";
#endif

/*******************************************************************************
* CONSTANTS
*/

/*******************************************************************************
* MACROS
*/
#define     RUN_LED             1
#define     X1_SPEED_LED        2
#define     X2_DTU_LED          3
#define     X3_TAX_LED          4
#define     X4_VOL_LED          5

/*******************************************************************************
* TYPEDEFS
*/

/*******************************************************************************
* LOCAL VARIABLES
*/


/*******************************************************************************
* GLOBAL VARIABLES
*/

/*******************************************************************************
* LOCAL FUNCTIONS
*/
#if ( OSAL_EN == DEF_ENABLED )
#else
static  void    AppTaskLed           (void *p_arg);
#endif

/*******************************************************************************
* GLOBAL FUNCTIONS
*/

/*******************************************************************************
* EXTERN VARIABLES
*/

/*******************************************************************************/

/*******************************************************************************
* ��    �ƣ� AppTaskLed
* ��    �ܣ� ��������
* ��ڲ����� p_arg - �����񴴽���������
* ���ڲ����� ��
* ��    �ߣ� wumingshen.
* �������ڣ� 2015-02-05
* ��    �ģ�
* �޸����ڣ�
*******************************************************************************/
osalEvt  TaskLedEvtProcess(osalTid task_id, osalEvt task_event)
{
    /***********************************************
    * ������ �������Ź���־��λ
    */
    //OSSetWdtFlag(( OS_FLAGS     ) WDT_FLAG_LED);

    if( task_event & OS_EVT_LED_TICKS ) {
        static  uint8   mode = 0;
        
        /***********************************************
        * ������ ������ʾ
        */
        while( mode < 10) {
            
            if(mode==0 || mode == 9)    
                BSP_LED_Off(0);         //ָʾ��ȫ��
            else
            {
                BSP_LED_On(mode);       //���ε���ָʾ��
            }
            mode++;

            osal_start_timerEx( OS_TASK_ID_LED,
                                OS_EVT_LED_TICKS,
                                500);
            return ( task_event ^ OS_EVT_LED_TICKS );
        }
        
        /***********************************************
        * ����������״̬����ָʾ����˸
        */
//        if( !Ctrl.Mtr.ConnCtrl[0].ErrFlg ){
//            BSP_LED_Toggle(BSP_LED_DP1);                  //��Ӧָʾ�Ƶ���
//        }else{
//            BSP_LED_Off(BSP_LED_DP1);                     //��Ӧָʾ�Ƶ�ر�
//        }      
     
        
        BSP_LED_Toggle(RUN_LED);                            //����ָʾ��                    
        /***********************************************
        * ������ ��ʱ������
        */
        osal_start_timerEx( OS_TASK_ID_LED,
                            OS_EVT_LED_TICKS,
                            50);
        return ( task_event ^ OS_EVT_LED_TICKS );
    }
    
    return 0;
}

/*******************************************************************************
* ��    �ƣ�APP_LedInit
* ��    �ܣ������ʼ��
* ��ڲ�������
* ���ڲ�������
* ��    �ߣ�redmorningcn.
* �������ڣ�2017-05-15
* ��    �ģ�
* �޸����ڣ�
*******************************************************************************/
void TaskInitLed(void)
{   
    /***********************************************
    * ������ �ڿ��Ź���־��ע�᱾����Ŀ��Ź���־
    */
    //OSRegWdtFlag(( OS_FLAGS     )WDT_FLAG_LED );
    
    /*************************************************
    * �����������¼���ѯ
    */   
    osal_start_timerEx( OS_TASK_ID_LED,
                       OS_EVT_LED_TICKS,
                       OS_TICKS_PER_SEC);
}

/*******************************************************************************
* 				                    end of file                                *
*******************************************************************************/