#ifndef __UART1_H_
#define __UART1_H_



    /* -------------------------------------------------- */
    // ● 宏定义
    /* -------------------------------------------------- */

    // UART1 对应的针脚定义
    // UART1的针脚是固定的，具体请参考芯片原理图



    /* -------------------------------------------------- */
    // ● 函数声明
    /* -------------------------------------------------- */
    void UART1_CInit();
    void UART1_SendByte( u8 byte );
    void UART1_SendBytes( u8 *bytes, short length );
    
    void UART1_InterruptHandlerRX();
    void UART1_InterruptHandlerTX();
    
    void UART1_MainReceiveSendToLin();

    
        
#endif