#include "common.h"



/* -------------------------------------------------- */
// ● LED 采用 74HC595 控制
// ● 使用前务必先对 74HC595 进行初始化
/* -------------------------------------------------- */



/* -------------------------------------------------- */
// ● 全局变量
/* -------------------------------------------------- */

/**
 * LED 基础变量
 */

//【按需设置】LED 灯编号集，只有将 LED 灯编号加入到这个变量后才能实现对应 LED 灯的相关功能
short GV_LedNumbers[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32};   

//【系统使用】LED 初始化灯状态集，该数组长度固定，将按照倒序发送给 74HC595 级联，最终数组数据按数组下标索引顺序映射到：U2:Q0~Q7，U3:Q0~Q7，U4:Q0~Q7，U5:Q0~Q7
//【系统固值】LED 灯编号映射灯状态集的索引，由于灯编号和灯状态没有直接的对应关系，所以通过该变量进行关联。“数组索引+1”即为 LED 灯编号
short GV_LedStates[] = {0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0};  
const short GV_LedNumMapStateIndexs[] = {0,1,2,3,6,7,4,5,  8,9,10,11,12,13,14,15,  16,17,18,19,20,21,22,23,  24,25,26,27,28,29,30,31};  



/* -------------------------------------------------- */
// ● 函数主体
/* -------------------------------------------------- */

/**
 * LED 灯初始化
 */
void LED_Init( bool render )
{
    // 初始化 LED 状态
    for( int i=0; i < ARR_LENGTH(GV_LedStates); i++ ) GV_LedStates[i] = 0;
    if( render ) LED_Render();
}


/**
 * LED 灯点亮
 *
 * @param short number  PCB上的编号
 * @param bool  render  是否渲染
 */
void LED_Open( short number, bool render )
{
    for( int i=0; i < ARR_LENGTH(GV_LedNumbers); i++ ) if( GV_LedNumbers[i] == number ) GV_LedStates[GV_LedNumMapStateIndexs[GV_LedNumbers[i]-1]] = 1;
    if( render ) LED_Render();
}


/**
 * LED 灯全点亮
 *
 * @param bool render  是否渲染
 */
void LED_OpenAll( bool render )
{
    for( int i=0; i < ARR_LENGTH(GV_LedNumbers); i++ ) GV_LedStates[GV_LedNumMapStateIndexs[GV_LedNumbers[i]-1]] = 1;
    if( render ) LED_Render();
}


/**
 * LED 灯熄灭
 *
 * @param short number  PCB上的编号
 * @param bool  render  是否渲染
 */
void LED_Close( short number, bool render )
{
    for( int i=0; i < ARR_LENGTH(GV_LedNumbers); i++ ) if( GV_LedNumbers[i] == number ) GV_LedStates[GV_LedNumMapStateIndexs[GV_LedNumbers[i]-1]] = 0;
    if( render ) LED_Render();
}


/**
 * LED 灯全熄灭
 *
 * @param bool render  是否渲染
 */
void LED_CloseAll( bool render )
{
    for( int i=0; i < ARR_LENGTH(GV_LedNumbers); i++ ) GV_LedStates[GV_LedNumMapStateIndexs[GV_LedNumbers[i]-1]] = 0;
    if( render ) LED_Render();
}


/**
 * LED 灯点亮或者熄灭切换
 *
 * @param short number  PCB上的编号
 * @param bool  render  是否渲染
 */
void LED_Reverse( short number, bool render )
{
    for( int i=0; i < ARR_LENGTH(GV_LedNumbers); i++ ) if( GV_LedNumbers[i] == number ) GV_LedStates[GV_LedNumMapStateIndexs[GV_LedNumbers[i]-1]] = GV_LedStates[GV_LedNumMapStateIndexs[GV_LedNumbers[i]-1]] ? 0 : 1;
    if( render ) LED_Render();
}


/**
 * LED 灯点亮或者熄灭全切换
 *
 * @param bool render  是否渲染
 */
void LED_ReverseAll( bool render )
{
    for( int i=0; i < ARR_LENGTH(GV_LedNumbers); i++ ) GV_LedStates[GV_LedNumMapStateIndexs[GV_LedNumbers[i]-1]] = GV_LedStates[GV_LedNumMapStateIndexs[GV_LedNumbers[i]-1]] ? 0 : 1;
    if( render ) LED_Render();
}


/**
 * LED 渲染
 * 由于 LED 采用 74HC595 控制，所以通过全局变量 GV_LedStates 记录各灯的状态，然后一次性发送数据点亮或熄灭
 */
void LED_Render()
{
    // 初始化
    u8 byte;
    int i, ii;

    // 构建数据并发送数据给 74HC595
    for( i=(ARR_LENGTH(GV_LedStates)-1)/8; i >= 0; i-- ){
        byte = 0;
        
        for( ii = 0; ii < 8; ii++ ){
            byte <<= 1;
            byte |= GV_LedStates[i*8+ii];
        }
        
        // 控制 LED 的 74HC595 编号为 2，可以参考原理图
        HC595_SendByte(2, byte);
    }
    
    // 74HC595 输出
    HC595_SendOutput(2);
}



/* -------------------------------------------------- */
// ● 主逻辑代码
/* -------------------------------------------------- */

/**
 * 主逻辑：跑马灯
 *
 * @param short delay  延迟时间（毫秒） 
 */
void LED_MainHorseLamp( short delay )
{
    for( int i=0; i < ARR_LENGTH(GV_LedNumbers); i++ ){
        LED_Open(GV_LedNumbers[i], true);
        delay_ms(delay);
    }
}
