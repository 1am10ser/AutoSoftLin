#include "common.h"



/* -------------------------------------------------- */
// ● IOEXT 采用 74HC595 控制
// ● 使用前务必先对 74HC595 进行初始化
/* -------------------------------------------------- */



/* -------------------------------------------------- */
// ● 全局变量
/* -------------------------------------------------- */

/**
 * IOExt 基础变量
 */

//【系统使用】IOEXT 的 74HC595 数据，低位优先发送，最终 GV_IOExtData 数据的 高位->低位 映射到 U2:Q0->U2:Q7
u8 GV_IOExtData = 0x00;

//【系统使用】IOEXT 使用的 74HC595 编号 
u8 GV_IOExtHC595Num = 2;



/* -------------------------------------------------- */
// ● 函数主体
/* -------------------------------------------------- */

/**
 * IOEXT 初始化
 * 该函数使用前务必先调用 HC595_Init() 对 74HC595 进行初始化
 *
 * IOEXT 初始化后，默认发送的数据全是 0，IOEXT 8个脚对应的设备需要自行调用对应的函数进行功能激活
 * 例如：U2Q1 发送 0 后，LCD 处于复位状态，那么你需要调用 IOEXT_LcdMsp3520Enable(true) 函数使 LCD 激活可用
 */
void IOEXT_Init( bool send )
{
    HC595_SendByteSet(GV_IOExtHC595Num, GV_IOExtData);
    if( send ) HC595_SendByte(GV_IOExtHC595Num);
}


/**
 * 打开可控电源
 *
 * @param short number  PCB上的可控电源编号
 * @param bool  send    是否发送
 */
void IOEXT_VarPowerOpen( short number, bool send )
{
    if( number == 1 ) GV_IOExtData |= 0x04;  // U2Q5
    if( number == 2 ) GV_IOExtData |= 0x02;  // U2Q6 
    if( number == 3 ) GV_IOExtData |= 0x01;  // U2Q7

    HC595_SendByteSet(GV_IOExtHC595Num, GV_IOExtData);
    if( send ) HC595_SendByte(GV_IOExtHC595Num);
}


/**
 * 关闭可控电源
 *
 * @param short number  PCB上的可控电源编号
 * @param bool  send    是否发送
 */
void IOEXT_VarPowerClose( short number, bool send )
{
    if( number == 1 ) GV_IOExtData &= 0xFB;  // U2Q5
    if( number == 2 ) GV_IOExtData &= 0xFD;  // U2Q6 
    if( number == 3 ) GV_IOExtData &= 0xFE;  // U2Q7

    HC595_SendByteSet(GV_IOExtHC595Num, GV_IOExtData);
    if( send ) HC595_SendByte(GV_IOExtHC595Num);
}


/**
 * LCD 可用
 * 设置 LCD RESET 脚为高电平（低电平时 LCD 复位）
 *
 * @param bool send  是否发送
 */
void IOEXT_LcdMsp3520Enable( bool send )
{
    GV_IOExtData |= 0x40;  // U2Q1

    HC595_SendByteSet(GV_IOExtHC595Num, GV_IOExtData);
    if( send ) HC595_SendByte(GV_IOExtHC595Num);
}


/**
 * LCD 重置
 * 设置 LCD RESET 脚为低电平 -> 高电平，实现 复位-> 可用 的动作
 */
void IOEXT_LcdMsp3520Reset()
{
    GV_IOExtData &= 0xBF;  // U2Q1;

    // 发送低电平进行重置
    HC595_SendByteSet(GV_IOExtHC595Num, GV_IOExtData);
    HC595_SendByte(GV_IOExtHC595Num);
    
    // 延迟
    delay_ms(120);
    
    // LCD 可用
    IOEXT_LcdMsp3520Enable(true);
}


/**
 * LCD 的屏幕 SPI 通讯可用
 * 将 LCD 的屏幕 SPI 片选引脚拉低
 *
 * @param bool send  是否发送
 */
void IOEXT_LcdMsp3520SpiScreenEnable( bool send )
{
    GV_IOExtData &= 0xEF;  // U2Q3
    
    HC595_SendByteSet(GV_IOExtHC595Num, GV_IOExtData);
    if( send ) HC595_SendByte(GV_IOExtHC595Num);
}


/**
 * LCD 的屏幕 SPI 通讯关闭
 * 将 LCD 的屏幕 SPI 片选引脚拉高
 *
 * @param bool send  是否发送
 */
void IOEXT_LcdMsp3520SpiScreenDisable( bool send )
{
    GV_IOExtData |= 0x10;  // U2Q3

    HC595_SendByteSet(GV_IOExtHC595Num, GV_IOExtData);
    if( send ) HC595_SendByte(GV_IOExtHC595Num);
}


/**
 * LCD 的存储卡 SPI 通讯可用
 * 将 LCD 的存储卡 SPI 片选引脚拉低
 *
 * @param bool send  是否发送
 */
void IOEXT_LcdMsp3520SpiStorageEnable( bool send )
{
    GV_IOExtData &= 0xDF;  // U2Q2
    
    HC595_SendByteSet(GV_IOExtHC595Num, GV_IOExtData);
    if( send ) HC595_SendByte(GV_IOExtHC595Num);
}


/**
 * LCD 的存储卡 SPI 通讯关闭
 * 将 LCD 的存储卡 SPI 片选引脚拉高
 *
 * @param bool send  是否发送
 */
void IOEXT_LcdMsp3520SpiStorageDisable( bool send )
{
    GV_IOExtData |= 0x20;  // U2Q2

    HC595_SendByteSet(GV_IOExtHC595Num, GV_IOExtData);
    if( send ) HC595_SendByte(GV_IOExtHC595Num);
}


/**
 * LCD 的触摸 SPI 通讯可用
 * 将 LCD 的触摸 SPI 片选引脚拉低
 *
 * @param bool send  是否发送
 */
void IOEXT_LcdMsp3520SpiTouchEnable( bool send )
{
    GV_IOExtData &= 0xF7;  // U2Q4
    
    HC595_SendByteSet(GV_IOExtHC595Num, GV_IOExtData);
    if( send ) HC595_SendByte(GV_IOExtHC595Num);
}


/**
 * LCD 的触摸 SPI 通讯关闭
 * 将 LCD 的触摸 SPI 片选引脚拉高
 *
 * @param bool send  是否发送
 */
void IOEXT_LcdMsp3520SpiTouchDisable( bool send )
{
    GV_IOExtData |= 0x08;  // U2Q4

    HC595_SendByteSet(GV_IOExtHC595Num, GV_IOExtData);
    if( send ) HC595_SendByte(GV_IOExtHC595Num);
}


/**
 * LCD 的 SPI 通讯开启
 * 将 LCD 的 SPI 片选引脚拉低
 *
 * @param u8   device  LCD 相关设备
 * @param bool send    是否发送
 */
void IOEXT_LcdMsp3520SpiOpen( u8 device, bool send )
{
    IOEXT_LcdMsp3520SpiClose(false);

    if( device == LCD_MSP3520_SPI_DEVICE_TOUCH ) IOEXT_LcdMsp3520SpiTouchDisable(true);
    if( device == LCD_MSP3520_SPI_DEVICE_SCREEN ) IOEXT_LcdMsp3520SpiScreenEnable(true);
    if( device == LCD_MSP3520_SPI_DEVICE_STORAGE ) IOEXT_LcdMsp3520SpiStorageDisable(true);
}


/**
 * LCD 的 SPI 通讯关闭
 * 将 LCD 的 SPI 片选引脚拉高
 *
 * @param bool send  是否发送
 */
void IOEXT_LcdMsp3520SpiClose( bool send )
{
    IOEXT_LcdMsp3520SpiTouchDisable(send);
    IOEXT_LcdMsp3520SpiScreenDisable(send);
    IOEXT_LcdMsp3520SpiStorageDisable(send);
}


/**
 * LCD 设置发送命令
 */
void IOEXT_LcdMsp3520SendModeSetCmd( bool send )
{
    GV_IOExtData &= 0x7F;  // U2Q0

    HC595_SendByteSet(GV_IOExtHC595Num, GV_IOExtData);
    if( send ) HC595_SendByte(GV_IOExtHC595Num);
}


/**
 * LCD 设置发送数据
 */
void IOEXT_LcdMsp3520SendModeSetData( bool send )
{
    GV_IOExtData |= 0x80;  // U2Q0

    HC595_SendByteSet(GV_IOExtHC595Num, GV_IOExtData);
    if( send ) HC595_SendByte(GV_IOExtHC595Num);
}

