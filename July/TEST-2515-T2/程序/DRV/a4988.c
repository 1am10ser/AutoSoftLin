#include "common.h"



/* -------------------------------------------------- */
// ● 全局变量
/* -------------------------------------------------- */

/**
 * A4988 相关变量
 */
bool  GV_A4988Motor       = false;  //【系统使用】电机启用
bool  GV_A4988MotorRuning = false;  //【系统使用】电机计数开始

long  GV_A4988MotorCountCircle = 0;  //【系统使用】电机复位后走的总圈数（正表示顺时针，负表示逆时针）
long  GV_A4988MotorCountDegree = 0;  //【系统使用】电机复位后走的单圈度数（正表示顺时针，负表示逆时针，同时超过360°后并统计到圈数变量中）

short GV_A4988MotorDir    = 0;  //【系统使用】电机方向
short GV_A4988MotorSubdiv = 0;  //【系统使用】电机细分

short GV_A4988MotorStateKeepRun    = -1;  //【系统使用】电机状态：是否保持运行
short GV_A4988MotorStateKeepRunDir = -1;  //【系统使用】电机状态：保持运行时方向



/* -------------------------------------------------- */
// ● 函数主体
/* -------------------------------------------------- */

/**
 * 初始化
 */
void A4988_Init()
{
    // 初始化细分
    GPIO_Init(A4988_MS_PORT, A4988_MS_PIN, GPIO_MODE_OUT_OD_LOW_SLOW);
    A4988_SetSubdiv(A4988_MS_MODE0_SUBDIV);

    // 初始化计数
    A4988_InitCount();

    // 初始化电机方向
    // 基于原理图，高阻态时 DIR 脚为高电平、 低电平时 DIR 脚为低电平
    //
    // 我们规定高电平时电机顺时针旋转（或者向前走），低电平时电机逆时针旋转（或者向后走）。
    // 我们需要根据实际情况调整电机和驱动芯片间的连线，使之符合上述的规则
    GPIO_Init(A4988_DIR_PORT, A4988_DIR_PIN, GPIO_MODE_OUT_OD_HIZ_SLOW); // 低速开漏输出高阻态

    // 初始化电机脉冲频率
    TIM2_PWMO_Init(A4988_MOTOR_PWM_FREQ);

    // 初始化标记
    GV_A4988Motor = true;
}


/**
 * 初始化计数
 */
void A4988_InitCount()
{
    GV_A4988MotorCountCircle = 0;
    GV_A4988MotorCountDegree = 0;
}


/**
 * 电机计数
 * 基于一个脉冲的累计，函数自动根据电机方向和细分累加相关的计数
 */
void A4988_CountStep()
{
    // 细分的步距角（度）
    // 乘以 10000 是采用整形方式代替浮点数
    long subdivCircles = 3600000;
    short subdivStep = (short)(A4988_MOTOR_STEP_DEGREE*10000)/GV_A4988MotorSubdiv;
    
    // 记录度数累加
    if( GV_A4988MotorDir == A4988_DIR_BACK  ) GV_A4988MotorCountDegree -= subdivStep;
    if( GV_A4988MotorDir == A4988_DIR_FRONT ) GV_A4988MotorCountDegree += subdivStep;

    // 度数转圈数
    if( GV_A4988MotorCountDegree >= subdivCircles || GV_A4988MotorCountDegree <= -subdivCircles ) GV_A4988MotorCountCircle += GV_A4988MotorCountDegree/subdivCircles;
    GV_A4988MotorCountDegree = GV_A4988MotorCountDegree % subdivCircles;
}


/**
 * 获取统计的圈数
 * 获取累计的圈数值：正数表示顺时针，负数表示逆时针
 */
long A4988_GetCountCircle()
{ 
    return GV_A4988MotorCountCircle; 
}


/**
 * 获取统计的角度
 * 获取单圈的角度值：正数表示顺时针，负数表示逆时针
 */
double A4988_GetCountDegree()
{ 
    return GV_A4988MotorCountDegree*1.0/10000; 
}


/**
 * 设置电机细分
 * 由于测试版上 IO 口不够用，所以 A4988 无法独立控制全部细分模式
 * 目前的设计只能采用初始一种细分，然后软件再动态控制另外一种细分。至于初始的和动态控制的是哪种细分需要预先在 PCB 板上进行跳线设置
 * 
 * 编号为 TesterProductA.v5_20210604  的PCB板的细分控制支持由 PCB 上直接焊接电阻初始一种细分，也可以通过飞线连接到 PE2 然后软件控制另外一种细分
 * 编号为 TesterProductA.v5_20210605+ 的PCB板的细分控制支持由 PCB 上直接焊接电阻初始一种细分，也可以通过焊盘连接到 PE2 然后软件控制另外一种细分
 *
 * @param short subdiv  细分值：A4988_MS_MODE0_SUBDIV, A4988_MS_MODE1_SUBDIV
 */
void A4988_SetSubdiv( short subdiv )
{
    // 无效参数
    if( !(subdiv==A4988_MS_MODE0_SUBDIV||subdiv==A4988_MS_MODE1_SUBDIV) ) return;
    
    // 初始的细分（焊接电阻，控制引脚低电平）
    if( subdiv == A4988_MS_MODE0_SUBDIV ) GPIO_WriteLow(A4988_MS_PORT, A4988_MS_PIN);

    // 软控的细分（软件控制，控制引脚高组态）
    if( subdiv == A4988_MS_MODE1_SUBDIV ) GPIO_WriteHigh(A4988_MS_PORT, A4988_MS_PIN);

    // 设置全局变量
    GV_A4988MotorSubdiv = subdiv;
}


/**
 * 获取电机细分
 */
short A4988_GetSubdiv()
{
    return GV_A4988MotorSubdiv;
}


/**
 * 设置电机方向
 *
 * @param short dir  方向：A4988_DIR_FRONT、A4988_DIR_BACK
 */
void A4988_SetDir( short dir )
{
    if( dir == A4988_DIR_BACK  ){ GPIO_WriteLow(A4988_DIR_PORT, A4988_DIR_PIN); GV_A4988MotorDir = dir; } // 低电平
    if( dir == A4988_DIR_FRONT ){ GPIO_WriteHigh(A4988_DIR_PORT, A4988_DIR_PIN); GV_A4988MotorDir = dir; }  // 高阻态
}


/**
 * 获取电机方向
 */
short A4988_GetDir()
{
    return GV_A4988MotorDir;
}


/**
 * 保持运行电机
 * 如果电机处于 保持运行 中，那么自动根据 runKeepProtectPrev 参数忽略本次调用 或者 终止旧的“保持运行”并执行新的运行 
 *
 * @param short   dir                 方向：A4988_DIR_FRONT、A4988_DIR_BACK
 * @param boolean runKeepProtectPrev  上一个 “保持运行” 保护：如果电机正处于保持运行中，那么再次调用该函数，那么函数不执行

 * @return boolean   true 表示执行成功，false 表示执行失败
 */
bool A4988_RunKeep( short dir, short runKeepProtectPrev )
{
    // 启用上一个 “保持运行” 保护 且 电机处于运行中，那么该函数不继续执行
    if( runKeepProtectPrev == true && A4988_RunKeeping(-1) == true ) return false;

    // 如果 电机正在运行 且 方向和参数一致，那么直接返回 true
    if( A4988_RunKeeping(dir) ) return true;
    
    // 停止运行电机
    A4988_RunKeepStop();
    
    // 初始化
    bool result = true;

    // 设置方向
    A4988_SetDir(dir);
    
    // 发送脉冲
    result = TIM2_PWMO_Send(50, 0, true);

    // 保存状态值
    if( result == true ){
        GV_A4988MotorStateKeepRun    = 1;
        GV_A4988MotorStateKeepRunDir = dir;
    }

    // 返回
    return result;
}


/**
 * 停止运行电机
 * 如果电机未运行，那么直接返回 true
 *
 * @return boolean  true 表示执行成功，false 表示执行失败
 */
bool A4988_RunKeepStop()
{
    // 电机未保持运行状态
    if( A4988_RunKeeping(-1) == false ) return true; 
    
    // 终止发送脉冲
    TIM2_PWMO_Stop();

    // 保存状态值
    GV_A4988MotorStateKeepRun    = -1;
    GV_A4988MotorStateKeepRunDir = -1;

    // 返回
    return true;
}


/**
 * 电机是否处于保持运行中
 * 支持匹配电机的方向检查，-1 表示不对方向进行检查
 *
 * @param short dir  方向：-1、A4988_DIR_FRONT、A4988_DIR_BACK
 * @return boolean   true 表示运行中，false 表示未运行
 */
bool A4988_RunKeeping( short dir )
{
    if( GV_A4988MotorStateKeepRun != 1 ) return false; 
    return dir == -1 || GV_A4988MotorStateKeepRunDir == dir ? true : false;
}


/**
 * 按步数运行电机
 * 一步等于一个步进脚
 *
 * @param short dir     方向：A4988_DIR_FRONT、A4988_DIR_BACK
 * @param u32   number  步数
 * 
 * @return boolean  true 表示执行成功，false 表示执行失败
 */
bool A4988_RunStep( short dir, u32 number )
{
    // 无效参数
    if( number <= 0 ) return false;

    // 初始化
    bool result = true;

    // 设置方向
    A4988_SetDir(dir);

    // 发送脉冲
    result = TIM2_PWMO_Send(50, number, true);

    // 返回
    return result;
}


/**
 * 等待电机运行完成
 * 自动根据 当前驱动的频率 以及 预设的电机参数 计算出电机运行需要的较为合理的时间并等待
 *
 * @param u32 number  步数
 */
void A4988_RunStepWait( u32 number )
{
    // 获取电机走一圈所需毫秒数
    // 根据当前的频率，计算出电机走一圈所需毫秒数
    int circleTime = (long)A4988_MOTOR_CIRCLE_TIME*GV_A4988MotorSubdiv;

    // 电机走 1步 所需的毫秒数
    double msPerNumber = circleTime*1.0/(360.0/(A4988_MOTOR_STEP_DEGREE/GV_A4988MotorSubdiv));

    // 延时等待
    delay_ms((int)(number*msPerNumber));
}


/**
 * 按长度值运行电机
 *
 * @param short  dir     方向：A4988_DIR_FRONT、A4988_DIR_BACK
 * @param double length  长度，单位：mm
 * 
 * @return boolean  true 表示执行成功，false 表示执行失败
 */
bool A4988_RunLength( short dir, double length )
{
    // 无效参数
    if( length <= 0 ) return false;
    
    // 初始化
    bool result = true;
    
    // 构建基本单位参数
    double circlePulses = 360.0/(A4988_MOTOR_STEP_DEGREE/GV_A4988MotorSubdiv);  // 电机走一圈脉冲个数
    double pulseLength = A4988_MOTOR_CIRCLE_LEN*1.0/circlePulses;  // 一个脉冲电机走的距离，单位：mm
    
    // 构建发送的脉冲个数
    u32 sendLimit = (u32)(length/pulseLength);

    // 设置方向
    A4988_SetDir(dir);
    
    // 发送脉冲
    result = TIM2_PWMO_Send(50, sendLimit, true);

    // 返回
    return result;
}


/**
 * 等待电机运行完成
 * 自动根据 当前驱动的频率 以及 预设的电机参数 计算出电机运行需要的较为合理的时间并等待
 *
 * @param double length  长度，单位：mm
 */
void A4988_RunLengthWait( double length )
{
    // 获取电机走一圈所需毫秒数
    // 根据当前的频率，计算出电机走一圈所需毫秒数
    int circleTime = (long)A4988_MOTOR_CIRCLE_TIME*GV_A4988MotorSubdiv;

    // 电机走 1毫米 所需的 毫秒数
    double msPerMm = circleTime*1.0/A4988_MOTOR_CIRCLE_LEN;

    // 延时等待
    delay_ms((int)(length*msPerMm));
}


/**
 * 按角度值运行电机
 *
 * @param short  dir     方向：A4988_DIR_FRONT、A4988_DIR_BACK
 * @param double degree  角度，单位：度
 * 
 * @return boolean  true 表示执行成功，false 表示执行失败
 */
bool A4988_RunDegree( short dir, double degree )
{
    // 无效参数
    if( degree <= 0 ) return false;
    
    // 初始化
    bool result = true;

    // 构建发送的脉冲个数
    u32 sendLimit = (int)(degree/(A4988_MOTOR_STEP_DEGREE/GV_A4988MotorSubdiv));

    // 设置方向
    A4988_SetDir(dir);

    // 发送脉冲
    result = TIM2_PWMO_Send(50, sendLimit, true);

    // 返回
    return result;
}


/**
 * 等待电机运行完成
 * 自动根据 当前驱动的频率 以及 预设的电机参数 计算出电机运行需要的较为合理的时间并等待
 *
 * @param double degree  角度，单位：°
 */
void A4988_RunDegreeWait( double degree )
{
    // 获取电机走一圈所需毫秒数
    // 根据当前的频率，计算出电机走一圈所需毫秒数
    int circleTime = (long)A4988_MOTOR_CIRCLE_TIME*GV_A4988MotorSubdiv;

    // 电机走 1度 所需的毫秒数
    double msPerDegree = circleTime*1.0/360;

    // 延时等待
    delay_ms((int)(degree*msPerDegree));
}


/**
 * 电机运行中断
 * 电机由 TIM2 的 PWM 进行控制，所以该中断函数由 TIM2 的 “计数器值溢出更新” 中断函数被动调用
 * 所以该中断函数执行的频次是基于脉冲为单位的，当然我们也对其做了限制（即电机未运行时该中断函数不会被调用）
 */
void A4988_InterruptHandlerRuning()
{
    A4988_CountStep();
}


/* -------------------------------------------------- */
// ● 主逻辑代码
/* -------------------------------------------------- */


/**
 * 主逻辑：初始化
 */
void A4988_MainInit()
{
    A4988_Init();
}


/**
 * 主逻辑：按键控制电机(实时)
 * 　　　　按键 43 控制电机 - MODE0:向前
 * 　　　　按键 44 控制电机 - MODE0:向后
 * 　　　　按键 45 控制电机 - MODE1:向前
 * 　　　　按键 46 控制电机 - MODE1:向后
 */
void A4988_MainKeyControlMotorReal()
{
    // 初始化
    short i;
    short flag, keys[4] = {43,44,45,46};
    
    // 按键数据加载
    KEY_DatLoad();
    
    // 按键全部松开时，电机停止运行
    flag = true; 
    for( i = 0; i < 4; i++) flag &= KEY_IsPress(keys[i],false) == KEY_STATE_NO;
    if( flag == true ){ A4988_RunKeepStop(); return; }

    // 细分：MODE0，方向：向前
    if( KEY_IsPress(43,false) == KEY_STATE_YES ){
        if( A4988_GetSubdiv() == A4988_MS_MODE0_SUBDIV && A4988_RunKeeping(A4988_DIR_FRONT) == true ) return;
        A4988_RunKeepStop(); A4988_SetSubdiv(A4988_MS_MODE0_SUBDIV); A4988_RunKeep(A4988_DIR_FRONT, false);
    }

    // 细分：MODE0，方向：向后
    if( KEY_IsPress(44,false) == KEY_STATE_YES ){
        if( A4988_GetSubdiv() == A4988_MS_MODE0_SUBDIV && A4988_RunKeeping(A4988_DIR_BACK) == true ) return;
        A4988_RunKeepStop(); A4988_SetSubdiv(A4988_MS_MODE0_SUBDIV); A4988_RunKeep(A4988_DIR_BACK, false);
    }

    // 细分：MODE1，方向：向前
    if( KEY_IsPress(45,false) == KEY_STATE_YES ){
        if( A4988_GetSubdiv() == A4988_MS_MODE1_SUBDIV && A4988_RunKeeping(A4988_DIR_FRONT) == true ) return;
        A4988_RunKeepStop(); A4988_SetSubdiv(A4988_MS_MODE1_SUBDIV); A4988_RunKeep(A4988_DIR_FRONT, false);
    }

    // 细分：MODE1，方向：向后
    if( KEY_IsPress(46,false) == KEY_STATE_YES ){
        if( A4988_GetSubdiv() == A4988_MS_MODE1_SUBDIV && A4988_RunKeeping(A4988_DIR_BACK) == true ) return;
        A4988_RunKeepStop(); A4988_SetSubdiv(A4988_MS_MODE1_SUBDIV); A4988_RunKeep(A4988_DIR_BACK, false);
    }
}


/**
 * 主逻辑：按键控制电机(按长度)
 * 　　　　按键 43 控制电机 - MODE0:向前
 * 　　　　按键 44 控制电机 - MODE0:向后
 * 　　　　按键 45 控制电机 - MODE1:向前
 * 　　　　按键 46 控制电机 - MODE1:向后
 *
 * @param double length  长度，单位：mm
 */
void A4988_MainKeyControlMotorByLength( double length )
{
    // TODO...
}
