#ifndef __74HC595_H_
#define __74HC595_H_



    /* -------------------------------------------------- */
    // ● 宏定义
    /* -------------------------------------------------- */

    /**
     * 74HC595 对应的针脚定义
     * 请在 74hc595.c 文件中的 HC595_GetDataPin()        函数内定义 PIN
     * 请在 74hc595.c 文件中的 HC595_GetDataPort()       函数内定义 PORT
     * 请在 74hc595.c 文件中的 HC595_GetBitClockPin()    函数内定义 PIN
     * 请在 74hc595.c 文件中的 HC595_GetBitClockPort()   函数内定义 PORT
     * 请在 74hc595.c 文件中的 HC595_GetStoreClockPin()  函数内定义 PIN
     * 请在 74hc595.c 文件中的 HC595_GetStoreClockPort() 函数内定义 PORT
     */
    // GPIOX
    // GPIO_PIN_X



    /* -------------------------------------------------- */
    // ● 函数声明（API）
    /* -------------------------------------------------- */
    void HC595_Init();                                // 74HC595 初始化
    void HC595_SendByte( short number );              // 74HC595 发送数据
    void HC595_SendByteSet( short number, u8 byte );  // 设置 74HC595 发送数据
    
    
    
#endif