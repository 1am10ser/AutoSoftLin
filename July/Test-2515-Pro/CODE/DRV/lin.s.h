#ifndef __LIN_SLAVE_H_
#define __LIN_SLAVE_H_

    
    
    /* -------------------------------------------------- */
    // ● 函数声明（API）
    /* -------------------------------------------------- */
    void LIN_SLAVE_InitSignalResponse();          // 初始化响应的信号
    void LIN_SLAVE_SendSignalResponse();          // 发送响应的信号
    void LIN_SLAVE_SetSignalResponse( short dataNum, char dataHighLow, short data8421Bit, short data8421BitValue );  // 设置响应的信号  

    void LIN_SLAVE_ReceiveStart();                // 数据帧接收启动
    void LIN_SLAVE_ReceiveHandle( u8 dataByte );  // 数据帧接收处理
    
    
      
#endif