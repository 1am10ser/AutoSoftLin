#ifndef __IOEXT_H_
#define __IOEXT_H_



    /* -------------------------------------------------- */
    // ● LED 采用 74HC595 控制
    // ● 使用前务必先对 74HC595 进行初始化
    /* -------------------------------------------------- */



    /* -------------------------------------------------- */
    // ● 函数声明（API）
    /* -------------------------------------------------- */
    void IOEXT_Init( bool send );                          // IOEXT 初始化

    void IOEXT_VarPowerOpen( short number, bool send );    // 打开可控电源
    void IOEXT_VarPowerClose( short number, bool send );   // 关闭可控电源
    
    void IOEXT_LcdMsp3520Reset();                          // LCD 重置
    void IOEXT_LcdMsp3520Enable( bool send );              // LCD 可用
    void IOEXT_LcdMsp3520SpiOpen( u8 device, bool send );  // LCD 的 SPI 通讯开启
    void IOEXT_LcdMsp3520SpiClose( bool send );            // LCD 的 SPI 通讯关闭
    void IOEXT_LcdMsp3520SendModeSetCmd( bool send );      // LCD 设置发送命令
    void IOEXT_LcdMsp3520SendModeSetData( bool send );     // LCD 设置发送数据
    
    
    
#endif