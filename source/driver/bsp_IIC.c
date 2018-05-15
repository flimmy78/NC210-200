/*******************************************************************************
* Description  : 模拟IIC总线，IIC时序
* Author       : 2018/5/15 星期二, by redmorningcn
*******************************************************************************/

#include <includes.h>

#define     SCL     	    106		//PB.10
#define     SDA             107		//PB.11

#define Delay_Factor       5                                        		//延时因子

#if (UCOS_EN     == DEF_ENABLED)
/*******************************************************************************
* 名    称： 		FRAM_WaitEvent
* 功    能： 		等待信号量
* 入口参数： 	无
* 出口参数：  	0（操作有误），1（操作成功）
* 作    者： 	redmorningcn
* 创建日期： 	2017-05-15
* 修    改：
* 修改日期：
* 备    注： 仅在使用UCOS操作系统时使用
*******************************************************************************/
u8 FRAM_WaitEvent(void)
{
    /***********************************************
    * 描述： OS接口
    */
    return BSP_OS_SemWait(&Bsp_FramSem,0);           	// 等待信号量
}

/*******************************************************************************
* 名    称：	 	FRAM_SendEvent
* 功    能： 		释放信号量
* 入口参数： 	无
* 出口参数： 	无
* 作    者： 	redmorningcn
* 创建日期： 	2017-05-15
* 修    改：
* 修改日期：
* 备    注： 仅在使用UCOS操作系统时使用
*******************************************************************************/
void FRAM_SendEvent(void)
{
    BSP_OS_SemPost(&Bsp_FramSem);                        	// 发送信号量
}

/*******************************************************************************
* 名    称： FRAM_SendEvent
* 功    能： 释放信号量
* 入口参数： 无
* 出口参数： 无
* 作    者： redmorningcn
* 创建日期： 2017-05-15
* 修    改：
* 修改日期：
* 备    注： 仅在使用UCOS操作系统时使用
*******************************************************************************/
void BSP_FramOsInit(void)
{
    /***********************************************
    * 描述： OS接口
    */
#if (UCOS_EN     == DEF_ENABLED)
#if OS_VERSION > 30000U
    BSP_OS_SemCreate(&Bsp_FramSem,1, "Bsp EepSem");      // 创建信号量
#else
    Bsp_FramSem     = OSSemCreate(1);                    // 创建信号量
#endif
#endif
}

#endif /* end of (UCOS_EN     == DEF_ENABLED)*/


/*******************************************************************************
* Function Name  : I2C_delay
* Description    : 延时程序，i决定延时时间
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
OPTIMIZE_NONE void I2C_delay(void)
{
	u8 i = Delay_Factor; 
	while(i) 
	{ 
		i--; 
	} 
}

//------------------------------------------------------------------------
//  名  称 ：		GPIO_Fram_Init
//  功  能 :          初始化铁电的 IO口，为通用IO 口
// 入口参数：	    无
// 出口参数：	    无
//------------------------------------------------------------------------
void	GPIO_Fram_Init(void)
{
	GPIO_Config(SCL,0);
	GPIO_Config(SCL,0);
	GPIO_Config(SDA,0);
	GPIO_Config(SDA,0);
    
#if (UCOS_EN     == DEF_ENABLED)        //os接口
    BSP_FramOsInit();
#endif
}

//------------------------------------------------------------------------
//  名  称 ：void WriteSCL( uint8 temp )
//  功  能 ：设置 SCL
// 入口参数：无
// 出口参数：无
//------------------------------------------------------------------------
void WriteSCL(uint8 temp)
{	
	if(temp)		
	{
		GPIO_Set(SCL,1);
	}
	else
	{
		GPIO_Set(SCL,0);		
	}	

	I2C_delay();
}

//------------------------------------------------------------------------
//  名  称 ：uint8 ReadSCL(void)
//  功  能 ：读取 SCL 的值
// 入口参数：无
// 出口参数：无
//------------------------------------------------------------------------
uint8 ReadSCL(void)
{
    uint8    temp;
    temp = 	GPIO_Read(SCL);
    I2C_delay();
    return temp;
}

//------------------------------------------------------------------------
//  名  称 ：void WriteSDA( uint8 temp )
//  功  能 ：设置 SDA
// 入口参数：无
// 出口参数：无
//------------------------------------------------------------------------
void WriteSDA(uint8 temp)
{
	if(temp)		
	{
		GPIO_Set(SDA,1);
	}
	else
	{
		GPIO_Set(SDA,0);		
	}	
	I2C_delay();
}

//------------------------------------------------------------------------
//  名  称 ：uint8 ReadSDA(void)
//  功  能 ：读取 SDA 的值
// 入口参数：无
// 出口参数：无
//------------------------------------------------------------------------
uint8 ReadSDA(void)
{
    uint8 temp;
    
    temp = 	GPIO_Read(SDA);
    I2C_delay();

    return temp;
}

//------------------------------------------------------------
//函数名称:         StartI2C()
//功    能:         I2C总线开始条件
//入口参数:         无
//出口参数:         无
//说明：            I2C总线的开始条件：
//                  SCL高电平时SDA的下降沿
//------------------------------------------------------------
void StartI2C(void)
{
    WriteSCL(0);                                // SCL=0;
    WriteSDA(1);                                //SDA=1;
    WriteSCL(1);                                //SCL=1;
    WriteSDA(0);                                //SDA=0;
    WriteSCL(0);                                // SCL=0;
}

//---------------------------------------------------------------
//函数名称:         StopI2C()
//功    能:         I2C总线结束条件
//入口参数:         无
//出口参数:         无
//说明：            I2C总线结束条件：SCL为高电平时SDA的上升沿
//--------------------------------------------------------------------
void StopI2C(void)
{
    WriteSCL(0);                                // SCL=0;
    WriteSDA(0);                                //SDA=0;
    WriteSCL(1);                                //SCL=1;
    WriteSDA(1);                                //SDA=1;
    WriteSCL(0);                                //SCL=0;
}

//-------------------------------------------------------------------------------
//函数名称:         MasterAckI2C()
//功    能:         主机接收应答，在接收到1字节数据后，返回应答，告诉从机接收完成
//入口参数:         无
//出口参数:         无
//说明：            I2C总线应答：读完一个字节后将SDA线拉低，即应答0
//---------------------------------------------------------------------------------
void MasterAckI2C(void)
{
    WriteSCL(0);                                // SCL=0;
    WriteSDA(0);                               //SDA=0;
    WriteSCL(1);                                //SCL=1;
    WriteSCL(0);                                 //SCL=0;
    WriteSDA(0);                                //SDA=0;
}

//-----------------------------------------------------------------------------------
//函数名称:         MasterNoAckI2C()
//功    能:         主机接收应答，告诉通信从机接收完成
//入口参数:         无
//出口参数:         无
//说明：            I2C总线应答：
//                  读完所要求的字节数据后将SDA线拉高，为结束IIC总线操作做准备
//-------------------------------------------------------------------------------------
void MasterNoAckI2C(void)
{
    WriteSCL(0);                                    // SCL=0;
    WriteSDA(1);                                    //SDA=1;
    WriteSCL(1);                                    //SCL=1;
    WriteSCL(0);                                    //SCL=0;
    WriteSDA(0);                                    //SDA=0;
}

//------------------------------------------------------------------------------------
//函数名称:         CheckSlaveAckI2C()
//功    能:         读I2C从机应答
//入口参数:         无
//出口参数:         ackflag
//                  如果从机产生应答，则返回0，
//                  从机没有产生应答,否则返回1
//说明：            检查I2C器件(从机)应答情况,应答返回1,否则为零
//these waw a bug befor 2008/05/17.
//------------------------------------------------------------------------------------
uint8 CheckSlaveAckI2C(void)
{
    uint8  ackflag;                                  //清除应答标志;
    uint16 ctr  = 0;
    
    do {
        ackflag = ReadSDA();
    } while ( ( ++ctr < 5000 ) && ( ackflag ) );
    
    WriteSCL(1);                                    //SCL=1;
    WriteSCL(0);                                    //SCL=0;
    
    return ackflag;
}

//------------------------------------------------------------------------------------
//函数名称:         WriteByteWithI2C()
//功    能:         	主机对I2C总线写1操作
//入口参数:         Data:要写出的数据
//出口参数:         无
//说明：            	写一字节数据Data
//------------------------------------------------------------------------------------
void WriteByteWithI2C(uint8 Data)  
{
    uint8 i;
    for(i=0;i<8;i++)
    {
        if(Data & 0x80)                             //传输位从高位到低位,先传最高位 
        {
            WriteSDA(1);                            //SDA=1;
            WriteSCL(1);                            //SCL=1;
        }
        else
        {  
            WriteSDA(0);                            //SDA=0;
            WriteSCL(1);                            //SCL=1;
        }   
        WriteSCL(0);                                //SCL=0;
        WriteSDA(0);                                //SDA=0;
        Data <<= 1;                                 //数据左移             
    }
}

//-------------------------------------------------------------------------------------------------------
//函数名称:         ReadByteWithI2C()
//功    能:         主机对I2C总线写1操作
//入口参数:         无
//出口参数:         无
//说明：            读一字节数据：返回值即读的数据
//--------------------------------------------------------------------------------------------------------
uint8 ReadByteWithI2C(void)            
{
    uint8 readdata=0;                           // Read_Data为读入的数据，0
    uint8 j;
    
    for (j=0;j<8;j++)
    {
        readdata <<=1;
								  //数据读出，若是0则移位；若是1则与0x01或
        if(ReadSDA())
        {
            readdata =(readdata | 0x01);               
        }
        WriteSCL(1);                              //SCL=1;
        WriteSCL(0);                              //SCL=0;
    }
    
    return   readdata ;                         //返回读出的字节
}


