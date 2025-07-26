#ifndef __LIN_H_
#define __LIN_H_



    /* -------------------------------------------------- */
    // ● 宏定义
    /* -------------------------------------------------- */

    #define LIN_MODE_SLAVE     1
    #define LIN_MODE_MASTER    2

    #define LIN_TXUSB_MODE_DEFAULT   1  // 信号转发到USB的模式：不间断发送（默认）
    #define LIN_TXUSB_MODE_DOWNONE   2  // 信号转发到USB的模式：仅按下发一次
    #define LIN_TXUSB_MODE_DOWNUPONE 3  // 信号转发到USB的模式：仅按下和松开发一次



    /* -------------------------------------------------- */
    // ● 数据类型定义
    /* -------------------------------------------------- */

    /**
     * LIN 状态
     * 该变量指明了当前 LIN 正处于的状态枚举
     */
    typedef enum{
        LinStateIdle,           // 步骤0：空闲
        LinStateIdWait,         // 步骤3：等待标识段
        LinStateBreakReceived,  // 步骤1：收到间隔域
        LinStateSynchFieldWait, // 步骤2：等待同步段
        LinStateDataRevice      // 步骤4：数据接收
    }LinState;



    /* -------------------------------------------------- */
    // ● 函数声明
    /* -------------------------------------------------- */
    void LIN_Init();
    u8 LIN_CalcChecknum( u8 *bytes, short length, short start );

    
      
#endif