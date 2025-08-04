#include "common.h"



/* -------------------------------------------------- */
// ● 全局变量
/* -------------------------------------------------- */

/**
 * ADC2 基础变量
 */
short GV_Adc2Numbers[] = {68,69,90,91,92,93};   //【按需设置】ADC2 接口编号集，只有将 ADC2 接口编号加入到这个变量后才能实现对应 ADC2 接口的相关功能


/**
 * ADC2 采样相关变量
 */
short GV_Adc2SamplingAvgCount  = 5;  // ADC2 采样平均值的数量，数量越大平均值越准确
float GV_Adc2SamplingBaseVolt  = 5;  // ADC2 采样的基准电压



/* -------------------------------------------------- */
// ● 函数主体
/* -------------------------------------------------- */

/**
 * 获取 ADC2 接口对应芯片上的针脚号
 *
 * @param short number  PCB 上的 ADC2 接口编号
 * @return GPIO_Pin_TypeDef  无效参数时返回 GPIO_PIN_ALL
 */
GPIO_Pin_TypeDef ADC2_GetPin( short number )
{
    // 适配针脚
    if( number == 68 ) return GPIO_PIN_0;
    if( number == 69 ) return GPIO_PIN_7;
    if( number == 90 ) return GPIO_PIN_5;
    if( number == 91 ) return GPIO_PIN_4;
    if( number == 92 ) return GPIO_PIN_3;
    if( number == 93 ) return GPIO_PIN_2;

    // 无效返回
    return GPIO_PIN_ALL;
}


/**
 * 获取 ADC2 接口对应芯片上的端口号
 *
 * @param short number  PCB 上的 ADC2 接口编号
 * @return GPIO_TypeDef*  无效参数时返回 0
 */
GPIO_TypeDef* ADC2_GetPort( short number )
{
    // 适配端口
    if( number == 68 ) return GPIOB;
    if( number == 69 ) return GPIOE;
    if( number == 90 ) return GPIOB;
    if( number == 91 ) return GPIOB;
    if( number == 92 ) return GPIOB;
    if( number == 93 ) return GPIOB;
    
    // 无效返回
    return (GPIO_TypeDef*)0x0000;
}


/**
 * 获取 ADC2 接口对应芯片上的通道号
 *
 * @param short number  PCB 上的 ADC2 接口编号
 * @return ADC2_Channel_TypeDef  无效参数时返回 0xFF
 */
ADC2_Channel_TypeDef ADC2_GetChannel( short number )
{
    // 适配端口
    if( number == 68 ) return ADC2_CHANNEL_0;
    if( number == 69 ) return ADC2_CHANNEL_8;
    if( number == 90 ) return ADC2_CHANNEL_5;
    if( number == 91 ) return ADC2_CHANNEL_4;
    if( number == 92 ) return ADC2_CHANNEL_3;
    if( number == 93 ) return ADC2_CHANNEL_2;

    // 无效返回
    return (ADC2_Channel_TypeDef)0xFF;
}


/**
 * 获取 ADC2 接口对应芯片上的施密特通道号
 *
 * @param short number  PCB 上的 ADC2 接口编号
 * @return ADC2_SchmittTrigg_TypeDef  无效参数时返回 0xFF
 */
ADC2_SchmittTrigg_TypeDef ADC2_GetSchmittChannel( short number )
{
    // 适配端口
    if( number == 68 ) return ADC2_SCHMITTTRIG_CHANNEL0;
    if( number == 69 ) return ADC2_SCHMITTTRIG_CHANNEL8;
    if( number == 90 ) return ADC2_SCHMITTTRIG_CHANNEL5;
    if( number == 91 ) return ADC2_SCHMITTTRIG_CHANNEL4;
    if( number == 92 ) return ADC2_SCHMITTTRIG_CHANNEL3;
    if( number == 93 ) return ADC2_SCHMITTTRIG_CHANNEL2;
    
    // 无效返回
    return (ADC2_SchmittTrigg_TypeDef)0xFF;
}


/**
 * ADC2 初始化
 */
void ADC2_CInit()
{
    // 初始化 ADC2 GPIO
    // GPIO_MODE_IN_FL_IT     浮动输入有中断功能
    // GPIO_MODE_IN_PU_IT     上拉输入有中断功能
    // GPIO_MODE_IN_FL_NO_IT  浮动输入无中断功能
    // GPIO_MODE_IN_PU_NO_IT  上拉输入无中断功能
    for( int i=0; i < ARR_LENGTH(GV_Adc2Numbers); i++ ){
        if( ADC2_GetPort(GV_Adc2Numbers[i]) == 0 ) continue;
        if( ADC2_GetPin(GV_Adc2Numbers[i]) == GPIO_PIN_ALL ) continue;
        GPIO_Init(ADC2_GetPort(GV_Adc2Numbers[i]), ADC2_GetPin(GV_Adc2Numbers[i]), GPIO_MODE_IN_FL_NO_IT);
    }
    
    // 启用 ADC2 功能
    ADC2_Cmd(ENABLE);
}


/**
 * 获取 ADC2 电压值
 *
 * @param number  PCB上的编号
 * @return float  电压值
 */
float ADC2_GetVoltValue( short number )
{
    // 无效参数
    if( ADC2_GetChannel(number) == 0xFF ) return ADC2_INVALID_VOLT;
    if( ADC2_GetSchmittChannel(number) == 0xFF ) return ADC2_INVALID_VOLT;
    
    // 初始化
    int adc2SamplingTotalValue = 0;
    
    // ADC2_Init 参数说明：
    // 　　1. 转换模式（单次或连续转换）
    // 　　2. 通道（0~9）
    // 　　3. 时钟分频器
    // 　　4. 外部触发转换事件
    // 　　5. 启用/关闭外部触发转换事件
    // 　　6. 数据对齐方向（左对齐或者右对齐）
    // 　　7. 指定通道的施密特触发器
    // 　　8. 启用或者关闭指定通道的施密特触发器
    //
    //        1                           2                        3                      4                 5        6                 7                               8
    ADC2_Init(ADC2_CONVERSIONMODE_SINGLE, ADC2_GetChannel(number), ADC2_PRESSEL_FCPU_D18, ADC2_EXTTRIG_TIM, DISABLE, ADC2_ALIGN_RIGHT, ADC2_GetSchmittChannel(number), DISABLE);

    // 获取 ADC2 采样值累加值
    for( int ii = 0; ii < GV_Adc2SamplingAvgCount; ii++ ){
        // 采集并累加
        ADC2_StartConversion();
        adc2SamplingTotalValue += ADC2_GetConversionValue(); 
        
        // 延迟，提高电压采集稳定性
        //delay_ms(10);
    }

    // 返回
    return (float)adc2SamplingTotalValue/GV_Adc2SamplingAvgCount/1024*GV_Adc2SamplingBaseVolt;
}



/* -------------------------------------------------- */
// ● 主逻辑代码
/* -------------------------------------------------- */

/**
 * 主逻辑：显示所有ADC并输出
 */
void ADC2_MainPrintAllAdcValue()
{
    // 初始化
    short i;
    float volts[10];
    
    // 采集数据集
    for( i=0; i < ARR_LENGTH(GV_Adc2Numbers); i++ ){
        volts[i] = ADC2_GetVoltValue(GV_Adc2Numbers[i]);
    }

    // 输出数据集
    for( i=0; i < ARR_LENGTH(GV_Adc2Numbers); i++ ){
        printf("J%d = ", GV_Adc2Numbers[i]);
        if( volts[i] == ADC2_INVALID_VOLT ) printf("Invalid Volt\n");
        else printf("%.2f V\n", volts[i]);
    }
    
    // 输出分割线
    printf("==========\n");
}