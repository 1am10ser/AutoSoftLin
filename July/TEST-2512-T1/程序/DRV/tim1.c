#include "common.h"

/**
 * TIM1 PWM 输入捕获基础变量
 * PCB 线路板上 TIM1 捕获编号为 56和57
 * 考虑到不同通道输出/捕获时都同时设置 TIM1_TimeBaseInit 函数，会相互干扰，所以使用 TIM1 就启用一个功能：PWM输出 或 PWM输入捕获
 */
short  GV_Tim1PwmiNumbers[] = {56, 57};  //【按需设置】PWM捕获 编号集，只有将 PWM输出 编号加入到这个变量后才能实现对应 PWM输出 的相关功能

unsigned int GV_Tim1PwmiFreq56 = 0;    //【系统使用】定时器捕获的 PWM 频率
double GV_Tim1PwmiDuty56 = 0.0;  //【系统使用】定时器捕获的 PWM 正占空比

u16    GV_Tim1PwmiFreq57 = 0;    //【系统使用】定时器捕获的 PWM 频率
double GV_Tim1PwmiDuty57 = 0.0;  //【系统使用】定时器捕获的 PWM 正占空比


void TIME1_Init(void)
{ 

  TIM1_DeInit();
 
//  GV_Time1OverflowValue = (((GV_MCUTime / GV_Time1PrescalerValue) * 1000) * GV_MotorOne * 97 / 100) / 360;
//  
//  if(GV_Used1OR3 !=2)
//  {
//      if(GV_MotorOne > 2500)TIM1_TimeBaseInit(GV_Time1PrescalerValue - 1,TIM1_COUNTERMODE_UP,GV_Time1OverflowValue + ((GV_MotorOne - 2500) / 30),0);
//      else TIM1_TimeBaseInit(GV_Time1PrescalerValue - 1,TIM1_COUNTERMODE_UP,GV_Time1OverflowValue - 50,0);
//  }
//  else if(GV_Used1OR3 == 2)
//  {
//      if(GV_MotorOne > 3000)TIM1_TimeBaseInit(GV_Time1PrescalerValue - 1,TIM1_COUNTERMODE_UP,GV_Time1OverflowValue + ((GV_MotorOne - 3000) / 200),0);
//      else TIM1_TimeBaseInit(GV_Time1PrescalerValue - 1,TIM1_COUNTERMODE_UP,GV_Time1OverflowValue - 110,0);
//  }
  TIM1_TimeBaseInit(15,TIM1_COUNTERMODE_UP,65535 - 50000,0);

  TIM1_ARRPreloadConfig(ENABLE);

  TIM1_ITConfig(TIM1_IT_UPDATE,ENABLE);

  TIM1_Cmd(DISABLE);
}




/* -------------------------------------------------- */
// ● 函数主体（PWM 输入捕获）
// ● PCB 线路板上 TIM1 输出编号为 56和57
// ● 考虑到不同通道输出/捕获时都同时设置 TIM1_TimeBaseInit 函数，会相互干扰，所以使用 TIM1 就启用一个功能：PWM输出 或 PWM输入捕获
/* -------------------------------------------------- */

/**
 * 初始化 PWM
 */
//void TIM1_PWMI_Init()
//{
//    // 重置初始化
//    TIM1_DeInit();
//    
//    // 初始化 TIM1 时基单元
//    // 16预分频，向上计数，每 1s/1M = 1us 计数器值加 1（累加 1000 次需要 1 毫秒）
//    // 参数说明：预分配值，计数模式，自动重装载值（计数器值到 65536 后重新装载），计数器起始值（计数器值重新装载后从0开始）
//    TIM1_TimeBaseInit(15, TIM1_COUNTERMODE_UP, 65536-1, 0);
//
//    // 初始化 TIM1 通道
////    for( int i=0; i < ARR_LENGTH(GV_Tim1PwmiNumbers); i++ ){
////        if( GV_Tim1PwmiNumbers[i] == 56 ) TIM1_PWMI_Init56();
////        if( GV_Tim1PwmiNumbers[i] == 57 ) TIM1_PWMI_Init57();
////    }
//
//}
void TIM1_PWMI_Init56()
{
    // PWM的通道1配置
    // PWM的通道2无需配置，因为 TIM1_PWMIConfig(...) 内部已经进行了配置
    //
    // 参数说明：
    //　　1. TIM1 通道1
    //　　2. 极性：上升沿捕获
    //　　3. 输入脚：TIM1_ICSELECTION_DIRECTTI 表示直接将 T1 信号经过 TI1FP1 发送给 IC1 模块进行处理
    //　　4. 预分频：意思是控制在多少个输入周期做一次捕获；【经过实际测试，复位触发模式下该值无效，都是按照1分频执行】
    //       　　　　1M 系统时钟，捕获 20Hz PWM 一个周期需要计数 50000  次（1M/20=50000），TIM1 最大计数有65536，可以捕获
    //       　　　　1M 系统时钟，捕获 15Hz PWM 一个周期需要计数 66666  次（1M/15=66666），TIM1 最大计数有65536，无法捕获
    //　　5. 滤波频率：经历几个周期相同的跳变则波形稳定。如果频率捕获偶发值不对，那么可以启用 0x02（4个事件）
    TIM1_ICInit(TIM1_CHANNEL_1, TIM1_ICPOLARITY_RISING, TIM1_ICSELECTION_DIRECTTI, TIM1_ICPSC_DIV1, 0x02);
    
//    TIM1_SelectInputTrigger(TIM1_TS_TI1FP1);
//    TIM1_SelectSlaveMode(TIM1_SLAVEMODE_RESET);
    
    TIM1_ClearITPendingBit(TIM1_IT_CC1);
    TIM1_ClearFlag(TIM1_FLAG_CC1);


    // 输入捕获/比较输出使能
    //【经过实际测试，以下使能代码可以注释掉，但是为了安心还是加上去】
//    TIM1_CCxCmd(TIM1_CHANNEL_1, ENABLE);
//    TIM1_CCxCmd(TIM1_CHANNEL_2, ENABLE);
}
void TIM1_PWMI_Init57()
{
    // PWM的通道3配置
    //
    // 参数说明：
    //　　1. TIM1 通道3
    //　　2. 极性：上升沿捕获
    //　　3. 输入脚：TIM1_ICSELECTION_DIRECTTI 表示直接将 T3 信号经过 TI3FP3 发送给 IC3 模块进行处理
    //　　4. 预分频：意思是控制在多少个输入周期做一次捕获；【经过实际测试，复位触发模式下该值无效，都是按照1分频执行】
    //       　　　　1M 系统时钟，捕获 20Hz PWM 一个周期需要计数 50000  次（1M/20=50000），TIM1 最大计数有65536，可以捕获
    //       　　　　1M 系统时钟，捕获 15Hz PWM 一个周期需要计数 66666  次（1M/15=66666），TIM1 最大计数有65536，无法捕获
    //　　5. 滤波频率：经历几个周期相同的跳变则波形稳定。如果频率捕获偶发值不对，那么可以启用 0x02（4个事件）
    TIM1_ICInit(TIM1_CHANNEL_3, TIM1_ICPOLARITY_RISING, TIM1_ICSELECTION_DIRECTTI, TIM1_ICPSC_DIV1, 0x02);
    
    // PWM的通道4配置
    //
    // 参数说明：
    //　　1. TIM1 通道4
    //　　2. 极性：上升沿捕获
    //　　3. 输入脚：TIM1_ICSELECTION_INDIRECTTI 表示直接将 T3 信号经过 TI3FP4 发送给 IC4 模块进行处理
    //　　4. 预分频：意思是控制在多少个输入周期做一次捕获；【经过实际测试，复位触发模式下该值无效，都是按照1分频执行】
    //       　　　　1M 系统时钟，捕获 20Hz PWM 一个周期需要计数 50000  次（1M/20=50000），TIM1 最大计数有65536，可以捕获
    //       　　　　1M 系统时钟，捕获 15Hz PWM 一个周期需要计数 66666  次（1M/15=66666），TIM1 最大计数有65536，无法捕获
    //　　5. 滤波频率：经历几个周期相同的跳变则波形稳定。如果频率捕获偶发值不对，那么可以启用 0x02（4个事件）
//    TIM1_ICInit(TIM1_CHANNEL_4, TIM1_ICPOLARITY_FALLING, TIM1_ICSELECTION_INDIRECTTI, TIM1_ICPSC_DIV1, 0x02);

    
    TIM1_ClearITPendingBit(TIM1_IT_CC3);
    TIM1_ClearFlag(TIM1_FLAG_CC3);
    
    // 输入捕获/比较输出使能
    //【经过实际测试，以下使能代码可以注释掉，但是为了安心还是加上去】
//    TIM1_CCxCmd(TIM1_CHANNEL_3, ENABLE);
//    TIM1_CCxCmd(TIM1_CHANNEL_4, ENABLE);
}
void TIM1_PWMI_InitData56()
{
    GV_Tim1PwmiFreq56 = 0;
    GV_Tim1PwmiDuty56 = 0.0;
}
void TIM1_PWMI_InitData57()
{
    GV_Tim1PwmiFreq57 = 0;
    GV_Tim1PwmiDuty57 = 0.0;
}


/**
 * 开始 PWM 捕获
 */
void TIM1_PWMI_Start()
{
    // 清空俘获中断标志


    // 启用中断函数：俘获比较通道CH1中断
    TIM1_ITConfig(TIM1_IT_CC1, ENABLE);
    TIM1_ITConfig(TIM1_IT_CC3, ENABLE);

    // 开定时器
    //【经过实际测试，定时器就算未开启，上面的 TIM1_IT_CC1 中断仍正常进入，但是获取的计数器值不确定】
    TIM1_Cmd(ENABLE);
}


/**
 * 终止 PWM 捕获
 * 利用 TIM1_Cmd(DISABLE) 关闭捕获，注意其他函数可能受影响，例如：TIM1_PWMI_Init
 */
void TIM1_PWMI_Stop()
{
    // 关闭中断函数：俘获比较通道CH1中断
    TIM1_ITConfig(TIM1_IT_CC1, DISABLE);
    TIM1_ITConfig(TIM1_IT_CC3, DISABLE);

    // 关闭定时器
    TIM1_Cmd(DISABLE);
}


/**
 * 获取 PWM 频率
 *
 * @param short number  PCB上的编号：56, 57
 * @return integer      频率
 */
u16 TIM1_PWMI_GetFreq( short number )
{
    // 返回频率
    if( number == 56 ) return GV_Tim1PwmiFreq56;
    if( number == 57 ) return GV_Tim1PwmiFreq57;
        
    // 无效参数
    return 0;
}


/**
 * 获取 PWM 正占空比
 *
 * @param short number  PCB上的编号：56, 57
 * @return double       占空比
 */
double TIM1_PWMI_GetDuty( short number )
{
    // 返回占空比
    if( number == 56 ) return GV_Tim1PwmiDuty56;
    if( number == 57 ) return GV_Tim1PwmiDuty57;
        
    // 无效参数
    return 0;
}



/* -------------------------------------------------- */
// ● 中断回调函数
//   主调用入口参见 stm8s_it.c
/* -------------------------------------------------- */


/**
 * TIM1 中断
 */
void TIM1_InterruptHandler()
{
    // 中断：计数器值溢出更新
//    if( TIM1_GetITStatus(TIM1_IT_UPDATE) ) TIM1_InterruptHandlerUpdate();
    
    // 中断：PWM 捕获
    // 对于 PWM = 0 或者 100 的捕获有问题，因为这两个 PWM 值都无法触发中断
//    if( TIM1_GetITStatus(TIM1_IT_CC1) != RESET ) TIM1_InterruptHandlerCapturePwm56();
//    if( TIM1_GetITStatus(TIM1_IT_CC3) != RESET ) TIM1_InterruptHandlerCapturePwm57();
}

/**
 * TIM1 中断：PWM 捕获
 */
void TIM1_InterruptHandlerCapturePwm56()
{
    // 初始化定时器输入捕获值
    // 分别对应一个PWM周期的第一个上升沿，第一个下降沿，第二个上升沿
    static u16 icValueA, icValueB, icValueC;
    static u8 K = 0;
    // 获取定时器输入捕获值
    if( K == 0 )
    {
        icValueA = icValueB = icValueC = 0;
        icValueA = TIM1_GetCapture1();
        K = 1;
        
        TIM1->CCER1 |= TIM1_CCER1_CC1P;
    }
    else if(K == 1)
    {
        icValueB = TIM1_GetCapture1();
        K = 2;
        
        TIM1->CCER1 &= (uint8_t)(~TIM1_CCER1_CC1P);
    }
    else
    {

        icValueC = TIM1_GetCapture1() - icValueA;
        GV_Tim1PwmiFreq56 = 16000000/icValueC;
        GV_Tim1PwmiDuty56 = (icValueB-icValueA)*100.0/icValueC;
        K = icValueA = icValueB = icValueC = 0;        
    }

    // 清除中断标志位
    TIM1_ClearITPendingBit(TIM1_IT_CC1);    
    TIM1_ClearFlag(TIM1_FLAG_CC1);
}
void TIM1_InterruptHandlerCapturePwm57()
{
    // 初始化定时器输入捕获值
    // 分别对应一个PWM周期的第一个上升沿，第一个下降沿，第二个上升沿
    static u16 icValueA3, icValueB3, icValueC3;
    static u8 K3 = 0;
    
    // 获取定时器输入捕获值
    if( K3 == 0 )
    {
        icValueA3 = icValueB3 = icValueC3 = 0;
        icValueA3 = TIM1_GetCapture3();
        K3 = 1;
        
        TIM1->CCER2 |= TIM1_CCER2_CC3P;
    }
    else if(K3 == 1)
    {
        icValueB3 = TIM1_GetCapture3();
        K3 = 2;
        
        TIM1->CCER2 &= (uint8_t)(~TIM1_CCER2_CC3P);
    }
    else
    {
        icValueC3 = TIM1_GetCapture3() - icValueA3;
        GV_Tim1PwmiFreq57 = 16000000/icValueC3;
        GV_Tim1PwmiDuty57 = (icValueB3-icValueA3)*100.0/icValueC3;
        K3 = icValueA3 = icValueB3 = icValueC3 = 0;
    }

    // 清除中断标志位
    TIM1_ClearITPendingBit(TIM1_IT_CC3);    
    TIM1_ClearFlag(TIM1_FLAG_CC3);
}