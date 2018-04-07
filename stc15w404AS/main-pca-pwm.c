
/*------------------------------------------------------------------*/
/* --- STC MCU International Limited -------------------------------*/
/* --- STC 1T Series MCU RC Demo -----------------------------------*/
/* --- Mobile: (86)13922805190 -------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ---------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ---------------------*/
/* --- Web: www.GXWMCU.com -----------------------------------------*/
/* --- QQ:  800003751 ----------------------------------------------*/
/* If you want to use the program or the program referenced in the  */
/* article, please specify in which data and procedures from STC    */
/*------------------------------------------------------------------*/


#include	"config.h"
#include	"PCA.h"
#include	"delay.h"
#include	"spi.h"

/*************	功能说明	**************

输出3路9~16位变化的PWM信号。类似"呼吸灯"的驱动.

PWM频率 = MAIN_Fosc / PWM_DUTY, 假设 MAIN_Fosc = 24MHZ, PWM_DUTY = 6000, 则输出PWM频率为4000HZ.

******************************************/

/*************	本地常量声明	**************/


/*************	本地变量声明	**************/

u16	pwm0,pwm1,pwm2;
int i=0;
u8 time=5;

/*************	本地函数声明	**************/



/*************  外部函数和变量声明 *****************/


void	PCA_config(void)
{
	PCA_InitTypeDef		PCA_InitStructure;

	PCA_InitStructure.PCA_Clock    = PCA_Clock_12T;		//PCA_Clock_1T, PCA_Clock_2T, PCA_Clock_4T, PCA_Clock_6T, PCA_Clock_8T, PCA_Clock_12T, PCA_Clock_Timer0_OF, PCA_Clock_ECI
	PCA_InitStructure.PCA_IoUse    = PCA_P12_P11_P10_P37;	//PCA_P12_P11_P10_P37, PCA_P34_P35_P36_P37, PCA_P24_P25_P26_P27
	PCA_InitStructure.PCA_Interrupt_Mode = DISABLE;		//ENABLE, DISABLE
	PCA_InitStructure.PCA_Polity   = PolityHigh;			//优先级设置	PolityHigh,PolityLow
	PCA_InitStructure.PCA_RUN      = DISABLE;			//ENABLE, DISABLE
	PCA_Init(PCA_Counter,&PCA_InitStructure);

	PCA_InitStructure.PCA_Mode     = PCA_Mode_HighPulseOutput;		//PCA_Mode_PWM, PCA_Mode_Capture, PCA_Mode_SoftTimer, PCA_Mode_HighPulseOutput
	PCA_InitStructure.PCA_PWM_Wide = 0;					//PCA_PWM_8bit, PCA_PWM_7bit, PCA_PWM_6bit
	PCA_InitStructure.PCA_Interrupt_Mode = ENABLE;		//PCA_Rise_Active, PCA_Fall_Active, ENABLE, DISABLE
	PCA_InitStructure.PCA_Value    = 65535;			//对于软件定时, 为匹配比较值
	PCA_Init(PCA0,&PCA_InitStructure);

	PCA_InitStructure.PCA_Mode     = PCA_Mode_HighPulseOutput;		//PCA_Mode_PWM, PCA_Mode_Capture, PCA_Mode_SoftTimer, PCA_Mode_HighPulseOutput
	PCA_InitStructure.PCA_PWM_Wide = 0;					//PCA_PWM_8bit, PCA_PWM_7bit, PCA_PWM_6bit
	PCA_InitStructure.PCA_Interrupt_Mode = ENABLE;		//PCA_Rise_Active, PCA_Fall_Active, ENABLE, DISABLE
	PCA_InitStructure.PCA_Value    = 65535;				//对于软件定时, 为匹配比较值
	PCA_Init(PCA1,&PCA_InitStructure);

	PCA_InitStructure.PCA_Mode     = PCA_Mode_HighPulseOutput;		//PCA_Mode_PWM, PCA_Mode_Capture, PCA_Mode_SoftTimer, PCA_Mode_HighPulseOutput
	PCA_InitStructure.PCA_PWM_Wide = 0;					//PCA_PWM_8bit, PCA_PWM_7bit, PCA_PWM_6bit
	PCA_InitStructure.PCA_Interrupt_Mode = ENABLE;		//PCA_Rise_Active, PCA_Fall_Active, ENABLE, DISABLE
	PCA_InitStructure.PCA_Value    = 65535;				//对于软件定时, 为匹配比较值
	PCA_Init(PCA2,&PCA_InitStructure);
}

 void	SPI_config(void)
{
	SPI_InitTypeDef		SPI_InitStructure;
	SPI_InitStructure.SPI_Module    = ENABLE;              //SPI启动    ENABLE, DISABLE
	SPI_InitStructure.SPI_SSIG      = DISABLE;			  //片选位     ENABLE, DISABLE
	SPI_InitStructure.SPI_FirstBit  = SPI_MSB;			  //移位方向   SPI_MSB, SPI_LSB
	SPI_InitStructure.SPI_Mode      = SPI_Mode_Slave;	  //主从选择   SPI_Mode_Master, SPI_Mode_Slave
	SPI_InitStructure.SPI_CPOL      = SPI_CPOL_High;      //时钟相位   SPI_CPOL_High,   SPI_CPOL_Low
	SPI_InitStructure.SPI_CPHA      = SPI_CPHA_1Edge;	  //数据边沿   SPI_CPHA_1Edge,  SPI_CPHA_2Edge
	SPI_InitStructure.SPI_Interrupt = ENABLE;			  //中断允许   ENABLE,DISABLE
	SPI_InitStructure.SPI_Speed     = SPI_Speed_16;		  //SPI速度    SPI_Speed_4, SPI_Speed_16, SPI_Speed_64, SPI_Speed_128
	SPI_InitStructure.SPI_IoUse     = SPI_P12_P13_P14_P15; //IO口切换   SPI_P12_P13_P14_P15, SPI_P24_P23_P22_P21, SPI_P54_P40_P41_P43
	SPI_Init(&SPI_InitStructure);
	
	SPI_TxRxMode = SPI_Mode_Slave;
}

/******************** task A **************************/
void main(void)
{
//	P2M1 &= ~(0xe0);	//P2.7 P2.6 P2.5 设置为推挽输出
//	P2M0 |=  (0xe0);
	
    P1M1 &= ~(0x03);
	P1M0 |=  (0x03);

	P3M1 &= ~(0x80);
	P3M0 |=  (0x80);

	PCA_config();
	SPI_config();

	
	pwm0 = 1500;	//给PWM一个初值	总值20000——20ms
	pwm1 = 2500;
	pwm2 = 500;

	PWMn_Update(PCA0,pwm0);
	PWMn_Update(PCA1,pwm1);
	PWMn_Update(PCA2,pwm2);

	EA = 1;
	
	SPI_WriteToTxBuf(0x55);
	SPI_WriteToTxBuf(0x55);
	SPI_WriteToTxBuf(0x55);
	SPI_WriteToTxBuf(0x55);
	SPI_WriteToTxBuf(0x55);
	while (1)
	{	
	  if(SPI_RxTimerOut > 0)
	  {
	   SPI_RxTimerOut=0;
	   SPI_WriteToTxBuf(0x55);
	   i=SPI_RxCnt-1;
	   
	   for(time=5;time>0;time--)
         { 
		   if (!SPI_RxBuffer[i]) 
		   {
		     i=i-4;
		     if(i<0) i=i+SPI_BUF_LENTH;
		     if(SPI_RxBuffer[i]==0xff)
		      {
		     	pwm0 = SPI_RxBuffer[++i]*10;	
				pwm1 = SPI_RxBuffer[++i]*10;
				pwm2 = SPI_RxBuffer[++i]*10;
				PWMn_Update(PCA0,pwm0);
				PWMn_Update(PCA1,pwm1);
				PWMn_Update(PCA2,pwm2);
		      }
			  break;
		   }
		   
		   else if(i==0) i=SPI_BUF_LENTH;
		   else   i--;

		  }
		  }
	      

//		delay_ms(2);
//
//		if(++pwm0 >= PWM_HIGH_MAX)	pwm0 = PWM_HIGH_MIN;
//		PWMn_Update(PCA0,pwm0);
//
//		if(++pwm1 >= PWM_HIGH_MAX)	pwm1 = PWM_HIGH_MIN;
//		PWMn_Update(PCA1,pwm1);
//
//		if(++pwm2 >= PWM_HIGH_MAX)	pwm2 = PWM_HIGH_MIN;
//		PWMn_Update(PCA2,pwm2);
	}
}



