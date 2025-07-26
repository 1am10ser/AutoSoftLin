#ifndef __IOEXT_H_
#define __IOEXT_H_



    /* -------------------------------------------------- */
    // ● LED 采用 74HC595 控制
    // ● 使用前务必先对 74HC595 进行初始化
    /* -------------------------------------------------- */



    /* -------------------------------------------------- */
    // ● 函数声明
    /* -------------------------------------------------- */
    void IOEXT_Init( bool send );

    void IOEXT_VarPowerOpen( short number, bool send );
    void IOEXT_VarPowerClose( short number, bool send );
    
    void IOEXT_LcdMsp3520Reset();
    void IOEXT_LcdMsp3520Enable( bool send );
    void IOEXT_LcdMsp3520SpiOpen( u8 device, bool send );
    void IOEXT_LcdMsp3520SpiClose( bool send );
    void IOEXT_LcdMsp3520SendModeSetCmd( bool send );
    void IOEXT_LcdMsp3520SendModeSetData( bool send );
    
    
    
    
#endif