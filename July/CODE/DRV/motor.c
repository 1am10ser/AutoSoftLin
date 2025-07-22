#include "common.h"



/* -------------------------------------------------- */
// ● 采用 STEP/DIR 方式驱动
// ● 对于 TMC2209 芯片，还支持 STEP/DIR 和 UART 混用
/* -------------------------------------------------- */



/* -------------------------------------------------- */
// ● 全局变量
/* -------------------------------------------------- */

/**
 * MOTOR 相关变量
 */
bool  GV_Motor = false;  //【系统使用】电机启用

short GV_MotorDir = 0;  //【系统使用】电机方向
short GV_MotorDiv = 0;  //【系统使用】电机细分

long  GV_MotorSpeed     = 30;    //【按需设置】电机运行速度，单位：RPM（圈/分钟）
                                 //            按照步进脚1.8°，细分18，那么每个就是 0.1°/Hz；1000Hz = 100°/秒
                                 //            30RPM=0.5圈/秒=1800Hz，60RPM=1圈/秒=3600Hz，
long  GV_MotorSpeedFreq = 1800;  //【系统使用】电机运行频率
                                 //            该频率值和 GV_MotorSpeed 直接关联，此处预设的是30RPM时的频率，平时使用时 MOTOR_InitSpeed() 自动调整

short GV_MotorState     = 0;  //【系统使用】电机当前状态（参见 motor.h 中的 MOTOR_STATE_XXX 状态值）
short GV_MotorStatePrev = 0;  //【系统使用】电机上一个状态（电机停止后当前状态会自动保存为上一个状态）

long  GV_MotorCountPulse  = 0;  //【系统使用】电机归零后累计脉冲数
long  GV_MotorCountCircle = 0;  //【系统使用】电机归零后走的总圈数（正表示顺时针，负表示逆时针）
long  GV_MotorCountDegree = 0;  //【系统使用】电机归零后走的单圈度数（正表示顺时针，负表示逆时针，同时超过360°后并统计到圈数变量中；角度值放大10000倍存储，避免了浮点保证了精度）



/* -------------------------------------------------- */
// ● 函数主体
/* -------------------------------------------------- */

/**
 * 初始化
 * 默认低细分、顺时针、电机启用、TIM2初始化
 */
void MOTOR_Init()
{
    // 初始化细分
    GPIO_Init(MOTOR_DIV_PORT, MOTOR_DIV_PIN, GPIO_MODE_OUT_OD_LOW_SLOW);
    MOTOR_SetDiv(MOTOR_DIV_LOW);

    // 初始化计数
    MOTOR_InitCount();

    // 初始化电机方向
    // 基于原理图，高阻态时 DIR 脚为高电平、 低电平时 DIR 脚为低电平
    //
    // 我们规定高电平时电机顺时针旋转（或者向前走），低电平时电机逆时针旋转（或者向后走）。
    // 我们需要根据实际情况调整电机和驱动芯片间的连线，使之符合上述的规则
    GPIO_Init(MOTOR_DIR_PORT, MOTOR_DIR_PIN, GPIO_MODE_OUT_OD_LOW_SLOW);  // 低速开漏输出高阻态
    MOTOR_SetDir(MOTOR_DIR_CW);

    // 初始化启禁
    GPIO_Init(MOTOR_ENABLE_PORT, MOTOR_ENABLE_PIN, GPIO_MODE_OUT_PP_LOW_FAST);  // 低速推挽输出低电平
    MOTOR_Enable();

    // 初始化电机脉冲频率
    TIM2_PWMO_Init(GV_MotorSpeedFreq);

    // 初始化标记
    GV_Motor = true;
}


/**
 * 初始化计数
 */
void MOTOR_InitCount()
{
    GV_MotorCountPulse  = 0;
    GV_MotorCountCircle = 0;
    GV_MotorCountDegree = 0;
}


/**
 * 电机启用
 */
void MOTOR_Enable()
{
    // 4988、TB67S109、TMC2209
    GPIO_WriteLow(MOTOR_ENABLE_PORT, MOTOR_ENABLE_PIN);
}


/**
 * 电机禁用
 */
void MOTOR_Disable()
{
    // 4988、TB67S109、TMC2209
    GPIO_WriteHigh(MOTOR_ENABLE_PORT, MOTOR_ENABLE_PIN);
}


/**
 * 电机计数
 * 基于一个脉冲的累计，函数自动根据电机方向和细分累加相关的计数
 */
void MOTOR_CountStep()
{
    // 细分的步距角（度）
    // 角度值需要乘以 10000，避免了浮点运算保证了精度
    long divCircles = 3600000;
    short divStep = (short)(MOTOR_STEP_DEGREE*10000)/GV_MotorDiv;
    
    // 脉冲累计
    GV_MotorCountPulse++;
    
    // 记录度数累加
    if( GV_MotorDir == MOTOR_DIR_CW  ) GV_MotorCountDegree += divStep;
    if( GV_MotorDir == MOTOR_DIR_CCW ) GV_MotorCountDegree -= divStep;

    // 度数转圈数
    if( GV_MotorCountDegree >= divCircles || GV_MotorCountDegree <= -divCircles ) GV_MotorCountCircle += GV_MotorCountDegree/divCircles;
    GV_MotorCountDegree = GV_MotorCountDegree % divCircles;
}


/**
 * 获取累计的脉冲数
 */
long MOTOR_GetCountPulse()
{ 
    return GV_MotorCountPulse; 
}


/**
 * 获取累计的圈数
 * 获取累计的圈数值：正数表示顺时针，负数表示逆时针
 */
long MOTOR_GetCountCircle()
{ 
    return GV_MotorCountCircle; 
}


/**
 * 获取累计的角度
 * 获取单圈的角度值：正数表示顺时针，负数表示逆时针
 */
double MOTOR_GetCountDegree()
{ 
    return GV_MotorCountDegree*1.0/10000; 
}


/**
 * 设置电机细分
 * 由于测试版上 IO 口不够用，所以 A4988 无法独立控制全部细分模式
 * 目前的设计只能采用初始一种细分，然后软件再动态控制另外一种细分。至于初始的和动态控制的是哪种细分需要预先在 PCB 板上进行跳线设置
 *
 * @param short div  细分值：MOTOR_DIV_LOW, MOTOR_DIV_HIGH
 */
void MOTOR_SetDiv( short div )
{
    // 无效参数
    if( !(div==MOTOR_DIV_LOW||div==MOTOR_DIV_HIGH) ) return;
    
    // 初始的细分（PE2低电平）
    if( div == MOTOR_DIV_LOW ) GPIO_WriteLow(MOTOR_DIV_PORT, MOTOR_DIV_PIN);

    // 软控的细分（PE2高组态）
    if( div == MOTOR_DIV_HIGH ) GPIO_WriteHigh(MOTOR_DIV_PORT, MOTOR_DIV_PIN);

    // 设置全局变量
    GV_MotorDiv = div;
}


/**
 * 获取电机细分
 */
short MOTOR_GetDiv()
{
    return GV_MotorDiv;
}


/**
 * 设置电机方向
 *
 * @param short dir  方向：MOTOR_DIR_CW、MOTOR_DIR_CCW
 */
void MOTOR_SetDir( short dir )
{
    if( dir == MOTOR_DIR_CW  ){ GPIO_WriteLow(MOTOR_DIR_PORT, MOTOR_DIR_PIN);  GV_MotorDir = dir; }  // 低电平
    if( dir == MOTOR_DIR_CCW ){ GPIO_WriteHigh(MOTOR_DIR_PORT, MOTOR_DIR_PIN); GV_MotorDir = dir; }  // 高阻态
}


/**
 * 获取电机方向
 */
short MOTOR_GetDir()
{
    return GV_MotorDir;
}


/**
 * 初始化电机运行速度（RPM）
 * 该函数设置完成后会直接生效输出信号，建议调用前先终止电机运行
 *
 * @param short speed  运行速度，单位：RPM（圈/分钟）
 */
void MOTOR_InitSpeed( short speed )
{
    // 将 speed 转为频率
    // 频率 = 转速（圈/秒） * 一圈总频率个数
    // 频率 = (speed/60) * (360/(MOTOR_STEP_DEGREE/GV_MotorDiv))
    GV_MotorSpeedFreq = (long)((360*(u32)speed*GV_MotorDiv)/(60*MOTOR_STEP_DEGREE));

    // 初始化电机脉冲频率
    TIM2_PWMO_Init(GV_MotorSpeedFreq);
}


/**
 * 获取电机运行速度（RPM）
 */
short MOTOR_GetSpeed()
{
    return GV_MotorSpeed;
}


/**
 * 设置电机当前状态
 *
 * @param short state  状态，参见 motor.h 中的 MOTOR_STATE_XXX
 */
void MOTOR_SetState( short state )
{
    GV_MotorState = state;
}


/**
 * 获取电机当前状态
 */
short MOTOR_GetState()
{
    return GV_MotorState;
}


/**
 * 设置电机上一个状态
 *
 * @param short state  状态，参见 motor.h 中的 MOTOR_STATE_XXX
 */
void MOTOR_SetStatePrev( short state )
{
    GV_MotorStatePrev = state;
}


/**
 * 获取电机上一个状态
 */
short MOTOR_GetStatePrev()
{
    return GV_MotorStatePrev;
}


/**
 * 判断电机是否已归零
 * 获取归零触发脚的实时值判断当前电机是否处于归零位置
 */
bool MOTOR_IsZero()
{
    if( MOTOR_ZERO_EXTITYPE == 0 && EXTI_GetValue(MOTOR_ZERO_EXTINUM) == 0 ) return true;
    if( MOTOR_ZERO_EXTITYPE == 1 && EXTI_GetValue(MOTOR_ZERO_EXTINUM) == 1 ) return true;
    return false;
}


/**
 * 停止运行电机
 * 该函数会设置 GV_MotorStatePrev 和 GV_MotorState = MOTOR_STATE_IDLE
 *
 * @return boolean  true 表示执行成功，false 表示执行失败
 */
bool MOTOR_RunStop()
{
    // 终止发送脉冲
    // 该函数会设置 GV_MotorStatePrev 和 GV_MotorState = MOTOR_STATE_IDLE
    TIM2_PWMO_Stop();

    // 返回
    return true;
}


/**
 * 保持运行电机
 * 该函数会默认设置电机运行状态为 MOTOR_STATE_RUN
 * 如果需要自定义电机的运行状态，那么需要预先调用 MOTOR_SetState(...) 进行设置 
 *
 * @return boolean  true 表示执行成功，false 表示执行失败
 */
bool MOTOR_RunKeep()
{
    // 设置默认状态
    // 该状态会在 TIM2_PWMO_Stop() 设置为 MOTOR_STATE_IDLE
    if( GV_MotorState == MOTOR_STATE_IDLE ) GV_MotorState = MOTOR_STATE_RUN;
    
    // 发送脉冲
    return TIM2_PWMO_Send(50, 0);
}


/**
 * 按步数运行电机
 * 一步等于一个步进脚
 *
 * 该函数会默认设置电机运行状态为 MOTOR_STATE_RUN
 * 如果需要自定义电机的运行状态，那么需要预先调用 MOTOR_SetState(...) 进行设置 
 *
 * @param u32 number  步数
 * @return boolean  true 表示执行成功，false 表示执行失败
 */
bool MOTOR_RunStep( u32 number )
{
    // 无效参数
    if( number <= 0 ) return false;

    // 设置默认状态
    // 该状态会在 TIM2_PWMO_Stop() 设置为 MOTOR_STATE_IDLE
    if( GV_MotorState == MOTOR_STATE_IDLE ) GV_MotorState = MOTOR_STATE_RUN;
    
    // 发送脉冲
    return TIM2_PWMO_Send(50, number);
}


/**
 * 按长度值运行电机
 * 该函数会默认设置电机运行状态为 MOTOR_STATE_RUN
 * 如果需要自定义电机的运行状态，那么需要预先调用 MOTOR_SetState(...) 进行设置 
 *
 * @param double length  长度，单位：mm
 * @return boolean  true 表示执行成功，false 表示执行失败
 */
bool MOTOR_RunLength( double length )
{
    // 无效参数
    if( length <= 0 ) return false;

    // 构建基本单位参数
    double circlePulses = 360.0/(MOTOR_STEP_DEGREE/GV_MotorDiv);  // 电机走一圈所需的脉冲数量
    double pulseLength = MOTOR_CIRCLE_LEN*1.0/circlePulses;  // 一个脉冲电机走的距离，单位：mm

    // 构建发送的脉冲个数
    u32 sendLimit = (u32)(length/pulseLength);

    // 设置默认状态
    // 该状态会在 TIM2_PWMO_Stop() 设置为 MOTOR_STATE_IDLE
    if( GV_MotorState == MOTOR_STATE_IDLE ) GV_MotorState = MOTOR_STATE_RUN;
    
    // 发送脉冲
    return TIM2_PWMO_Send(50, sendLimit);
}


/**
 * 按角度值运行电机
 * 该函数会默认设置电机运行状态为 MOTOR_STATE_RUN
 * 如果需要自定义电机的运行状态，那么需要预先调用 MOTOR_SetState(...) 进行设置 
 *
 * @param double degree  角度，单位：度
 * @return boolean  true 表示执行成功，false 表示执行失败
 */
bool MOTOR_RunDegree( double degree )
{
    // 无效参数
    if( degree <= 0 ) return false;
 
    // 构建发送的脉冲个数
    u32 sendLimit = (int)(degree/(MOTOR_STEP_DEGREE/GV_MotorDiv));

    // 设置默认状态
    // 该状态会在 TIM2_PWMO_Stop() 设置为 MOTOR_STATE_IDLE
    if( GV_MotorState == MOTOR_STATE_IDLE ) GV_MotorState = MOTOR_STATE_RUN;
    
    // 发送脉冲
    return TIM2_PWMO_Send(50, sendLimit);
}



/* -------------------------------------------------- */
// ● 中断回调函数
//   主调用入口参见 tim2.c 和 extint.c
/* -------------------------------------------------- */

/**
 * 电机脉冲中断
 * 每发送一个脉冲都会调用这个函数
 * 电机由 TIM2 的 PWM 进行控制，所以该中断函数由 TIM2 的 “计数器值溢出更新” 中断函数被动调用
 * 所以该中断函数执行的频次是基于脉冲为单位的，当然我们也对其做了限制（即电机未运行时该中断函数不会被调用）
 */
void MOTOR_InterruptHandlerPulse()
{
    MOTOR_CountStep();
}


/**
 * 电机归零完成中断
 * 主要用于电机运行中并触发原点时的电机停止操作
 *
 * @param short extiNum   中断引脚：J60 或 J61
 * @param short extiType  中断类型：0 表示下降沿，1 表示上升沿
 */
void MOTOR_InterruptHandlerZeroFinish( short extiNum, short extiType )
{
    // 匹配参数
    if( extiNum  != MOTOR_ZERO_EXTINUM  ) return;
    if( extiType != MOTOR_ZERO_EXTITYPE ) return;

    // 电机停止
    // 该函数会设置 GV_MotorStatePrev 和 GV_MotorState = MOTOR_STATE_IDLE
    MOTOR_RunStop();

    // 初始化计数
    MOTOR_InitCount();
}



/* -------------------------------------------------- */
// ● 主逻辑代码
/* -------------------------------------------------- */

/**
 * 主逻辑：电机两点往返运行
 * 
 * @param short length  往返长度，单位：mm
 */
void MOTOR_MainTwoPointRunByLength( short length )
{
    if( GV_MotorState != MOTOR_STATE_IDLE ) return;
    delay_ms(2000);

    if( MOTOR_GetDir() == MOTOR_DIR_CW  ){ MOTOR_SetDir(MOTOR_DIR_CCW); MOTOR_RunLength(length); return; }
    if( MOTOR_GetDir() == MOTOR_DIR_CCW ){ MOTOR_SetDir(MOTOR_DIR_CW); MOTOR_RunLength(length); return;  }
}


/**
 * 主逻辑：电机两点往返运行
 * 
 * @param short degree  往返角度，单位：度
 */
void MOTOR_MainTwoPointRunByDegree( short degree )
{
    if( GV_MotorState != MOTOR_STATE_IDLE ) return;
    delay_ms(2000);
    
    if( MOTOR_GetDir() == MOTOR_DIR_CW  ){ MOTOR_SetDir(MOTOR_DIR_CCW); MOTOR_RunDegree(degree); return; }
    if( MOTOR_GetDir() == MOTOR_DIR_CCW ){ MOTOR_SetDir(MOTOR_DIR_CW); MOTOR_RunDegree(degree); return;  }
}
    

/**
 * 主逻辑：按键控制电机(实时)
 * 　　　　按键 43 控制电机 - 低细分:顺时针/向前
 * 　　　　按键 44 控制电机 - 低细分:逆时针/向后
 * 　　　　按键 45 控制电机 - 高细分:顺时针/向前
 * 　　　　按键 46 控制电机 - 高细分:逆时针/向后
 */
void MOTOR_MainKeyControlMotorReal()
{
    // 初始化
    short i;
    short flag, keys[4] = {43,44,45,46};
    
    // 按键数据加载
    KEY_DatLoad();
    
    // 按键全部松开时，电机停止运行
    flag = true; 
    for( i = 0; i < 4; i++) flag &= KEY_IsPress(keys[i],false) == KEY_STATE_NO;
    if( flag == true ){ MOTOR_RunStop(); return; }

    // 细分：低细分，方向：顺时针/向前
    // TODO...

    // 细分：低细分，方向：逆时针/向后
    // TODO...

    // 细分：高细分，方向：顺时针/向前
    // TODO...

    // 细分：高细分，方向：逆时针/向后
    // TODO...
}


/**
 * 主逻辑：按键控制电机(按长度)
 * 　　　　按键 43 控制电机 - 低细分:顺时针/向前
 * 　　　　按键 44 控制电机 - 低细分:逆时针/向后
 * 　　　　按键 45 控制电机 - 高细分:顺时针/向前
 * 　　　　按键 46 控制电机 - 高细分:逆时针/向后
 *
 * @param double length  长度，单位：mm
 */
void MOTOR_MainKeyControlMotorByLength( double length )
{
    // TODO...
}