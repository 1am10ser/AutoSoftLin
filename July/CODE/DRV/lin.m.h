#ifndef __LIN_MASTER_H_
#define __LIN_MASTER_H_


    
    /* -------------------------------------------------- */
    // ● 函数声明（API）
    /* -------------------------------------------------- */
    void  LIN_MASTER_InitAuxSignal();                            //【辅信号】初始化下发的辅信号
    void  LIN_MASTER_SendAuxSignal();                            //【辅信号】发送下发的辅信号

    void  LIN_MASTER_SetAuxSignalBytes( u8 *bytes, u8 length );  //【辅信号】设置下发的辅信号
    void  LIN_MASTER_SetAuxSignalBLedOn();                       //【辅信号】设置下发的辅信号 - 背光灯亮
    void  LIN_MASTER_SetAuxSignalLLedOn();                       //【辅信号】设置下发的辅信号 - 右使能灯亮
    void  LIN_MASTER_SetAuxSignalRLedOn();                       //【辅信号】设置下发的辅信号 - 左使能灯亮

    void  LIN_MASTER_SendPrimarySignal();                        //【主信号】发送主信号给从机
    void  LIN_MASTER_SendPrimaryReceiveToUsb();                  //【主信号】发送接收的信号到 USB
    void  LIN_MASTER_PrimaryReceiveReset();                      //【主信号】重置接收反馈信号
    short LIN_MASTER_PrimaryReceiveContain( short dataNum, char dataHighLow, short data8421Bit, short data8421BitValue );  //【主信号】反馈信号包含检测

    void  LIN_MASTER_FindReceiveReset();                         //【LIN ID 查找】重置接收反馈信号
    void  LIN_MASTER_SendFindSignal( u8 id );                    //【LIN ID 查找】发送主信号给从机
    void  LIN_MASTER_SendFindReceiveToUsb();                     //【LIN ID 查找】发送接收的信号到 USB

    
    
    /* -------------------------------------------------- */
    // ● 函数声明（中断）
    /* -------------------------------------------------- */
    void LIN_MASTER_InterruptHandlerRxPrimaryReceive( u8 byte );  //【主信号】接收反馈的信号
    void LIN_MASTER_InterruptHandlerRxFindReceive( u8 byte );     //【LIN ID 查找】接收反馈的信号



    /* -------------------------------------------------- */
    // ● 函数声明（主逻辑）
    /* -------------------------------------------------- */
    void LIN_MASTER_MainFindSlaveId( bool process );  // 主逻辑：查找从机的 ID 标识
    void LIN_MASTER_MainPrimaryReceiveToUsb();        // 主逻辑：发送主信号并接收从机的反馈信号，然后输出到 USB
    void LIN_MASTER_MainPrimaryReceiveCheck();        // 主逻辑：发送主信号并接收从机的反馈信号，然后进行检测

    

#endif