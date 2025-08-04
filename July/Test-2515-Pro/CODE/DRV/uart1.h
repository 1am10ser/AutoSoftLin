#ifndef __UART1_H_
#define __UART1_H_



    /* -------------------------------------------------- */
    // ● 宏定义
    /* -------------------------------------------------- */

    // UART1 对应的针脚定义
    // UART1的针脚是固定的，具体请参考芯片原理图



    /* -------------------------------------------------- */
    // ● 函数声明（API）
    /* -------------------------------------------------- */
    void UART1_CInit();                               // UART1 初始化
    void UART1_SendByte( u8 byte );                   // UART1 发送字节
    void UART1_SendBytes( u8 *bytes, short length );  // UART1 发送字节集
    
    u8*  UART1_GetReceiveData();                      // 获取接收到的数据 
    u8   UART1_GetReceiveDataCount();                 // 获取接收到的数据长度
    u8   UART1_CopyReceiveData( u8 *data );           // 复制接收到的数据
    void UART1_ResetReceiveData();                    // 重置接收到的数据
    


    /* -------------------------------------------------- */
    // ● 函数声明（中断）
    /* -------------------------------------------------- */
    void UART1_InterruptHandlerRX();                   // UART1 接收中断回调处理
    void UART1_InterruptHandlerTX();                   // UART1 发送中断回调处理
    


    /* -------------------------------------------------- */
    // ● 函数声明（主逻辑）
    /* -------------------------------------------------- */
    void UART1_MainReceiveAndSendToUsbByDirect();      // 主逻辑：接收数据并直接转发送给 USB（即发送给 UART1 自身）
    void UART1_MainReceiveAndSendToUsbByHeadFoot();    // 主逻辑：接收数据并重构数据发送给 USB（即发送给 UART1 自身）
    //void UART1_MainReceiveAndSendToLin();            // 主逻辑：接收数据并发送给 LIN

    // 主逻辑：KedeAngleSenser.exe 上位机中的 ProductAutoTester 测试台专用
    void UART1_MainProductAutoTester();
    short UART1_MainProductAutoTesterFuncExe( u8 funcCodeX1, u8 funcCodeAuxY1, u8 funcCodeAuxY2, u8 *sendBytes );
    

#endif