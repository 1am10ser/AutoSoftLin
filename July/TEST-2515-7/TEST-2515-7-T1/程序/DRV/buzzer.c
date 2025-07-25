#include "common.h"



/* -------------------------------------------------- */
// ● 全局变量
/* -------------------------------------------------- */

/**
 * 蜂鸣器相关变量
 */

//【按需设置】蜂鸣器 编号集，只有将 蜂鸣器 编号加入到这个变量后才能实现对应蜂鸣的相关功能
short GV_BuzzerNumbers[] = {1};



/* -------------------------------------------------- */
// ● 函数主体
/* -------------------------------------------------- */

/**
 * 获取蜂鸣器对应芯片上的针脚号
 *
 * @param short number  PCB 上的蜂鸣器编号
 * @return GPIO_Pin_TypeDef  无效参数时返回 GPIO_PIN_ALL
 */
GPIO_Pin_TypeDef BUZZER_GetPin( short number )
{
    // 适配针脚
    if( number == 1 ) return GPIO_PIN_1;

    // 无效返回
    return GPIO_PIN_ALL;
}


/**
 * 获取蜂鸣器对应芯片上的端口号
 *
 * @param short number  PCB 上的蜂鸣器编号
 * @return GPIO_TypeDef*  无效参数时返回 0
 */
GPIO_TypeDef* BUZZER_GetPort( short number )
{
    // 适配端口
    if( number == 1 ) return GPIOD;

    // 无效返回
    return (GPIO_TypeDef*)0x0000;
}

    
/**
 * 蜂鸣器初始化
 */
void BUZZER_Init()
{  
    // GPIO_MODE_OUT_OD_LOW_SLOW   低速开漏输出低电平
    // GPIO_MODE_OUT_PP_LOW_SLOW   低速推挽输出低电平
    // GPIO_MODE_OUT_OD_HIZ_SLOW   低速开漏输出高阻态
    // GPIO_MODE_OUT_PP_HIGH_SLOW  低速推挽输出高电平
    
    // 推挽输出：针脚对应的芯片内部集成 一个NPN三极管(集电极C端内接V+) 连 一个PNP三极管(集电极C端内接V-)
    // 　　　推：电流走NPN三极管并从发射端E端作为输出口（V+）
    // 　　　挽：电流走PNP三极管并从集电极C端作为输出口（V-）
    // 推挽输出的大致原理应是如此，STM芯片里面可能没有 V- 负电压而是 0V，所以低电平就是 0V，仅个人理解
    // 推挽输出可以直接输出高电平而无需接上拉电阻和电源就可以驱动负载
    
    // 开漏输出：针脚对应的芯片内部集成一个NPN三极管，集电极C端什么都不接，直接作为输出端口
    // 开漏输出只能输出低电平和高组态，所以必须外接上拉电阻和电源才能输出高电平去驱动负载，电压由外接电源决定
  
    // 初始化蜂鸣器对应的 GPIO 针脚
    for( int i=0; i < ARR_LENGTH(GV_BuzzerNumbers); i++ ){
        if( BUZZER_GetPort(GV_BuzzerNumbers[i]) == 0 ) continue;
        if( BUZZER_GetPin(GV_BuzzerNumbers[i]) == GPIO_PIN_ALL ) continue;
        GPIO_Init(BUZZER_GetPort(GV_BuzzerNumbers[i]), BUZZER_GetPin(GV_BuzzerNumbers[i]), GPIO_MODE_OUT_PP_LOW_SLOW);
    }
}


/**
 * 蜂鸣器发声
 *
 * @param number  PCB上的编号
 */
void BUZZER_Open( short number )
{
    // 初始化
    GPIO_TypeDef* GPIO_Port = BUZZER_GetPort(number);
    if( GPIO_Port == 0 ) return;

    GPIO_Pin_TypeDef GPIO_Pin = BUZZER_GetPin(number);
    if( GPIO_Pin == GPIO_PIN_ALL ) return;

    // 蜂鸣器发声
    GPIO_WriteHigh(GPIO_Port, GPIO_Pin);
}


/**
 * 蜂鸣器全发声
 */
void BUZZER_OpenAll()
{
    for( int i=0; i < ARR_LENGTH(GV_BuzzerNumbers); i++ ) BUZZER_Open(GV_BuzzerNumbers[i]);
}


/**
 * 蜂鸣器静音
 *
 * @param number  PCB上的编号
 */
void BUZZER_Close( short number )
{
    // 初始化
    GPIO_TypeDef* GPIO_Port = BUZZER_GetPort(number);
    if( GPIO_Port == 0 ) return;

    GPIO_Pin_TypeDef GPIO_Pin = BUZZER_GetPin(number);
    if( GPIO_Pin == GPIO_PIN_ALL ) return;

    // 蜂鸣器静音
    GPIO_WriteLow(GPIO_Port, GPIO_Pin);
}


/**
 * 蜂鸣器全静音
 */
void BUZZER_CloseAll()
{
    for( int i=0; i < ARR_LENGTH(GV_BuzzerNumbers); i++ ) BUZZER_Close(GV_BuzzerNumbers[i]);
}


/**
 * 蜂鸣器发声或者静音切换
 *
 * @param number  PCB上的编号
 */
void BUZZER_Reverse( short number )
{
    // 初始化
    GPIO_TypeDef* GPIO_Port = BUZZER_GetPort(number);
    if( GPIO_Port == 0 ) return;

    GPIO_Pin_TypeDef GPIO_Pin = BUZZER_GetPin(number);
    if( GPIO_Pin == GPIO_PIN_ALL ) return;

    // 切换蜂鸣器
    GPIO_WriteReverse(GPIO_Port, GPIO_Pin);
}


/**
 * 蜂鸣器点亮或者熄灭全切换
 */
void BUZZER_ReverseAll()
{
    for( int i=0; i < ARR_LENGTH(GV_BuzzerNumbers); i++ ) BUZZER_Reverse(GV_BuzzerNumbers[i]);
}