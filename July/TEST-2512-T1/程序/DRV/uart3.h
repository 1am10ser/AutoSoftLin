#ifndef __UART3_H_
#define __UART3_H_



    /* -------------------------------------------------- */
    // ● 宏定义
    /* -------------------------------------------------- */

    // UART3 对应的针脚定义
    // UART3的针脚是固定的，具体请参考芯片原理图



    /* -------------------------------------------------- */
    // ● 函数声明
    /* -------------------------------------------------- */
    void UART3_CInit();
    void UART3_CInitLinSlave();
    void UART3_CInitLinMaster();
    
    void UART3_SendByte( u8 byte );
    void UART3_SendBytes( u8 *bytes, short length );
        
    void UART3_InterruptHandlerRX();
    void UART3_InterruptHandlerTX();
    
    void UART3_InterruptHandlerRXTriggerLinSlave();
    void UART3_InterruptHandlerRXTriggerLinMaster();
    
    
        
#endif