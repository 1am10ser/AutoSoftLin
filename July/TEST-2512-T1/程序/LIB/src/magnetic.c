#include "common.h"



/* -------------------------------------------------- */
// ● MAGNETIC 基于 ADC2 的 3 号编号
// ● 使用前务必先对 ADC2 进行初始化
/* -------------------------------------------------- */



/* -------------------------------------------------- */
// ● 全局变量
/* -------------------------------------------------- */

/**
 * MAGNETIC 基础变量
 */

//【系统使用】MAGNETIC 使用的 ADC2 编号 
short GV_MagneticADC2Num = 3;

//【系统使用】MAGNETIC 的 ADC2 采样电压值 
float GV_MagneticADC2Volt = 0;



/* -------------------------------------------------- */
// ● 函数主体
/* -------------------------------------------------- */

/**
 * 磁场感应初始化
 */
void MAGNETIC_Init()
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
  
    // 初始化
    GPIO_Init(MAGNETIC_LED_PORT, MAGNETIC_LED_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
}


/**
 * 采样磁场电压值并返回
 */
float MAGNETIC_Sampling()
{
    GV_MagneticADC2Volt = ADC2_GetVoltValue(GV_MagneticADC2Num);
    return GV_MagneticADC2Volt;
}


/**
 * 获取磁场磁极
 *
 * @param bool sampling  是否进行磁场电压值采样
 * @return char  N 表示北极，S 表示南极，* 表示未知或者磁极中心
 */
char MAGNETIC_GetMagnetic( bool sampling )
{
    // 磁场电压值采样
    if( sampling ) MAGNETIC_Sampling();

    // 初始无磁场时电压
    // 基于 WSH315 芯片的数据
    // 注：每颗芯片无磁场时可能不同，下面的 2.55 是采集于当前焊接在板子上芯片（同时适配器供电12V，如果烧录器5V供电，那么该值会不一样）
    float noMagneticVolt = 2.55;  // 无磁场时电压
    float noMagneticVoltBound = 0.05;  // 无磁场电压值误差
    
    // 磁极判断
    if( GV_MagneticADC2Volt > (noMagneticVolt+noMagneticVoltBound) ) return 'S';
    if( GV_MagneticADC2Volt < (noMagneticVolt-noMagneticVoltBound) ) return 'N';
    
    // 返回未知
    return '*';
}


/**
 * 获取磁场高斯
 *
 * @param bool sampling  是否进行磁场电压值采样
 * @return char  N 表示北极，S 表示南极，* 表示未知或者磁极中心
 */
u16 MAGNETIC_GetMagneticGauss( bool sampling )
{
    // 磁场电压值采样
    if( sampling ) MAGNETIC_Sampling();

    // 每高斯变化对应的电压值
    float voltPerGauss = 0.0015;  // 1.5mV/Gauss
    
    // 初始无磁场时电压
    // 基于 WSH315 芯片的数据
    // 注：每颗芯片无磁场时可能不同，下面的 2.55 是采集于当前焊接在板子上芯片（同时适配器供电12V，如果烧录器5V供电，那么该值等于 2.64）
    float noMagneticVolt = 2.55;  // 无磁场时电压
    float noMagneticVoltBound = 0.05;  // 无磁场电压值误差
    
    // 磁极判断
    if( GV_MagneticADC2Volt > (noMagneticVolt+noMagneticVoltBound) ) return (u16)((GV_MagneticADC2Volt-noMagneticVolt)/voltPerGauss);
    if( GV_MagneticADC2Volt < (noMagneticVolt+noMagneticVoltBound) ) return (u16)((noMagneticVolt-GV_MagneticADC2Volt)/voltPerGauss);
    
    // 返回未知
    return 0;
}



/* -------------------------------------------------- */
// ● 主逻辑代码
/* -------------------------------------------------- */

/**
 * Magnetic 主逻辑：南北极显示对应的灯
 */
void MAGNETIC_MainMagneticShowLed()
{
    // 初始化
    char magnetic = MAGNETIC_GetMagnetic(true);
    static u8 unknowMagnetic = 0;
    
    // 磁极判断并点亮对应 LED 灯
    if( magnetic == 'N' ){ MAGNETIC_LED_N; unknowMagnetic = 0; return; }
    if( magnetic == 'S' ){ MAGNETIC_LED_S; unknowMagnetic = 0; return; }
    
    // 未知磁极 LED 灯闪烁
    if( unknowMagnetic++ > 200 ) unknowMagnetic = 1;
    if( unknowMagnetic%2 == 0 ){ MAGNETIC_LED_N; }else{ MAGNETIC_LED_S; }
}