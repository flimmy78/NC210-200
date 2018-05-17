/*******************************************************************************
* Description  : ���弰��ʼ��ȫ�ֱ���
* Author       : 2018/5/10 ������, by redmorningcn
*******************************************************************************/

#include <app_type.h>
#include <tasks.h>

/*******************************************************************************
* Description  : ����ȫ�ֱ���  sCtrl
* Author       : 2018/5/10 ������, by redmorningcn
*******************************************************************************/
volatile    Unnctrl     Ctrl;              //
    
/*******************************************************************************
* Description  : ���߷���ģ�����ָ��
* Author       : 2018/5/14 ����һ, by redmorningcn
*******************************************************************************/
volatile  StrCOMCtrl  * DtuCom;

/*******************************************************************************
* Description  : �ͼ��崮�ڿ���ָ��
* Author       : 2018/5/14 ����һ, by redmorningcn
*******************************************************************************/
volatile  StrCOMCtrl  * MtrCom;

/*******************************************************************************
* Description  : �ͼ��崮�ڿ���ָ��
* Author       : 2018/5/14 ����һ, by redmorningcn
*******************************************************************************/
volatile  StrCOMCtrl  * TaxCom;


/*******************************************************************************
 * ��    �ƣ� app_init_sctrl
 * ��    �ܣ� 
 * ��ڲ����� ��
 * ���ڲ����� ��
 * ��    �ߣ� ������
 * �������ڣ� 2017/12/27
 * ��    �ģ� 
 * �޸����ڣ� 
 * ��    ע�� 
 *******************************************************************************/
void app_init_sctrl(void)
{ 
    /***********************************************
    * ������ 2017/12/23,�����򣺴�1ͨ�ų�ʼ��
    */
//    //����װ��1 ��ʼ��    
//    Ctrl.Mtr.ConnCtrl[0].ConnFlg            = 1;
//    Ctrl.Mtr.ConnCtrl[0].ErrFlg             = 0;
//    Ctrl.Mtr.ConnCtrl[0].MasterAddr         = 0x80;
//    Ctrl.Mtr.ConnCtrl[0].SlaveAddr          = SLAVE_ADDR_DIP1;
//    Ctrl.Mtr.ConnCtrl[0].SendFlg            = 0;
//    Ctrl.Mtr.ConnCtrl[0].SendFramNum        = 1;
//    Ctrl.Mtr.ConnCtrl[0].TimeOut            = 10;
//    Ctrl.Mtr.ConnCtrl[0].Baud               = 9600;
//    Ctrl.Mtr.ConnCtrl[0].Bits               = UART_DATABIT_8;
//    Ctrl.Mtr.ConnCtrl[0].Parity             = UART_PARITY_NONE;
//    Ctrl.Mtr.ConnCtrl[0].Stops              = UART_STOPBIT_1;
//    

    /***********************************************
    * ������ 2017/12/20,�����򣺶�Ctrl
    */

//    WdtReset();


    /*****************************************************************
    * Description  : ��FRAM�зֱ����head��NumMgr��Porduct��RunPara
    * Author       : 2018/5/17 ������, by redmorningcn
    */
    Ctrl.sRunPara.FramFlg.RdHead    = 1;
    Ctrl.sRunPara.FramFlg.RdNumMgr  = 1;
    Ctrl.sRunPara.FramFlg.RdProduct = 1;
    Ctrl.sRunPara.FramFlg.RdRunPara = 1;
    App_FramPara();
    
    //ͨѶ����
    Ctrl.sHeadInfo.Password = 6237;

    //����汾YYMMX
    Ctrl.sProductInfo.SwVer         = SOFT_VERSION;
    /***********************************************
    * ������ 2017/12/27,������ ����Ʒ���
    */
    //��ʼ��ʾ 
    Ctrl.sRunPara.SysSts.StartFlg              = 1;
    
    if ( ( Ctrl.sRunPara.StoreTime < 5 ) || 
         ( Ctrl.sRunPara.StoreTime > 10*60 ) ) {
        Ctrl.sRunPara.StoreTime    = 60;
    }
    
    /***********************************************
    * ������ 2017/12/20,������
    */
    Ctrl.sRunPara.StoreType         = 0x00;
    //������ϴ���
    Ctrl.sRunPara.Err.Error         = 0;
}
