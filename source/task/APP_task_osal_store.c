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


#if (UCOS_EN     == DEF_ENABLED)

#if OS_VERSION > 30000U
static  OS_SEM			Bsp_FlashSem;    	//�ź���
#else
static  OS_EVENT      *Bsp_FlashSem;       //�ź���
#endif

/*******************************************************************************
* ��    �ƣ� BSP_FlashWaitEvent
* ��    �ܣ� �ȴ��ź���
* ��ڲ����� ��
* ���ڲ����� 0���������󣩣�1�������ɹ���
* ��    �ߣ� redmorningcn
* �������ڣ� 2017-05-15
* ��    �ģ�
* �޸����ڣ�
* ��    ע�� ����ʹ��UCOS����ϵͳʱʹ��
*******************************************************************************/
u8 BSP_FlashOsInit(void)
{    
    /***********************************************
    * ������ OS�ӿ�
    */
#if (UCOS_EN     == DEF_ENABLED)
#if OS_VERSION > 30000U
    BSP_OS_SemCreate(&Bsp_FlashSem,1, "Bsp_FlashSem");      // �����ź���
#else
    Bsp_FramSem     = OSSemCreate(1);                       // �����ź���
#endif
#endif 
    return TRUE;
}

/*******************************************************************************
* ��    �ƣ� BSP_FlashWaitEvent
* ��    �ܣ� �ȴ��ź���
* ��ڲ����� ��
* ���ڲ����� 0���������󣩣�1�������ɹ���
* ��    �ߣ� redmorningcn
* �������ڣ� 2017-05-15
* ��    �ģ�
* �޸����ڣ�
* ��    ע�� ����ʹ��UCOS����ϵͳʱʹ��
*******************************************************************************/
static u8 BSP_FlashWaitEvent(void)
{
    /***********************************************
    * ������ OS�ӿ�
    */
    return BSP_OS_SemWait(&Bsp_FlashSem,0); 
}
/*******************************************************************************
* ��    �ƣ� FRAM_SendEvent
* ��    �ܣ� �ͷ��ź���
* ��ڲ����� ��
* ���ڲ����� ��
* ��    �ߣ� redmorningcn
* �������ڣ� 2017-05-15
* ��    �ģ�
* �޸����ڣ�
* ��    ע�� ����ʹ��UCOS����ϵͳʱʹ��
*******************************************************************************/
static void BSP_FlashSendEvent(void)
{
    BSP_OS_SemPost(&Bsp_FlashSem);                        	// �����ź���
}
#endif
/*******************************************************************************/

u8	    WriteFlshRec(u32 FlshAddr,stcFlshRec *sFlshRec);
void	ReadFlshRec(stcFlshRec * sFlshRec,u32 FlshRecNum);

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

stcFlshRec  sRectmp;        //���ݼ�¼����ʱ�洢
/*******************************************************************************
* ��    �ƣ�RoadNum;        		StoreData
* ��    �ܣ�RelRoadNum;     		���ݴ洢���������ݼ�¼�Ž����ݴ洢��ָ��
StationNum;    		flash��ַ;���¼�¼�ŵ���Ϣ���󲿷����ݼ�¼��
E_StationNum;  		��������������ֱ�Ӷ�sCtrl.sRec �и��¡��ٲ������������ڸú����и��¡�
* ��ڲ�����SignalTyp;      	��
* ���ڲ�����LocoSign;       	��
* ��    ��        �� 	redmornigcn
* �������ڣ�LocoWorkState;  	2017-05-15
* ��    �ģ�LocoState;     
* �޸����ڣ�
*******************************************************************************/
u8 App_SaveRecord(void)
{	
    u32     addr ;	
    u16     CRC_sum1;
    u16     CRC_sum2;
    u16     retrys;
    u8      ret;
    /*******************************************************************************
    * Description  : ����flash��ַ��ѭ���洢
    * Author       : 2018/5/16 ������, by redmorningcn
    */
    addr = GetRecFlashAddr(Ctrl.sRecNumMgr.Current);

    retrys = 4096/128;                                              //�����洢��¼����
    
    do {
        // ���ݴ洢��flash
        ret = WriteFlshRec(addr, (stcFlshRec *)&Ctrl.Rec);
        // ��FLASH�ж�ȡ�������жԱ�
        ReadFlshRec((stcFlshRec *)&sRectmp, Ctrl.sRecNumMgr.Current);
        /**************************************************
        * ������ ����У��
        */
        CRC_sum1 = GetCrc16Chk((u8*)&sRectmp,sizeof(stcFlshRec)-2);
        CRC_sum2 = sRectmp.CrcCheck; 
        if(CRC_sum1 == CRC_sum2) {                                  //���ݼ�¼��ȷ,��¼�������
            Ctrl.sRecNumMgr.Current++;          
            Ctrl.sRecNumMgr.Record++;
            Ctrl.Rec.RecordId = Ctrl.sRecNumMgr.Record;
            
            Ctrl.sRunPara.FramFlg.WrNumMgr = 1;         
            osal_set_event( OS_TASK_ID_STORE, OS_EVT_STORE_FRAM);   //֪ͨ�����FRAM
        } 
        else
        {
            Ctrl.sRecNumMgr.Current++;                              //����ַ�洢     
        }
    } while ( --retrys );
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
void	ReadFlshRec(stcFlshRec * sFlshRec,u32 FlshRecNum)
{
	u32		FlshAddr;
	BSP_FlashWaitEvent();
    
	FlshAddr = GetRecFlashAddr( FlshRecNum );
	
	ReadFlsh(FlshAddr,( INT08U * )sFlshRec,sizeof( stcFlshRec ));
	
	BSP_FlashSendEvent();
    
	return	;
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
*/
void    BSP_StoreInit(void)
{    
    SPI_FLASH_Init();           //��ʼ��mx25 spi�ӿ�
}

/*******************************************************************************
* Description  : �������
* Author       : 2018/5/16 ������, by redmorningcn
*/
void App_FramPara(void)
{
    int     add;
    
    if(Ctrl.sRunPara.FramFlg.Flags)                                        
    {
        /*******************************************************************************
        * Description  : ��HEAD
        * Author       : 2018/5/16 ������, by redmorningcn
        */
        if(Ctrl.sRunPara.FramFlg.WrHead == 1)                               
        {
            Ctrl.sRunPara.FramFlg.WrHead = 0;
            add = (int)&Ctrl.sHeadInfo - (int)&Ctrl;                                
            
            WriteFM24CL64(add,(u8 *)&Ctrl.sHeadInfo,sizeof(Ctrl.sHeadInfo));  
        }
        /*******************************************************************************
        * Description  : ��HEAD
        * Author       : 2018/5/16 ������, by redmorningcn
        */
        if(Ctrl.sRunPara.FramFlg.RdHead == 1)                               
        {
            Ctrl.sRunPara.FramFlg.RdHead = 0;
            add = (int)&Ctrl.sHeadInfo - (int)&Ctrl;                                
            
            ReadFM24CL64(add,(u8 *)&Ctrl.sHeadInfo,sizeof(Ctrl.sHeadInfo));  
        }
        
        /***********************************************************************
        * Description  : ��NumMgr
        * Author       : 2018/5/16 ������, by redmorningcn
        */
        if(Ctrl.sRunPara.FramFlg.WrNumMgr == 1)
        {
            Ctrl.sRunPara.FramFlg.WrNumMgr = 0;
            add = (int)&Ctrl.sRecNumMgr - (int)&Ctrl;
            
            WriteFM24CL64(add,(u8 *)&Ctrl.sRecNumMgr,sizeof(Ctrl.sRecNumMgr));
        }
        /*******************************************************************************
        * Description  : ��NumMgr
        * Author       : 2018/5/16 ������, by redmorningcn
        */
        if(Ctrl.sRunPara.FramFlg.RdNumMgr == 1)
        {
            Ctrl.sRunPara.FramFlg.RdNumMgr = 0;
            add = (int)&Ctrl.sRecNumMgr - (int)&Ctrl;
            
            ReadFM24CL64(add,(u8 *)&Ctrl.sRecNumMgr,sizeof(Ctrl.sRecNumMgr));
        }
        
        /***********************************************************************
        * Description  : ���Ʒ��Ϣ
        * Author       : 2018/5/16 ������, by redmorningcn
        */
        if(Ctrl.sRunPara.FramFlg.WrProduct == 1)
        {
            Ctrl.sRunPara.FramFlg.WrProduct = 0;
            add = (int)&Ctrl.sProductInfo - (int)&Ctrl;
            
            WriteFM24CL64(add,(u8 *)&Ctrl.sProductInfo,sizeof(Ctrl.sProductInfo));
        }
        /***********************************************************************
        * Description  : ����Ʒ��Ϣ
        * Author       : 2018/5/16 ������, by redmorningcn
        */
        if(Ctrl.sRunPara.FramFlg.RdProduct == 1)
        {
            Ctrl.sRunPara.FramFlg.RdProduct = 0;
            add = (int)&Ctrl.sProductInfo - (int)&Ctrl;
            
            ReadFM24CL64(add,(u8 *)&Ctrl.sProductInfo,sizeof(Ctrl.sProductInfo));
        }        
        
        /***********************************************************************
        * Description  : ��������Ϣ
        * Author       : 2018/5/16 ������, by redmorningcn
        */
        if(Ctrl.sRunPara.FramFlg.WrRunPara == 1)
        {
            Ctrl.sRunPara.FramFlg.WrRunPara = 0;
            add = (int)&Ctrl.sProductInfo - (int)&Ctrl;
            
            WriteFM24CL64(add,(u8 *)&Ctrl.sRunPara,sizeof(Ctrl.sRunPara));
        } 
        /***********************************************************************
        * Description  : ��������Ϣ
        * Author       : 2018/5/16 ������, by redmorningcn
        */
        if(Ctrl.sRunPara.FramFlg.RdRunPara == 1)
        {
            Ctrl.sRunPara.FramFlg.RdRunPara = 0;
            add = (int)&Ctrl.sProductInfo - (int)&Ctrl;
            
            ReadFM24CL64(add,(u8 *)&Ctrl.sRunPara,sizeof(Ctrl.sRunPara));
        }             
    }
        
    Ctrl.sRunPara.FramFlg.Flags = 0;               //���ʶ
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
    if( task_event & OS_EVT_STORE_TICKS ) {
        osal_start_timerRl( OS_TASK_ID_STORE,
                            OS_EVT_STORE_TICKS,
                            OS_TICKS_PER_SEC * Ctrl.sRunPara.StoreTime);
        /***************************************************
        * ������ ��λ�������ݱ�־λ���������ݱ���FLASH
        */
        App_SaveRecord();               //�������ݼ�¼
        
        //osal_set_event( OS_TASK_ID_TMR, OS_EVT_TMR_DTU);      //֪ͨDTUģ�鷢������

        return ( task_event ^ OS_EVT_STORE_TICKS );
    }
    
    /*******************************************************************************
    * Description  : �������������ʶλ���档��ʶλ1�������Ӧ������
    * Author       : 2018/5/15 ���ڶ�, by redmorningcn
    *******************************************************************************/
    if( task_event & OS_EVT_STORE_FRAM ) {
        /***************************************************
        * ������ ��������洢���ȡ
        */
        App_FramPara();
        
        return ( task_event ^ OS_EVT_STORE_FRAM );
    }
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
    BSP_StoreInit();
    
//    /***********************************************
//    * ������ �ڿ��Ź���־��ע�᱾����Ŀ��Ź���־
//    */
//    OSRegWdtFlag(( OS_FLAGS     )WDT_FLAG_STORE );
//    
    /***********************************************
    * ������ 2017/12/3,���������ô洢����Ϊ60��
    */
    osal_start_timerRl( OS_TASK_ID_STORE,
                        OS_EVT_STORE_TICKS,
                        OS_TICKS_PER_SEC * 60);
}

/*******************************************************************************
* 				                    end of file                                *
*******************************************************************************/
#endif
