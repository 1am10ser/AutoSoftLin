#include "common.h"



/* -------------------------------------------------- */
// ● 全局变量
/* -------------------------------------------------- */

/**
 * MOTOR 外部变量
 * 以下变量定义在 motor.c 文件中
 */
extern bool  GV_Motor;


/**
 * 外部中断相关变量
 */

//【按需设置】外部中断编号集，只有将外部中断编号加入到这个变量后才能实现对应外部中断的相关功能
short GV_ExtiNumbers[] = {60, 61};   

//【系统使用】外部中断上一次的值集
short GV_ExtiPrevValues[] = {-1, -1};

//【按需设置】外部中断触发源：0 表示低电平触发，1表示高电平触发；该变量和 GV_ExtiNumbers 一一对应
//　　　　　　外部中断触发源的高低电平是由线路板上硬件决定的，所以具体需要参见PCB元器件和原理图来设置此处的值
short GV_ExtiTriggerSources[] = {0, 0};



/* -------------------------------------------------- */
// ● 函数主体
/* -------------------------------------------------- */

/**
 * 获取外部中断对应芯片上的针脚号
 *
 * @param short number  PCB 上的外部中断编号
 * @return GPIO_Pin_TypeDef  无效参数时返回 GPIO_PIN_ALL
 */
GPIO_Pin_TypeDef EXTI_GetPin( short number )
{
    // 适配针脚
    if( number == 60 ) return GPIO_PIN_0;
    if( number == 61 ) return GPIO_PIN_3;

    // 无效返回
    return GPIO_PIN_ALL;
}


/**
 * 获取外部中断对应芯片上的端口号
 *
 * @param short number  PCB 上的外部中断编号
 * @return GPIO_TypeDef*  无效参数时返回 0
 */
GPIO_TypeDef* EXTI_GetPort( short number )
{
    // 适配端口
    if( number == 60 ) return GPIOD;
    if( number == 61 ) return GPIOD;

    // 无效返回
    return (GPIO_TypeDef*)0x0000;
}


/**
 * 获取外部中断时的电压状态
 *
 * @param short number  PCB 上的外部中断编号
 * @return char  0表示低电平，1表示高电平，-1表示未知
 */
char EXTI_GetValue( short number )
{
    if( EXTI_GetPort(number) == 0 ) return -1;
    if( EXTI_GetPin(number) == GPIO_PIN_ALL ) return -1;
    return GPIO_ReadInputPin(EXTI_GetPort(number),EXTI_GetPin(number)) == RESET ? 0 : 1;
}


/**
 * 外部中断初始化
 */
void EXTI_Init()
{
    // 初始化外部中断对应的 GPIO 针脚
    for( int i=0; i < ARR_LENGTH(GV_ExtiNumbers); i++ ){
        if( EXTI_GetPort(GV_ExtiNumbers[i]) == 0 ) continue;
        if( EXTI_GetPin(GV_ExtiNumbers[i]) == GPIO_PIN_ALL ) continue;
        GPIO_Init(EXTI_GetPort(GV_ExtiNumbers[i]), EXTI_GetPin(GV_ExtiNumbers[i]), GV_ExtiTriggerSources[i]==1?GPIO_MODE_IN_FL_IT:GPIO_MODE_IN_PU_IT);
    }

    // 外部触发：上升沿和下降沿
    EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOD, EXTI_SENSITIVITY_RISE_FALL);  
}



/* -------------------------------------------------- */
// ● 中断回调函数
//   主调用入口参见 stm8s_it.c
/* -------------------------------------------------- */

/**
 * EXTI 中断
 */
void EXTI_InterruptHandler()
{
    // 初始化
    short value1, value2;
    
    // 遍历外部中断编号集
    for( int i=0; i < ARR_LENGTH(GV_ExtiNumbers); i++ ){
        if( EXTI_GetPort(GV_ExtiNumbers[i]) == 0 ) continue;
        if( EXTI_GetPin(GV_ExtiNumbers[i]) == GPIO_PIN_ALL ) continue;
        
        // 获取针脚值
        value1 = EXTI_GetValue(GV_ExtiNumbers[i]);

        // 低电平触发（下降沿）
        if( GV_ExtiTriggerSources[i] == 0 ){
            // 上一次值为-1或者1，并且当前值为0，那么触发下降沿
            if( (GV_ExtiPrevValues[i]==-1||GV_ExtiPrevValues[i]==1) && value1 == 0 ){
                // 消抖并获取IO口值
                if( EXTI_INT_JITTER ) delay_ms(EXTI_INT_JITTER); 
                value2 = EXTI_GetValue(GV_ExtiNumbers[i]);
                
                // 调用自定义函数
                if( value1 == value2 ) EXTI_InterruptHandlerFall(GV_ExtiNumbers[i]);
                else value1 = value2;
            }
        }
        
        // 高电平触发（上升沿）
        if( GV_ExtiTriggerSources[i] == 1 ){
            // 上一次值为-1或者0，并且当前值为1，那么触发上升沿
            if( (GV_ExtiPrevValues[i]==-1||GV_ExtiPrevValues[i]==0) && value1 == 1 ){
                // 消抖并获取IO口值
                if( EXTI_INT_JITTER ) delay_ms(EXTI_INT_JITTER); 
                value2 = EXTI_GetValue(GV_ExtiNumbers[i]);
                
                // 调用自定义函数
                if( value1 == value2 ) EXTI_InterruptHandlerRise(GV_ExtiNumbers[i]);
                else value1 = value2;
            }
        }
        
        // 设置上一次值
        GV_ExtiPrevValues[i] = value1;
    }
}


/**
 * EXTI 中断（下降沿）
 */
void EXTI_InterruptHandlerFall( short number )
{
    // 电机归零完成中断
    if( GV_Motor ) MOTOR_InterruptHandlerZeroFinish(number, 0);
    
    
    //【逻辑代码】 J60 下降沿
    if( number == 60 ){
        // ...
        //【下降沿】J60 端口变为低电平（GV_ExtiTriggerSources[0] == 0）后业务逻辑代码...
        // ...
    }
    
    
    //【逻辑代码】 J61 下降沿
    if( number == 61 ){
        // ...
        //【下降沿】J61 端口变为低电平（GV_ExtiTriggerSources[1] == 0）后业务逻辑代码...
        // ...
    }
}


/**
 * EXTI 中断（上升沿）
 */
void EXTI_InterruptHandlerRise( short number )
{
    // 电机归零完成中断
    if( GV_Motor ) MOTOR_InterruptHandlerZeroFinish(number, 1);
    

    //【逻辑代码】 J60 上升沿
    if( number == 60 ){
        // ...
        //【上升沿】J60 端口变为高电平（GV_ExtiTriggerSources[0] == 1）后业务逻辑代码...
        // ......
    }
    
    
    //【逻辑代码】 J61 上升沿
    if( number == 61 ){
        // ...
        //【上升沿】J61 端口变为高电平（GV_ExtiTriggerSources[1] == 1）后业务逻辑代码...
        // ...
    }
}