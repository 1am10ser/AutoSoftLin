#ifndef __UART3_H_
#define __UART3_H_



    /* -------------------------------------------------- */
    // ● 宏定义
    /* -------------------------------------------------- */

    // UART3 对应的针脚定义
    // UART3的针脚是固定的，具体请参考芯片原理图



    /* -------------------------------------------------- */
    // ● 函数声明（API）
    /* -------------------------------------------------- */
    void UART3_CInit();           // UART3 初始化
    void UART3_CInitUart();       // UART3 串口初始化
    void UART3_CInitLinSlave();   // UART3 LIN 配置成从节点
    void UART3_CInitLinMaster();  // UART3 LIN 配置成主节点
    
    void UART3_SendByte( u8 byte );                   // UART1 发送字节
    void UART3_SendBytes( u8 *bytes, short length );  // UART1 发送字节集
    
    u8*  UART3_GetReceiveData();                      // 获取接收到的数据 
    u8   UART3_GetReceiveDataCount();                 // 获取接收到的数据长度
    u8   UART3_CopyReceiveData( u8 *data );           // 复制接收到的数据
    void UART3_ResetReceiveData();                    // 重置接收到的数据


    
    /* -------------------------------------------------- */
    // ● 函数声明（中断）
    /* -------------------------------------------------- */
    void UART3_InterruptHandlerRX();  // UART3 接收中断回调处理
    void UART3_InterruptHandlerTX();  // UART3 发送中断回调处理
    
    void UART3_InterruptHandlerRXUart();              // UART3 读中断回调处理 - 串口数据接收
    void UART3_InterruptHandlerRXTriggerLinSlave();   // UART3 读中断回调处理 - LIN 从机接收到主机的请求数据
    void UART3_InterruptHandlerRXTriggerLinMaster();  // UART3 读中断回调处理 - LIN 主机接收到从机的响应数据
    
    
        
#endif