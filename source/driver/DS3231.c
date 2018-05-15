/*******************************************************************************
* Description  : DS3231����
* Author       : 2018/5/15 ���ڶ�, by redmorningcn
*******************************************************************************/

/*******************************************************************************
 * INCLUDES
 */
#include <includes.h>
#include <ds3231.h>
#include <bsp_time.h>
#include <bsp_IIC.h>

     
/*******************************************************************************
 * CONSTANTS
 */
#define DS3231_ADDR     		    0xd0
#define DS3231_SEC      		    0x00
#define DS3231_MIN      		    0x01
#define DS3231_HOUR     		    0x02
#define DS3231_DAY      		    0x03
#define DS3231_DATA     		    0x04
#define DS3231_MONTH    		    0x05
#define DS3231_YEAR     		    0x06

#define DS3231_CONTROL      	    0x0E
#define DS3231_STATUS       	    0x0F
#define DS3231_AGING_OFFSET         0x10
#define DS3231_TEMP_MSB     	    0x11
#define DS3231_TEMP_LSB     	    0x12

/*******************************************************************************
 * TYPEDEFS
 */
 
/*******************************************************************************
 * LOCOAL VARIABLES
 */
/***********************************************
* ����: OS�ӿ�
*/


/*******************************************************************************
 * LOCOAL FUNCTIONS
 */
 
/*******************************************************************************
 * GLOBAL VARIABLES
 */

/*******************************************************************************
 * GLOBAL FUNCTIONS
 */

/*******************************************************************************
 * EXTERN VARIABLES
 */
 
/*******************************************************************************
 * EXTERN FUNCTIONS
 */
 

/*******************************************************************************
* ��    �ƣ� bcd2Hex
* ��    �ܣ� ��BCD���Ϊ��������
* ��ڲ����� BCD������
* ���ڲ����� ����������
* �������ߣ� ������
* �������ڣ� 2009-01-08
* ��    �ģ�
* �޸����ڣ�
*******************************************************************************/
BYTE Bcd2Hex(BYTE val)
{
    return ((val&0x0f)+(val >> 4)*10);
}

/*******************************************************************************
* ��    �ƣ� hex2Bcd
* ��    �ܣ� 
* ��ڲ�����
* ���ڲ����� ��
* �������ߣ� ������
* �������ڣ� 2009-01-08
* ��    �ģ�
* �޸����ڣ�
*******************************************************************************/
BYTE Hex2Bcd(BYTE val)
{
    return ((val / 10) << 4) + val % 10;
}

/*******************************************************************************
 * ��    �ƣ� ReadDS3231Byte
 * ��    �ܣ� ��DS3231�ж���һ���ֽ�
 * ��ڲ����� DS3231�豸�ţ� ��ȡ���ݵĵ�ַ
 * ���ڲ����� ���ض�����ֵ
 * ��    �ߣ� ������
 * �������ڣ� 2018/1/1
 * ��    �ģ� 
 * �޸����ڣ� 
 * ��    ע�� 
 *******************************************************************************/
uint8 ReadDS3231Byte(uint8 addr)  
{
    uint8   SlaveAddr;
    uint8   RecData;
    uint8   ack = 1;
    
    SlaveAddr = DS3231_ADDR & (~(0x01));            //�ӻ���ַ
    
    StartI2C();                                     //����IIC����
    
    WriteByteWithI2C(SlaveAddr);                    //дDS3231��ȡ
    
    ack = CheckSlaveAckI2C();                       //���ӻ�Ӧ���ź�
    if ( ack )
        goto exit;
    
    WriteByteWithI2C(addr);                         //дDS3231��ȡ
    
    ack = CheckSlaveAckI2C();                       //���ӻ�Ӧ���ź�
    if ( ack )
        goto exit;
    
    StartI2C();                                     //����IIC����
    
    SlaveAddr = DS3231_ADDR | 0x01;             	//�ӻ���ַ
    
    WriteByteWithI2C(SlaveAddr);                    //дDS3231�豸��ַ
    
    ack = CheckSlaveAckI2C();                       //���ӻ�Ӧ���ź�
    if ( ack )
        goto exit;
    
    RecData = ReadByteWithI2C();                	//���յ�������
    
    MasterNoAckI2C();                               //����Ӧ��ӻ�
exit:
    StopI2C();                          			//����IIC����
    
    return  RecData;
}

/*******************************************************************************
 * ��    �ƣ� WriteDS3231Byte
 * ��    �ܣ� дһ���ֽڵ�DS3231��
 * ��ڲ����� addr:DS3231��ַ�� Data:Ҫд�������
 * ���ڲ����� ��
 * ��    �ߣ� ������
 * �������ڣ� 2018/1/1
 * ��    �ģ� 
 * �޸����ڣ� 
 * ��    ע�� 
 *******************************************************************************/
void WriteDS3231Byte(uint8 Addr,uint8   Data)  
{
    uint8   SlaveAddr;
    uint8   ack = 1;
    
    StartI2C();                                     //����IIC����
    
    SlaveAddr = DS3231_ADDR & (~(0x01));            //�ӻ���ַ
    
    WriteByteWithI2C(SlaveAddr);                    //дDS3231��ȡ
    
    ack = CheckSlaveAckI2C();                       //���ӻ�Ӧ���ź�(��ΪӦ��)
    if ( ack )
        goto exit;
    
    WriteByteWithI2C(Addr);                         //дDS3231������ַ
    
    ack = CheckSlaveAckI2C();                       //���ӻ�Ӧ���ź�
    if ( ack )
        goto exit;
    
    WriteByteWithI2C(Data);                         //дDS3231����
    
    ack = CheckSlaveAckI2C();                       //���ӻ�Ӧ���ź�
    if ( ack )
        goto exit;
    
exit:
    StopI2C();                          			//����IIC����
}      

/*******************************************************************************
* Description  : ���ڲ��¶�ֵ������ֵ��
* Author       : 2018/5/15 ���ڶ�, by redmorningcn
*******************************************************************************/
s8  BSP_RTC_ReadTemp(void)  
{
    s8          TempMsb;

    FRAM_WaitEvent();
    
    TempMsb     = ReadDS3231Byte(DS3231_TEMP_MSB);      //����λ
    
    FRAM_SendEvent();
    
    return  TempMsb;
}

/*******************************************************************************
 * ��    �ƣ� ReadTime
 * ��    �ܣ� ��DS3231�ж���ʱ��
 * ��ڲ����� ���Ƕ�������ʱ������
 * ���ڲ����� ���ض�����ʱ��
 * ��    �ߣ� ������
 * �������ڣ� 2018/1/1
 * ��    �ģ� 
 * �޸����ڣ� 
 * ��    ע�� 
 *******************************************************************************/
uint8 BSP_RTC_ReadTime(stcTime  *sTime)  
{    
    FRAM_WaitEvent();
    
    sTime->Sec      = Bcd2Hex(ReadDS3231Byte(DS3231_SEC));
    sTime->Min      = Bcd2Hex(ReadDS3231Byte(DS3231_MIN));
    sTime->Hour     = Bcd2Hex(ReadDS3231Byte(DS3231_HOUR));
    sTime->Day      = Bcd2Hex(ReadDS3231Byte(DS3231_DATA));
    sTime->Mon      = Bcd2Hex(ReadDS3231Byte(DS3231_MONTH)); 
    sTime->Year     = Bcd2Hex(ReadDS3231Byte(DS3231_YEAR));
    
    FRAM_SendEvent();
    
    return  1;
}

/*******************************************************************************
 * ��    �ƣ� WriteTime
 * ��    �ܣ� дDS3231ʱ��
 * ��ڲ����� ��
 * ���ڲ����� ��
 * ��    �ߣ� ������
 * �������ڣ� 2018/1/1
 * ��    �ģ� 
 * �޸����ڣ� 
 * ��    ע�� 
 *******************************************************************************/
void BSP_RTC_WriteTime(stcTime  sTime)  
{
    FRAM_WaitEvent();
    
    WriteDS3231Byte(DS3231_SEC,     Hex2Bcd(sTime.Sec));     
    WriteDS3231Byte(DS3231_MIN,     Hex2Bcd(sTime.Min));        
    WriteDS3231Byte(DS3231_HOUR,    Hex2Bcd(sTime.Hour));
    WriteDS3231Byte(DS3231_DATA,    Hex2Bcd(sTime.Day));    
    WriteDS3231Byte(DS3231_MONTH,   Hex2Bcd(sTime.Mon));  
    WriteDS3231Byte(DS3231_YEAR,    Hex2Bcd(sTime.Year));
    
    FRAM_SendEvent();
}

/*******************************************************************************
 * ��    �ƣ� BSP_Ds3231Init
 * ��    �ܣ� ��ʼ��DS3231�豸ģʽ
 * ��ڲ����� ��
 * ���ڲ����� ��
 * ��    �ߣ� ������
 * �������ڣ� 2018/1/1
 * ��    �ģ� 
 * �޸����ڣ� 
 * ��    ע�� 
 *******************************************************************************/
void BSP_Ds3231Init(void)  
{
    //���߳�ʼ������bsp����ɡ�
    //
    WriteDS3231Byte(DS3231_CONTROL,0); 
    //��ʼ��״̬�Ĵ���
    WriteDS3231Byte(DS3231_STATUS,0);
}

/*******************************************************************************
* ��    �ƣ� BSP_RX8025T_SetDate()
* ��    �ܣ� ��ʼ��RTC�豸
* ��ڲ�����
* ���ڲ�����
* �������ߣ� ������
* �������ڣ� 2015-01-26
* ��    �ģ�
* �޸����ڣ�
*******************************************************************************/
BOOL BSP_RTC_Init(void)  
{
    BSP_Ds3231Init();
    tm_now  = TIME_GetCalendarTime(); 
    return TRUE;
}

/*******************************************************************************
* ��    �ƣ� BSP_RX8025T_SetDate()
* ��    �ܣ� ����ʱ�ڣ��꣬�£�ʱ�����ڣ�ʱ���֣���
* ��ڲ�����
* ���ڲ�����
* �������ߣ� ������
* �������ڣ� 2015-01-26
* ��    �ģ�
* �޸����ڣ�
*******************************************************************************/
BOOL BSP_SetTime(struct tm t_tm)
{
    INT08U  time[7];
    INT08U  i;
    
    FRAM_WaitEvent();
    /***********************************************
    * ������ �ж����Ƿ�Ϸ�
    */
    if ( t_tm.tm_year > 2000 )
        t_tm.tm_year -= 2000;
    
    /***********************************************
    * ������ �ȶ�ȡ����
    */        
    time[0]     = Hex2Bcd(t_tm.tm_sec);
    time[1]     = Hex2Bcd(t_tm.tm_min);
    time[2]     = Hex2Bcd(t_tm.tm_hour);
    time[3]     = Hex2Bcd(t_tm.tm_wday);
    time[4]     = Hex2Bcd(t_tm.tm_mday);
    time[5]     = Hex2Bcd(t_tm.tm_mon);
    time[6]     = Hex2Bcd(t_tm.tm_year);
    
    /***********************************************
    * ������ д��ʱ��
    */
    for ( i = 0; i < 7; i++ ) {
        WriteDS3231Byte(i,time[i]);
    }  
    FRAM_SendEvent();
    return TRUE;
}

/*******************************************************************************
* ��    �ƣ� BSP_RX8025T_GetTime()
* ��    �ܣ� ��ȡʱ�䣺�꣬�£��գ����ڣ�ʱ���֣���
* ��ڲ����� *t_tm - ʱ����սṹ��
* ���ڲ�����
* �������ߣ� ������
* �������ڣ� 2015-01-26
* ��    �ģ�
* �޸����ڣ�
*******************************************************************************/
BOOL BSP_GetTime(struct tm *t_tm)
{
    u8 time[7];
    INT08U  i;
    FRAM_WaitEvent();
    
    for ( i = 0; i < 7; i++ ) {
        time[i]    = ReadDS3231Byte(i);
    }
    t_tm->tm_sec     = Bcd2Hex(time[0]);
    t_tm->tm_min     = Bcd2Hex(time[1]);
    t_tm->tm_hour    = Bcd2Hex(time[2]);
    t_tm->tm_wday    =         time[3];
    t_tm->tm_mday    = Bcd2Hex(time[4]);
    t_tm->tm_mon     = Bcd2Hex(time[5]);
    t_tm->tm_year    = Bcd2Hex(time[6]) + 2000;
    
    FRAM_SendEvent();
    
    return TRUE;
}
/*******************************************************************************
 * 				end of file                                                    *
 *******************************************************************************/