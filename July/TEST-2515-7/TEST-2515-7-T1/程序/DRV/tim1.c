#include "common.h"



/* -------------------------------------------------- */
// ● 全局变量
/* -------------------------------------------------- */

/**
 * TIM1 PWM 输出基础变量
 * PCB 线路板上 TIM1 输出编号为 56和57
 * 考虑到不同通道输出/捕获时都同时设置 TIM1_TimeBaseInit 函数，会相互干扰，所以使用 TIM1 就启用一个功能：PWM输出 或 PWM输入捕获
 */
short  GV_Tim1PwmoArr = 0;  //【系统使用】计数器重装载值，自动根据频率计算
short  GV_Tim1PwmoNumbers[] = {56, 57};  //【按需设置】PWM输出 编号集，只有将 PWM输出 编号加入到这个变量后才能实现对应 PWM输出 的相关功能

short  GV_Tim1PwmoSendSoon56     = 0;  //【系统使用】即将发送标记
double GV_Tim1PwmoSendSoon56Duty = 0;  //【系统使用】即将发送的占空比数据
u32    GV_Tim1PwmoSendLimit56    = 0;  //【系统使用】发送个数限制
short  GV_Tim1PwmoSendCount56    = 0;  //【系统使用】发送个数累计
short  GV_Tim1PwmoSendSoon57     = 0;  //【系统使用】即将发送标记
double GV_Tim1PwmoSendSoon57Duty = 0;  //【系统使用】即将发送的占空比数据
u32    GV_Tim1PwmoSendLimit57    = 0;  //【系统使用】发送个数限制
short  GV_Tim1PwmoSendCount57    = 0;  //【系统使用】发送个数累计


/**
 * TIM1 PWM 输入捕获基础变量
 * PCB 线路板上 TIM1 捕获编号为 56和57
 * 考虑到不同通道输出/捕获时都同时设置 TIM1_TimeBaseInit 函数，会相互干扰，所以使用 TIM1 就启用一个功能：PWM输出 或 PWM输入捕获
 */
short  GV_Tim1PwmiNumbers[] = {56, 57};  //【按需设置】PWM捕获 编号集，只有将 PWM输出 编号加入到这个变量后才能实现对应 PWM输出 的相关功能

u16    GV_Tim1PwmiFreq56 = 0;    //【系统使用】定时器捕获的 PWM 频率
double GV_Tim1PwmiDuty56 = 0.0;  //【系统使用】定时器捕获的 PWM 正占空比

u16    GV_Tim1PwmiFreq57 = 0;    //【系统使用】定时器捕获的 PWM 频率
double GV_Tim1PwmiDuty57 = 0.0;  //【系统使用】定时器捕获的 PWM 正占空比



/* -------------------------------------------------- */
// ● 函数主体（PWM输出）
// ● PCB 线路板上 TIM1 输出编号为 56和57
// ● 考虑到不同通道输出/捕获时都同时设置 TIM1_TimeBaseInit 函数，会相互干扰，所以使用 TIM1 就启用一个功能：PWM输出 或 PWM输入捕获
/* -------------------------------------------------- */

/**
 * 初始化 TIM1 PWM 输出
 * 初始化后端口并不会输出 PWM 信号，需要调用 TIM1_PWMO_Send(...) 进行输出
 *
 * @param int   freq    PWM 输出频率：100 < freq < 450000（16分频，能输出比较好波形，占空比较准）
 *                          　　　　　100 < freq <  40000（160分频，能输出比较好波形，占空比较准）
 */
void TIM1_PWMO_Init( u32 freq )
{
    // 预分频
    u16 prescaler = 16;
    
    // 重置初始化
    TIM1_DeInit();
    
    // 初始化计数器重装载值
    // 根据系统时钟频率、分频率和期望频率算出计数器重装载值
    GV_Tim1PwmoArr = 16000000/prescaler/freq;

    // 初始化 TIM1 时基单元
    // 16预分频，向上计数，每 1s/1M = 1us 计数器值加 1（累加 1000 次需要 1 毫秒）
    // 参数说明：预分配值，计数模式，自动重装载值（计数器值到 GV_Tim1PwmoArr 后重新装载），计数器起始值（计数器值重新装载后从0开始）
    TIM1_TimeBaseInit(prescaler-1, TIM1_COUNTERMODE_UP, GV_Tim1PwmoArr-1, 0);

    // 自动预装载允许位
    // ARR：Auto-Reload Register
    TIM1_ARRPreloadConfig(ENABLE);

    // 初始化 TIM1 通道
    for( int i=0; i < ARR_LENGTH(GV_Tim1PwmoNumbers); i++ ){
        // 初始化通道的俘获/比较
        // 此处我们设置 TIM1_OCMODE_PWM2+TIM1_OCPOLARITY_HIGH，确保初始时为低电平
        //
        // 参数说明：
        //　　1. PWM输出模式：TIM1_OCMODE_PWM1（跳变前输出有效电平）；TIM1_OCMODE_PWM2（跳变前输出无效电平）
        //　　      　　　　　跳变前是指计时器值小于比较器设定的值
        //　　2. 禁止或者启用输出比较
        //　　3. 禁止或者启用互补输出比较
        //　　4. 脉冲宽度值：如果计时器自动装载设置为 2000，脉冲宽度值我们设置为 1000，那么将输出 50% 的占空比。同时该值后期可以动态更改
        //　　5. 输出比较极性：需要结合 参数1 确定跳变前的有效电平（参见《【众拳STM8 39 讲】PWM 输出实验》），通常 TIM1_OCMODE_PWM2+TIM1_OCPOLARITY_LOW 比较符合人的常规思维
        //　　   　　　　　　　此处我们使用 TIM1_OCMODE_PWM2+TIM1_OCPOLARITY_HIGH 方式，主要是确保PWM初始化默认输出低电平，该模式下通过 TIM1_SetCompare 设置的占空比值就是负占空比了，开发时注意
        //　　6. 互补输出比较极性：【随便给，以后再研究】
        //　　7. 输出比较空闲状态：【随便给，以后再研究】
        //　　8. 互补输出比较空闲状态：【随便给，以后再研究】
        if( GV_Tim1PwmoNumbers[i] == 56 ) TIM1_OC1Init(TIM1_OCMODE_PWM2, TIM1_OUTPUTSTATE_DISABLE, TIM1_OUTPUTNSTATE_DISABLE, 0, TIM1_OCPOLARITY_HIGH, TIM1_OCNPOLARITY_HIGH, TIM1_OCIDLESTATE_RESET, TIM1_OCNIDLESTATE_RESET);
        if( GV_Tim1PwmoNumbers[i] == 57 ) TIM1_OC3Init(TIM1_OCMODE_PWM2, TIM1_OUTPUTSTATE_DISABLE, TIM1_OUTPUTNSTATE_DISABLE, 0, TIM1_OCPOLARITY_HIGH, TIM1_OCNPOLARITY_HIGH, TIM1_OCIDLESTATE_RESET, TIM1_OCNIDLESTATE_RESET);
    
        // 当 TIM1_OCxInit 第四个参数为 0 时，那么初始的电平就不是低电平，必须通过以下函数再设置一次。
        if( GV_Tim1PwmoNumbers[i] == 56 ) TIM1_PWMO_SetDuty(56, 0);
        if( GV_Tim1PwmoNumbers[i] == 57 ) TIM1_PWMO_SetDuty(57, 0);
    }

    // 清空中断标志
    TIM1_ClearITPendingBit(TIM1_IT_UPDATE);
    TIM1_ClearFlag(TIM1_FLAG_UPDATE);
    
    // 启用中断函数：计数器值溢出更新
    TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE);
   
    // 允许主输出
    // 必须打开，否则无法输出PWM
    TIM1_CtrlPWMOutputs(ENABLE);

    // 开定时器
    TIM1_Cmd(ENABLE);

    // 输入捕获/比较输出使能
    // 以下代码和 TIM1_OCxInit() 中的第二个参数功能一致，一种用法是直接在 TIM1_OCxInit() 初始时直接设置为 ENABLE
    // 另外一种用法是在 TIM1_Cmd(ENABLE) 之后调用 TIM1_CCxCmd(.., ENABLE) 进行使能，因为 TIM1_OCxInit() 初始的时候直接设置为 ENABLE 的话，
    // 那么复位单片机时，TIM1 就可能输出极短的高电平（在一次电机项目中遇到过），如果项目对这个极短的高电平敏感，那么可以在 TIM1_Cmd(ENABLE) 之后设置 TIM1_CCxCmd(.., ENABLE)
    //TIM1_CCxCmd(TIM1_CHANNEL_1, ENABLE); 
    //TIM1_CCxCmd(TIM1_CHANNEL_3, ENABLE); 
}


/**
 * 设置 TIM1 通道的 PWM 正占空比
 *
 * @param short number  PCB上的编号：56, 57, TIM1_NUMBER_ALL
 * @param double duty   正占空比值
 */
void TIM1_PWMO_SetDuty( short number, double duty )
{
    // 初始参数
    if( duty < 0 ) duty = 0;
    if( duty > 100 ) duty = 100;
 
    // 由于使用了 TIM1_OCMODE_PWM2，那么 PWM 跳变前的将输出一个无效电平。
    // 又由于 TIM1_OCPOLARITY_HIGH 参数的设置：其指明了有效电平是高电平，无效电平是低电平。所以 PWM 跳变前的将输出低电平
    // 那么如果此时我们设置参数 duty = 73，实际上就输出了 73% 的低电平（负占空比）。正常我们说的占空比是正占空比。所以需要进行 100-duty 进行取反再设置
    duty = 100 - duty;
    
    // 设置占空比值
    if( number == TIM1_NUMBER_ALL || number == 56 ) TIM1_SetCompare1(((u32)(duty*GV_Tim1PwmoArr))/100);  // duty/100 = x/GV_Tim1PwmoArr  =>  x = duty*GV_Tim1PwmoArr/100
    if( number == TIM1_NUMBER_ALL || number == 57 ) TIM1_SetCompare3(((u32)(duty*GV_Tim1PwmoArr))/100);  // duty/100 = x/GV_Tim1PwmoArr  =>  x = duty*GV_Tim1PwmoArr/100
}


/**
 * 发送 TIM1 通道的 PWM 输出
 * 当设置了发送限制数量，那么程序会自动在 定时器更新中断 里检测并根据限制值来停止 定时器通道 输出使能
 *
 * 该函数调用后不会立即发送 PWM，而是初始相关的发送参数，最后在 定时器更新中断 里根据这些参数发送 PWM 脉冲
 * 主要原因：各个通道启动发送时，如果那时 TIM1_CNT 不为 0 时，那么就会导致PWM 第一个周期的波形异常，所以在 定时器更新中断 里启动发送就是确保 TIM1_CNT 为 0
 *
 * 函数调用后 GV_Tim1PwmoSendCountXX 立即设置为 1（输出使能启用前）：主要考虑到当程序进行下一步时，如果 GV_Tim1PwmoSendCountXX 为 0，那么就表示脉冲发送完毕。
 *
 * @param short  number     PCB上的编号：56, 57, TIM1_NUMBER_ALL
 * @param double duty       正占空比值：正数设置占空比，负数不设置
 * @param u32    sendLimit  脉冲发送个数限制：0 表示不限制
 * 
 * @return boolean  true 表示发送成功，false 表示发送失败
 */
bool TIM1_PWMO_Send( short number, double duty, u32 sendLimit )
{
    // PCB 上的编号 56 的端口 或 全部端口
    if( number == 56 || number == TIM1_NUMBER_ALL ){
        // 初始发送相关的变量
        // 虽然还没马上发送，但是此处直接设置发送计数为1（GV_Tim1PwmoSendCount56 = 1）
        // 主要考虑到程序的其他部分代码需要利用 GV_Tim1PwmoSendCount56 变量值进行判断是否正在发送等状态
        GV_Tim1PwmoSendSoon56 = 1;
        GV_Tim1PwmoSendSoon56Duty = duty;
        
        GV_Tim1PwmoSendCount56 = 1;
        GV_Tim1PwmoSendLimit56 = sendLimit;
        
        // 开启通道
        TIM1_CCxCmd(TIM1_CHANNEL_1, ENABLE); 
    }
    
    // PCB 上的编号 57 的端口 或 全部端口
    if( number == 57 || number == TIM1_NUMBER_ALL ){
        // 初始发送相关的变量
        // 虽然还没马上发送，但是此处直接设置发送计数为1（GV_Tim1PwmoSendCount57 = 1）
        // 主要考虑到程序的其他部分代码需要利用 GV_Tim1PwmoSendCount57 变量值进行判断是否正在发送等状态
        GV_Tim1PwmoSendSoon57 = 1;
        GV_Tim1PwmoSendSoon57Duty = duty;
        
        GV_Tim1PwmoSendCount57 = 1;
        GV_Tim1PwmoSendLimit57 = sendLimit;
                
        // 开启通道
        TIM1_CCxCmd(TIM1_CHANNEL_3, ENABLE); 
    }

    // 返回
    return true;
}


/**
 * 终止 TIM1 通道的 PWM 输出
 * 该函数主要利用将 正占空比设置为0（低电平） 来模拟 停止定时器通道
 *
 * @param short number  PCB上的编号：56, 57, TIM1_NUMBER_ALL
 */
void TIM1_PWMO_Stop( short number )
{
    // 关闭 TIM1 CH1 通道
    if( number == 56 || number == TIM1_NUMBER_ALL ){
        TIM1_PWMO_SetDuty(56, 0);
        // TIM1_CCxCmd(TIM1_CHANNEL_1, DISABLE); 

        GV_Tim1PwmoSendLimit56 = GV_Tim1PwmoSendCount56 = 0; 
        GV_Tim1PwmoSendSoon56 = 0; GV_Tim1PwmoSendSoon56Duty = 0;
    }
    
    // 关闭 TIM1 CH2 通道
    if( number == 57 || number == TIM1_NUMBER_ALL ){ 
        TIM1_PWMO_SetDuty(57, 0);
        // TIM1_CCxCmd(TIM1_CHANNEL_3, DISABLE); 
        
        GV_Tim1PwmoSendLimit57 = GV_Tim1PwmoSendCount57 = 0;
        GV_Tim1PwmoSendSoon57 = 0; GV_Tim1PwmoSendSoon57Duty = 0;
    }
}



/* -------------------------------------------------- */
// ● 函数主体（PWM 输入捕获）
// ● PCB 线路板上 TIM1 输出编号为 56和57
// ● 考虑到不同通道输出/捕获时都同时设置 TIM1_TimeBaseInit 函数，会相互干扰，所以使用 TIM1 就启用一个功能：PWM输出 或 PWM输入捕获
/* -------------------------------------------------- */

/**
 * 初始化 TIM1 PWM 捕获
 */
void TIM1_PWMI_Init()
{
    // 重置初始化
    TIM1_DeInit();
    
    // 初始化 TIM1 时基单元
    // 16预分频，向上计数，每 1s/1M = 1us 计数器值加 1（累加 1000 次需要 1 毫秒）
    // 参数说明：预分配值，计数模式，自动重装载值（计数器值到 65536 后重新装载），计数器起始值（计数器值重新装载后从0开始）
    TIM1_TimeBaseInit(15, TIM1_COUNTERMODE_UP, 65536-1, 0);

    // 初始化 TIM1 通道
    for( int i=0; i < ARR_LENGTH(GV_Tim1PwmiNumbers); i++ ){
        if( GV_Tim1PwmiNumbers[i] == 56 ) TIM1_PWMI_Init56();
        if( GV_Tim1PwmiNumbers[i] == 57 ) TIM1_PWMI_Init57();
    }

}
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
    TIM1_PWMIConfig(TIM1_CHANNEL_1, TIM1_ICPOLARITY_RISING, TIM1_ICSELECTION_DIRECTTI, TIM1_ICPSC_DIV1, 0x02);

    // 输入捕获/比较输出使能
    //【经过实际测试，以下使能代码可以注释掉，但是为了安心还是加上去】
    TIM1_CCxCmd(TIM1_CHANNEL_1, ENABLE);
    TIM1_CCxCmd(TIM1_CHANNEL_2, ENABLE);
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
    TIM1_ICInit(TIM1_CHANNEL_4, TIM1_ICPOLARITY_FALLING, TIM1_ICSELECTION_INDIRECTTI, TIM1_ICPSC_DIV1, 0x02);

    // 输入捕获/比较输出使能
    //【经过实际测试，以下使能代码可以注释掉，但是为了安心还是加上去】
    TIM1_CCxCmd(TIM1_CHANNEL_3, ENABLE);
    TIM1_CCxCmd(TIM1_CHANNEL_4, ENABLE);
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
 * 开始 TIM1 PWM 捕获
 */
void TIM1_PWMI_Start()
{
    // 清空俘获中断标志
    TIM1_ClearITPendingBit(TIM1_IT_CC1);
    TIM1_ClearFlag(TIM1_FLAG_CC1);

    TIM1_ClearITPendingBit(TIM1_IT_CC3);
    TIM1_ClearFlag(TIM1_FLAG_CC3);

    // 启用中断函数：俘获比较通道CH1中断
    TIM1_ITConfig(TIM1_IT_CC1, ENABLE);
    TIM1_ITConfig(TIM1_IT_CC3, ENABLE);

    // 开定时器
    //【经过实际测试，定时器就算未开启，上面的 TIM1_IT_CC1 中断仍正常进入，但是获取的计数器值不确定】
    TIM1_Cmd(ENABLE);
}


/**
 * 终止 TIM1 PWM 捕获
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
 * 获取 TIM1 捕获的 PWM 频率
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
 * 获取 TIM1 捕获的 PWM 正占空比
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
    if( TIM1_GetITStatus(TIM1_IT_UPDATE) ) TIM1_InterruptHandlerUpdate();
    
    // 中断：PWM 捕获
    // 对于 PWM = 0 或者 100 的捕获有问题，因为这两个 PWM 值都无法触发中断
    if( TIM1_GetITStatus(TIM1_IT_CC1) != RESET ) TIM1_InterruptHandlerCapturePwm56();
    if( TIM1_GetITStatus(TIM1_IT_CC3) != RESET ) TIM1_InterruptHandlerCapturePwm57();
}


/**
 * TIM1 中断：计数器值溢出更新
 */
void TIM1_InterruptHandlerUpdate()
{
    // 无即将发送的脉冲 且 启用发送限制
    if( GV_Tim1PwmoSendSoon56 == 0 && GV_Tim1PwmoSendLimit56 > 0 ){
        if( GV_Tim1PwmoSendCount56++ >= GV_Tim1PwmoSendLimit56 ) TIM1_PWMO_Stop(56);  // 如果发送个数超过限制个数，那么暂停输出
    }
    if( GV_Tim1PwmoSendSoon57 == 0 && GV_Tim1PwmoSendLimit57 > 0 ){
        if( GV_Tim1PwmoSendCount57++ >= GV_Tim1PwmoSendLimit57 ) TIM1_PWMO_Stop(57);  // 如果发送个数超过限制个数，那么暂停输出
    }

    // 有即将发送的脉冲
    if( GV_Tim1PwmoSendSoon56 == 1 ){
        if( GV_Tim1PwmoSendSoon56Duty >= 0 ) TIM1_PWMO_SetDuty(56, GV_Tim1PwmoSendSoon56Duty);
        GV_Tim1PwmoSendSoon56 = 0; GV_Tim1PwmoSendSoon56Duty = 0;
    }
    if( GV_Tim1PwmoSendSoon57 == 1 ){
        if( GV_Tim1PwmoSendSoon57Duty >= 0 ) TIM1_PWMO_SetDuty(57, GV_Tim1PwmoSendSoon57Duty);
        GV_Tim1PwmoSendSoon57 = 0; GV_Tim1PwmoSendSoon57Duty = 0;
    }
    
    // 清空中断标志
    TIM1_ClearITPendingBit(TIM1_IT_UPDATE);
    TIM1_ClearFlag(TIM1_FLAG_UPDATE);
}


/**
 * TIM1 中断：PWM 捕获（J56接口）
 */
void TIM1_InterruptHandlerCapturePwm56()
{
    // 初始化定时器输入捕获值
    // 分别对应一个PWM周期的第一个上升沿，第一个下降沿，第二个上升沿
    static u16 icValueA, icValueB, icValueC;

    // 获取定时器输入捕获值
    if( icValueA == 0 ){
        icValueA = TIM1_GetCapture1();
    }else{
        icValueB = TIM1_GetCapture2();
        icValueC = TIM1_GetCapture1();

        // 处理定时器捕获值溢出情况
        if( icValueC < icValueA ) icValueC = icValueC-icValueA+65535;
        else icValueC = icValueC - icValueA;
 
        if( icValueB < icValueA ) icValueB = icValueB-icValueA+65535;
        else icValueB = icValueB - icValueA;

        // 计算频率和周期
        // 这里的 1000000 是基于初始化函数中的系统时钟设置（16预分频 = 1000000）
        // 这里的 100.0 小数点表示将表达式 icValueB*100.0/icValueC 的过程和结果以浮点数来进行
        GV_Tim1PwmiFreq56 = 1000000/icValueC;
        GV_Tim1PwmiDuty56 = icValueB*100.0/icValueC;
        
        // 占空比补偿
        // 按照理论，icValueC 是肯定大于 icValueB，但是实际测试中，当捕获占空比 <= 1 的 PWM 时，icValueB 会大于 icValueC（这一点已经背离了正常逻辑）
        // 当 icValueB > icValueC，上面的代码计算出来的占空比结果会 > 100，于是我们对其进行了 求余 计算，实测发现占空比 <= 1 的就能正确了
        GV_Tim1PwmiDuty56 = GV_Tim1PwmiDuty56 > 100 ? GV_Tim1PwmiDuty56-100.0 : GV_Tim1PwmiDuty56;

        // 重置定时器输入捕获值
        icValueA = icValueB = icValueC = 0;
    }

    // 清除中断标志位
    TIM1_ClearITPendingBit(TIM1_IT_CC1);    
    TIM1_ClearFlag(TIM1_FLAG_CC1);
}


/**
 * TIM1 中断：PWM 捕获（J57接口）
 */
void TIM1_InterruptHandlerCapturePwm57()
{
    // 初始化定时器输入捕获值
    // 分别对应一个PWM周期的第一个上升沿，第一个下降沿，第二个上升沿
    static u16 icValueA, icValueB, icValueC;

    // 获取定时器输入捕获值
    if( icValueA == 0 ){
        icValueA = TIM1_GetCapture3();
    }else{
        icValueB = TIM1_GetCapture4();
        icValueC = TIM1_GetCapture3();
        
        // 处理定时器捕获值溢出情况
        if( icValueC < icValueA ) icValueC = icValueC-icValueA+65535;
        else icValueC = icValueC - icValueA;
 
        if( icValueB < icValueA ) icValueB = icValueB-icValueA+65535;
        else icValueB = icValueB - icValueA;

        // 计算频率和周期
        // 这里的 1000000 是基于初始化函数中的系统时钟设置（16预分频 = 1000000）
        // 这里的 100.0 小数点表示将表达式 icValueB*100.0/icValueC 的过程和结果以浮点数来进行
        GV_Tim1PwmiFreq57 = 1000000/icValueC;
        GV_Tim1PwmiDuty57 = icValueB*100.0/icValueC;
        
        // 占空比补偿
        // 按照理论，icValueC 是肯定大于 icValueB，但是实际测试中，当捕获占空比 <= 1 的 PWM 时，icValueB 会大于 icValueC（这一点已经背离了正常逻辑）
        // 当 icValueB > icValueC，上面的代码计算出来的占空比结果会 > 100，于是我们对其进行了 求余 计算，实测发现占空比 <= 1 的就能正确了
        GV_Tim1PwmiDuty57 = GV_Tim1PwmiDuty57 > 100 ? GV_Tim1PwmiDuty57-100.0 : GV_Tim1PwmiDuty57;

        // 重置定时器输入捕获值
        icValueA = icValueB = icValueC = 0;
    }

    // 清除中断标志位
    TIM1_ClearITPendingBit(TIM1_IT_CC3);    
    TIM1_ClearFlag(TIM1_FLAG_CC3);
}