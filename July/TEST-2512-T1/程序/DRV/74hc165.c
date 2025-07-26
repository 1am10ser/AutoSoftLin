#include "common.h"



/* -------------------------------------------------- */
// ● 全局变量
/* -------------------------------------------------- */

/**
 * 74HC165 基础变量
 */

//【按需设置】74HC165 编号集，只有将 74HC165 编号加入到这个变量后才能实现对应 74HC165 的相关功能
//　　　　　　由于 U10,U11,U12,U13 是级联的，所以填写 10 即可启用，无需要填写 11,12,13 这三个数字
short GV_HC165Numbers[] = {10,16};

//【按需设置】74HC165 初始化数据值集
//　　　　　　数组数据对应芯片脚数据：U16:D7~D0
//　　　　　　数组数据对应芯片脚数据：U10:D7~D0, U11:D7~D0, U12:D7~D0, U13:D7~D0
short GV_HC165DataValues[] = {0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0};



/* -------------------------------------------------- */
// ● 函数主体
/* -------------------------------------------------- */

/**
 * 74HC165 初始化
 */
void HC165_Init()
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

    // 初始化 74HC165 对应的 GPIO 针脚
    for( int i=0; i < ARR_LENGTH(GV_HC165Numbers); i++ ){
        // 时钟脚
        if( HC165_GetClockPort(GV_HC165Numbers[i]) == 0 ) continue;
        if( HC165_GetClockPin(GV_HC165Numbers[i]) == GPIO_PIN_ALL ) continue;
        GPIO_Init(HC165_GetClockPort(GV_HC165Numbers[i]), HC165_GetClockPin(GV_HC165Numbers[i]), GPIO_MODE_OUT_PP_LOW_SLOW);

        // 数据脚
        if( HC165_GetDataPort(GV_HC165Numbers[i]) == 0 ) continue;
        if( HC165_GetDataPin(GV_HC165Numbers[i]) == GPIO_PIN_ALL ) continue;
        GPIO_Init(HC165_GetDataPort(GV_HC165Numbers[i]), HC165_GetDataPin(GV_HC165Numbers[i]), GPIO_MODE_IN_PU_NO_IT);

        // 数据装载脚
        if( HC165_GetLoadPort(GV_HC165Numbers[i]) == 0 ) continue;
        if( HC165_GetLoadPin(GV_HC165Numbers[i]) == GPIO_PIN_ALL ) continue;
        GPIO_Init(HC165_GetLoadPort(GV_HC165Numbers[i]), HC165_GetLoadPin(GV_HC165Numbers[i]), GPIO_MODE_OUT_PP_LOW_SLOW);
    }
}


/**
 * 获取 74HC165 对应芯片上要处理的位码总数
 *
 * @param short number  PCB 上的 74HC165 编号
 * @return GPIO_Pin_TypeDef  无效参数时返回 GPIO_PIN_ALL
 */
short HC165_GetBitTotal( short number )
{
    // 适配针脚
    if( number == 10 ) return 32;
    if( number == 16 ) return 8;
    
    // 无效返回
    return 0;
}


/**
 * 获取 74HC165 对应芯片上的针脚号（数据脚 Q7）
 *
 * @param short number  PCB 上的 74HC165 编号
 * @return GPIO_Pin_TypeDef  无效参数时返回 GPIO_PIN_ALL
 */
GPIO_Pin_TypeDef HC165_GetDataPin( short number )
{
    // 适配针脚
    if( number == 10 ) return GPIO_PIN_7;
    if( number == 16 ) return GPIO_PIN_5;

    // 无效返回
    return GPIO_PIN_ALL;
}


/**
 * 获取 74HC165 对应芯片上的端口号（数据脚 Q7）
 *
 * @param short number  PCB 上的 74HC165 编号
 * @return GPIO_TypeDef*  无效参数时返回 0
 */
GPIO_TypeDef* HC165_GetDataPort( short number )
{
    // 适配端口
    if( number == 10 ) return GPIOB;
    if( number == 16 ) return GPIOE;

    // 无效返回
    return (GPIO_TypeDef*)0x0000;
}


/**
 * 获取 74HC165 对应芯片上的针脚号（数据装载脚 /PL）
 *
 * @param short number  PCB 上的 74HC165 编号
 * @return GPIO_Pin_TypeDef  无效参数时返回 GPIO_PIN_ALL
 */
GPIO_Pin_TypeDef HC165_GetLoadPin( short number )
{
    // 适配针脚
    if( number == 10 ) return GPIO_PIN_1;
    if( number == 16 ) return GPIO_PIN_0;

    // 无效返回
    return GPIO_PIN_ALL;
}


/**
 * 获取 74HC165 对应芯片上的端口号（数据装载脚 /PL）
 * 
 * @param short number  PCB 上的 74HC165 编号
 * @return GPIO_TypeDef*  无效参数时返回 0
 */
GPIO_TypeDef* HC165_GetLoadPort( short number )
{
    // 适配端口
    if( number == 10 ) return GPIOB;
    if( number == 16 ) return GPIOB;

    // 无效返回
    return (GPIO_TypeDef*)0x0000;
}


/**
 * 获取 74HC165 对应芯片上的针脚号（时钟脚 CP）
 *
 * @param short number  PCB 上的 74HC165 编号
 * @return GPIO_Pin_TypeDef  无效参数时返回 GPIO_PIN_ALL
 */
GPIO_Pin_TypeDef HC165_GetClockPin( short number )
{
    // 适配针脚
    if( number == 10 ) return GPIO_PIN_6;
    if( number == 16 ) return GPIO_PIN_7;

    // 无效返回
    return GPIO_PIN_ALL;
}


/**
 * 获取 74HC165 对应芯片上的端口号（时钟脚 CP）
 *
 * @param short number  PCB 上的 74HC165 编号
 * @return GPIO_TypeDef*  无效参数时返回 0
 */
GPIO_TypeDef* HC165_GetClockPort( short number )
{
    // 适配端口
    if( number == 10 ) return GPIOB;
    if( number == 16 ) return GPIOE;

    // 无效返回
    return (GPIO_TypeDef*)0x0000;
}


/**
 * 读取 74HC165 数据
 * 数据缓存到全局变量 GV_HC165DataValues[] 中
 *
 * @param short number  PCB 上的 74HC165 编号
 */
void HC165_ReadDataValues( short number )
{
    // 初始化
    GPIO_TypeDef* dataPort = HC165_GetDataPort(number);
    if( dataPort == 0 ) return;

    GPIO_Pin_TypeDef dataPin = HC165_GetDataPin(number);
    if( dataPin == GPIO_PIN_ALL ) return;
    
    GPIO_TypeDef* loadPort = HC165_GetLoadPort(number);
    if( loadPort == 0 ) return;

    GPIO_Pin_TypeDef loadPin = HC165_GetLoadPin(number);
    if( loadPin == GPIO_PIN_ALL ) return;

    GPIO_TypeDef* clockPort = HC165_GetClockPort(number);
    if( clockPort == 0 ) return;

    GPIO_Pin_TypeDef clockPin = HC165_GetClockPin(number);
    if( clockPin == GPIO_PIN_ALL ) return;
    
    // 拉低数据装载脚，使D0~D7端的数据进入移位寄存器
    GPIO_WriteLow(loadPort, loadPin);
    
    // 拉高数据装载脚，锁住移位寄存器中的数据
    // 当处于高电平时，首个 74HC165 的 Q7 数据输出脚指向 D7
    // 当处于高电平时，下一级的数据从DS引脚串行进入寄存器（配合时钟脚使用：时钟脚来一个上升沿，那么向右移动一位，例如：D6数据移到D7、D5数据移动到D6...，实现级联）
    GPIO_WriteHigh(loadPort, loadPin);
    
    // 读取数据集
    for( u8 i = 0; i < HC165_GetBitTotal(number); i++ ){
        // 读取数据，读取高位 D7
        GV_HC165DataValues[i] = GPIO_ReadInputPin(dataPort, dataPin) == RESET ? 0 : 1;

        // 时钟脚产生一个上升沿，数据右移动一位，例如：D7数据消失、D6数据移到D7、D5数据移动到D6...（实现级联）
        GPIO_WriteLow(clockPort, clockPin);
        GPIO_WriteHigh(clockPort, clockPin);
    }
}


