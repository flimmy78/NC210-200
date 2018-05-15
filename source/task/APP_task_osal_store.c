/*******************************************************************************
* Description  : ���ݼ�¼�洢����ȡ����
* Author       : 2018/5/15 ���ڶ�, by redmorningcn
*******************************************************************************/


/*******************************************************************************
* INCLUDES
*/
#include <includes.h>


#ifdef VSC_INCLUDE_SOURCE_FILE_NAMES
const  CPU_CHAR  *app_task_store__c = "$Id: $";
#endif

#define APP_TASK_STORE_EN     DEF_ENABLED
#if APP_TASK_STORE_EN == DEF_ENABLED
/*******************************************************************************
* CONSTANTS
*/

/*******************************************************************************
* MACROS
*/
#define	START_EVT		        31
#define MAX_STORE_TYPE          10

/*******************************************************************************
* TYPEDEFS
*/

/*******************************************************************************
* LOCAL VARIABLES
*/
/***********************************************


/*******************************************************************************
* GLOBAL VARIABLES
*/

/*******************************************************************************
* LOCAL FUNCTIONS
*/


/*******************************************************************************
* GLOBAL FUNCTIONS
*/

/*******************************************************************************
* EXTERN VARIABLES
*/


/*******************************************************************************/


/*******************************************************************************
* ��    �ƣ� GetRecFlashAddr
* ��    �ܣ� ȡ���ݼ�¼��ַ��
* ��ڲ����� �洢��¼��
* ���ڲ����� flash�洢��ַ
* ��    �ߣ� redmornigcn
* �������ڣ� 2017-05-15
* ��    �ģ�
* �޸����ڣ�
*******************************************************************************/
static u32  GetRecFlashAddr(u32 FlshRecNum)
{				
    return  (u32)(((FlshRecNum * sizeof(stcFlshRec)) % FLSH_MAX_SIZE)); 			
}


/*******************************************************************************
* ��    �ƣ�RoadNum;        		StoreData
* ��    �ܣ�RelRoadNum;     		���ݴ洢���������ݼ�¼�Ž����ݴ洢��ָ��
StationNum;    		flash��ַ;���¼�¼�ŵ���Ϣ���󲿷����ݼ�¼��
E_StationNum;  		��������������ֱ�Ӷ�sCtrl.sRec �и��¡��ٲ������������ڸú����и��¡�
* ��ڲ�����SignalTyp;      	��
* ���ڲ�����LocoSign;       	��
* ��    ��                 �� 	redmornigcn
* �������ڣ�LocoWorkState;  	2017-05-15
* ��    �ģ�LocoState;     
* �޸����ڣ�
*******************************************************************************/
u8 SaveOneREcord(u32 *id,u8 *buf,u32 len)
{	
    /***********************************************
    * ������ 2017/12/17,�����򣺼������ݼ�¼��ַ
    */
    u32     FlshAddr    = GetRecFlashAddr( *id );		
    u8      ret         = 0;
    u32     size;
    
//    stcFlshRec sRec,gRec;
//    /***********************************************
//    * ������ 2017/12/17,������
//    */

//    /***********************************************
//    * ������ 2017/12/17,������
//    */  
//    u8     retrys      = 3;
//    uint16_t    CRC_sum1;
//    uint16_t    CRC_sum2;
//    /***********************************************
//    * ������ 2017/12/17,������
//    */
//    do {
//        // ���ݴ洢��flash
//        ret = WriteFlshRec(FlshAddr, (stcFlshRec *)&sRec);
//        // ��FLASH�ж�ȡ�������жԱ�
//        ReadFlshRec((stcFlshRec *)&gRec, Ctrl.sRecNumMgr.Current);
//        /**************************************************
//        * ������ ����У��
//        */
//        CRC_sum1 = GetCrc16Chk((u8*)&gRec,size-2);
//        CRC_sum2 = gRec.CrcCheck; 
//        if(CRC_sum1 == CRC_sum2) {
//            u32 *p1 = (u32 *)&gRec;
//            u32 *p2 = (u32 *)&sRec;
//            for ( int i = 0; i < 128 / 4; i++) {
//                if ( p1[i] != p2[i] ) {
//                    //Ctrl.sRunPara.SysErr   = (2<<4) + (0<<2) + (1<<0);
//                    goto next;
//                }
//            }
//            //Ctrl.sRunPara.SysErr   &= ~((2<<4) + (0<<2) + (1<<0));
//            // 	�������ݼ�¼��
//            (*id)++;
//            FRAM_StoreRecNumMgr((StrRecNumMgr  *)&Ctrl.sRecNumMgr); //���ݼ�¼�ż�1��������
//            ret = DEF_TRUE;
//            break;
//        } else {
//        next:
//            ret = DEF_FALSE;
//        }
//    } while ( --retrys );
//    
//    
//    if ( ret == DEF_FALSE ) {
//        Ctrl.sRunPara.Err.Flag.FlashErr     = TRUE & (~Ctrl.sRunPara.ErrMask.Flag.FlashErr);                  //D14=1���洢������
//        do {
//            (*id)++;
//        } while ((*id) * 128 % 4096 != 0);
//        //(*id)++;
//        FRAM_StoreRecNumMgr((StrRecNumMgr  *)&Ctrl.sRecNumMgr);     //���ݼ�¼�ż�1��������
//    } else {
//        Ctrl.sRunPara.Err.Flag.FlashErr     = FALSE;                //D14=1���洢������
//    }
//    
    return ret;
}

/*******************************************************************************
* ��    �ƣ� ReadFlshRec
* ��    �ܣ� ���ݼ�¼�ţ�ȡ���ݼ�¼
* ��ڲ����� ��
* ���ڲ����� ��
* ��    �ߣ� redmornigcn
* �������ڣ� 2017-05-15
* ��    �ģ�
* �޸����ڣ�
*******************************************************************************/
u8	ReadFlshRec(stcFlshRec * sFlshRec,u32 FlshRecNum)
{
	u32		FlshAddr;
	BSP_FlashWaitEvent();
    
	FlshAddr = GetRecFlashAddr( FlshRecNum );
	
	u8 ret = ReadFlsh(FlshAddr,(INT08U *)sFlshRec,sizeof(stcFlshRec));
	
	BSP_FlashSendEvent();
    
	return	ret;
}

/*******************************************************************************
* ��    �ƣ� ReadFlshRec
* ��    �ܣ� ���ݼ�¼�ţ�ȡ���ݼ�¼
* ��ڲ����� ��
* ���ڲ����� ��
* ��    �ߣ� redmornigcn
* �������ڣ� 2017-05-15
* ��    �ģ�
* �޸����ڣ�
*******************************************************************************/
u8	WriteFlshRec(u32 FlshAddr,stcFlshRec *sFlshRec)
{
	BSP_FlashWaitEvent();
    
	u8 ret = WriteFlsh(FlshAddr,(INT08U *)sFlshRec, sizeof(stcFlshRec));
	
	BSP_FlashSendEvent();
    
	return	ret;
}
/*******************************************************************************
* ��    �ƣ�  TaskInitStore
* ��    �ܣ�  �����ʼ��
* ��ڲ����� 	��
* ���ڲ����� 	��
* ��    �ߣ� 	redmornigcn
* �������ڣ� 	2017-05-15
* ��    �ģ�
* �޸����ڣ�
*******************************************************************************/
void    BSP_StoreInit(void)
{    


}


/*******************************************************************************
 * ��    �ƣ� ����ĸ��д
 * ��    �ܣ� 
 * ��ڲ����� ��
 * ���ڲ����� ��
 * ��    �ߣ� ������
 * �������ڣ� 2017/12/20
 * ��    �ģ� 
 * �޸����ڣ� 
 * ��    ע�� 
 *******************************************************************************/
void App_ParaSave(u8 type)
{
//    switch(type) {
//    case 0:
//        /***************************************************
//        * ��������¼�Ź����ַ����ʼ��ַ = 000
//        */    
//        FRAM_StoreHeadInfo((StrRecHeadInfo  *)&Ctrl.sHeadInfo);
//    case 1:
//        /***************************************************
//        * ��������¼�Ź����ַ����ʼ��ַ = 016
//        */    
//        FRAM_StoreRecNumMgr((StrRecNumMgr  *)&Ctrl.sRecNumMgr);
//        break;
//    case 2:
//        /***************************************************
//        * ��������Ʒ��Ϣ����ʼ��ַ = 032
//        */
//        FRAM_StoreProductInfo((StrProductInfo  *)&Ctrl.sProductInfo);
//        break;
//    case 3:
//        /***************************************************
//        * ������ϵͳ���в�������ʼ��ַ = 064
//        */
//        FRAM_StoreRunPara((stcRunPara  *)&Ctrl.sRunPara); 
//        break;
//    case 4:
//        /***************************************************
//        * ���������������������ʼ��ַ = 096
//        */
//        FRAM_StoreOilPara((StrOilPara  *)&Ctrl.sOilPara); 
//        GetOilPara();
//        break;
//    case 5:
//        /***************************************************
//        * ����������������ģ�ͣ���ʼ��ַ = 128
//        */ 
//        FRAM_StoreCalcModel((stcCalcModel  *)&Ctrl.sCalcModel); 
//        break;
//    case 6:
//        /***************************************************
//        * ���������ݼ�¼����ʼ��ַ = 536����������һ��
//        */
//        FRAM_StoreCurRecord((stcFlshRec  *)&Ctrl.sRecB); 
//        break;
//    case 7:
//        break;
//    case 8:
//        /***************************************************
//        * ����������ģ��1��������ʼ��ַ = 792
//        */
//        MBM_FC16_HoldingRegWrN((MODBUS_CH   *)Ctrl.Mtr.pch,
//                               (CPU_INT08U   )1,
//                               (CPU_INT16U   )0,
//                               (CPU_INT16U  *)&Ctrl.MtrPara[0],
//                               (CPU_INT16U   )60);
//        break;
//    case 9:
//        /***************************************************
//        * ����������ģ��2��������ʼ��ַ = 1048
//        */
//        MBM_FC16_HoldingRegWrN((MODBUS_CH   *)Ctrl.Mtr.pch,
//                               (CPU_INT08U   )2,
//                               (CPU_INT16U   )0,
//                               (CPU_INT16U  *)&Ctrl.MtrPara[1],
//                               (CPU_INT16U   )60);
//        break;
//    default:
//        FRAM_StoreHeadInfo((StrRecHeadInfo  *)&Ctrl.sHeadInfo);
//        FRAM_StoreRecNumMgr((StrRecNumMgr  *)&Ctrl.sRecNumMgr); 
//        FRAM_StoreProductInfo((StrProductInfo  *)&Ctrl.sProductInfo);
//        FRAM_StoreRunPara((stcRunPara  *)&Ctrl.sRunPara); 
//        FRAM_StoreOilPara((StrOilPara  *)&Ctrl.sOilPara); 
//        FRAM_StoreCalcModel((stcCalcModel  *)&Ctrl.sCalcModel); 
//        FRAM_StoreCurRecord((stcFlshRecNdp02B  *)&Ctrl.sRecB);
//        break;
//    }
}


/*******************************************************************************
* ��    �ƣ� 		AppTaskStore
* ��    �ܣ� 		��������
* ��ڲ����� 	p_arg - �����񴴽���������
* ���ڲ����� 	��
* ��    �ߣ� 	redmorningcn.
* �������ڣ� 	2017-05-15
* ��    �ģ�
* �޸����ڣ�
*******************************************************************************/
osalEvt  TaskStoreEvtProcess(osalTid task_id, osalEvt task_event)
{
    /***********************************************
    * ������ �������Ź���־��λ
    */
    //OSSetWdtFlag(( OS_FLAGS     ) WDT_FLAG_STORE);
    
    /*******************************************************************************
    * Description  : �������ݼ�¼����Ctrl.sRunPara.StoreTimeʱ�����������ݱ��棩
    * Author       : 2018/5/15 ���ڶ�, by redmorningcn
    *******************************************************************************/
//    if( task_event & OS_EVT_STORE_TICKS ) {
//        osal_start_timerRl( OS_TASK_ID_STORE,
//                            OS_EVT_STORE_TICKS,
//                            OS_TICKS_PER_SEC * Ctrl.sRunPara.StoreTime);
//        /***************************************************
//        * ������ ��λ�������ݱ�־λ���������ݱ���
//        */
//        App_SaveDataToHistory();
//        
//        return ( task_event ^ OS_EVT_STORE_TICKS );
//    }
    
    /*******************************************************************************
    * Description  : �������������ʶλ���档��ʶλ1�������Ӧ������
    * Author       : 2018/5/15 ���ڶ�, by redmorningcn
    *******************************************************************************/
//    if( task_event & OS_EVT_STORE_INIT ) {
//        /***************************************************
//        * ������ �������
//        */
//        for ( u8 i = 0; i < MAX_STORE_TYPE; i++ ) {
//            //���ҵ�ǰ��������Ҫ����
//            if ( SaveType[i] == TRUE ) {
//                //����ǰ������־����
//                SaveType[i] = FALSE;
//                //�������
//                App_ParaSave(i);
//                //�Լ���ʱ���ٲ�ѯ�Ƿ�������������Ҫ���棬���û���򲻻��ٽ���˴�
//                osal_start_timerEx( OS_TASK_ID_STORE,
//                                    OS_EVT_STORE_INIT,
//                                    100);
//                break;
//            }
//        }
//        
//        return ( task_event ^ OS_EVT_STORE_INIT );
//    }
    return 0;
}

/*******************************************************************************
* ��    �ƣ� TaskInitStore
* ��    �ܣ� �����ʼ��
* ��ڲ����� ��
* ���ڲ����� ��
* ��    �ߣ� redmornigcn
* �������ڣ� 2017-05-15
* ��    �ģ�
* �޸����ڣ�
*******************************************************************************/
void TaskInitStore(void)
{    
    /***********************************************
    * ������ ��ʼ��Flash�ײ���غ���
    */
//    BSP_StoreInit();
//    
//    /***********************************************
//    * ������ �ڿ��Ź���־��ע�᱾����Ŀ��Ź���־
//    */
//    OSRegWdtFlag(( OS_FLAGS     )WDT_FLAG_STORE );
//    
//    /***********************************************
//    * ������ 2017/12/3,���������ô洢����Ϊ60��
//    */
//    osal_start_timerRl( OS_TASK_ID_STORE,
//                        OS_EVT_STORE_TICKS,
//                        OS_TICKS_PER_SEC * 60);
}

/*******************************************************************************
* 				                    end of file                                *
*******************************************************************************/
#endif
