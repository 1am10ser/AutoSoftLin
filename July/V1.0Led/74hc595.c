#include "common.h"



/* -------------------------------------------------- */
// ● 全局变量
/* -------------------------------------------------- */

/**
 * 74HC595 基础变量
 */

//【按需设置】74HC595 编号集，只有将 74HC595 编号加入到这个变量后才能实现对应 74HC595 的相关功能
short GV_HC595Numbers[] = {2};



/* -------------------------------------------------- */
// ● 函数主体
/* -------------------------------------------------- */

/**
 * 获取 74HC595 对应芯片上的针脚号（数据脚）
 *
 * @param short number  PCB 上的 74HC595 编号
 * @return GPIO_Pin_TypeDef  无效参数时返回 GPIO_PIN_ALL
 */
GPIO_Pin_TypeDef HC595_GetDataPin( short number )
{
    // 适配针脚
    if( number == 2 ) return GPIO_PIN_3;

    // 无效返回
    return GPIO_PIN_ALL;
}


/**
 * 获取 74HC595 对应芯片上的端口号（数据脚）
 *
 * @param short number  PCB 上的 74HC595 编号
 * @return GPIO_TypeDef*  无效参数时返回 0
 */
GPIO_TypeDef* HC595_GetDataPort( short number )
{
    // 适配端口
    if( number == 2 ) return GPIOA;

    // 无效返回
    return (GPIO_TypeDef*)0x0000;
}


/**
 * 获取 74HC595 对应芯片上的针脚号（移位时钟脚）
 *
 * @param short number  PCB 上的 74HC595 编号
 * @return GPIO_Pin_TypeDef  无效参数时返回 GPIO_PIN_ALL
 */
GPIO_Pin_TypeDef HC595_GetBitClockPin( short number )
{
    // 适配针脚
    if( number == 2 ) return GPIO_PIN_6;

    // 无效返回
    return GPIO_PIN_ALL;
}


/**
 * 获取 74HC595 对应芯片上的端口号（移位时钟脚）
 *
 * @param short number  PCB 上的 74HC595 编号
 * @return GPIO_TypeDef*  无效参数时返回 0
 */
GPIO_TypeDef* HC595_GetBitClockPort( short number )
{
    // 适配端口
    if( number == 2 ) return GPIOA;

    // 无效返回
    return (GPIO_TypeDef*)0x0000;
}


/**
 * 获取 74HC595 对应芯片上的针脚号（存储时钟脚）
 *
 * @param short number  PCB 上的 74HC595 编号
 * @return GPIO_Pin_TypeDef  无效参数时返回 GPIO_PIN_ALL
 */
GPIO_Pin_TypeDef HC595_GetStoreClockPin( short number )
{
    // 适配针脚
    if( number == 2 ) return GPIO_PIN_7;

    // 无效返回
    return GPIO_PIN_ALL;
}


/**
 * 获取 74HC595 对应芯片上的端口号（存储时钟脚）
 *
 * @param short number  PCB 上的 74HC595 编号
 * @return GPIO_TypeDef*  无效参数时返回 0
 */
GPIO_TypeDef* HC595_GetStoreClockPort( short number )
{
    // 适配端口
    if( number == 2 ) return GPIOD;

    // 无效返回
    return (GPIO_TypeDef*)0x0000;
}


/**
 * 74HC595 初始化
 */
void HC595_Init()
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

    // 初始化 74HC595 对应的 GPIO 针脚
    for( int i=0; i < ARR_LENGTH(GV_HC595Numbers); i++ ){
        // 数据脚
        if( HC595_GetDataPort(GV_HC595Numbers[i]) == 0 ) continue;
        if( HC595_GetDataPin(GV_HC595Numbers[i]) == GPIO_PIN_ALL ) continue;
        GPIO_Init(HC595_GetDataPort(GV_HC595Numbers[i]), HC595_GetDataPin(GV_HC595Numbers[i]), GPIO_MODE_OUT_PP_LOW_SLOW);
        
        // 移位时钟脚
        if( HC595_GetBitClockPort(GV_HC595Numbers[i]) == 0 ) continue;
        if( HC595_GetBitClockPin(GV_HC595Numbers[i]) == GPIO_PIN_ALL ) continue;
        GPIO_Init(HC595_GetBitClockPort(GV_HC595Numbers[i]), HC595_GetBitClockPin(GV_HC595Numbers[i]), GPIO_MODE_OUT_PP_LOW_SLOW);
                
        // 存储时钟脚
        if( HC595_GetStoreClockPort(GV_HC595Numbers[i]) == 0 ) continue;
        if( HC595_GetStoreClockPin(GV_HC595Numbers[i]) == GPIO_PIN_ALL ) continue;
        GPIO_Init(HC595_GetStoreClockPort(GV_HC595Numbers[i]), HC595_GetStoreClockPin(GV_HC595Numbers[i]), GPIO_MODE_OUT_PP_LOW_SLOW);
    }
}


/**
 * 74HC595 发送数据
 * 该函数为 74HC595 发送字节的程序,该程序先发送的是最低位
 * 备注：发送一个字节，先输入数据位最终将在Q7口输出，最后输入的数据位将在Q0口输出
 *
 * 当有多片 74HC595 串联时，只有多次调用该发送程序，先发的字节将会自动溢出到下一个 74HC595
 * 由于 74HC595 的锁存机制，所以通过该函数发送数据后并不会马上输出数据到并行口，需要调用 HC595_SendOutput() 进行输出
 */
void HC595_SendByte( short number, u8 byte )
{
    // 初始化
    u8 i, dat;
    
    // 初始化
    GPIO_TypeDef* dataPort = HC595_GetDataPort(number);
    if( dataPort == 0 ) return;

    GPIO_Pin_TypeDef dataPin = HC595_GetDataPin(number);
    if( dataPin == GPIO_PIN_ALL ) return;

    GPIO_TypeDef* clockPort = HC595_GetBitClockPort(number);
    if( clockPort == 0 ) return;

    GPIO_Pin_TypeDef clockPin = HC595_GetBitClockPin(number);
    if( clockPin == GPIO_PIN_ALL ) return;

    // 发送字节数据
    for( i = 0; i < 8; i++ ){
        // 取字节最低位
        dat = byte & 0x01; 
    
        // 发送字节位
        // 时钟脚上升沿一次，74HC595 会从其数据脚读取一位数据
        GPIO_WriteLow(clockPort, clockPin);
        dat ? GPIO_WriteHigh(dataPort, dataPin) : GPIO_WriteLow(dataPort, dataPin);
        GPIO_WriteHigh(clockPort, clockPin);
        

        // 将发送的字节右移1位
        byte >>= 1;
    }
}


/**
 * 74HC595 发送数据
 * 由于 74HC595 的锁存机制，所以通过 HC595_SendByte() 函数发送数据后并不会马上输出数据到并行口，需要调用该函数进行输出
 */
void HC595_SendOutput( short number )
{
    // 初始化
    GPIO_TypeDef* clockPort = HC595_GetStoreClockPort(number);
    if( clockPort == 0 ) return;

    GPIO_Pin_TypeDef clockPin = HC595_GetStoreClockPin(number);
    if( clockPin == GPIO_PIN_ALL ) return;

    // 输出数据
    // 74HC595 第 13（OE）脚为低电平时，然后引脚 12（STCP）产生一个上升沿后就会输出
    GPIO_WriteLow(clockPort, clockPin);
    GPIO_WriteHigh(clockPort, clockPin);
    
    
}






