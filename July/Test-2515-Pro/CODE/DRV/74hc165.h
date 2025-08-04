#ifndef __74HC165_H_
#define __74HC165_H_



    /* -------------------------------------------------- */
    // ● 宏定义
    /* -------------------------------------------------- */

    /**
     * 74HC165 对应的针脚定义
     * 请在 74hc165.c 文件中的 HC165_GetDataPin()   函数内定义 PIN
     * 请在 74hc165.c 文件中的 HC165_GetDataPort()  函数内定义 PORT
     * 请在 74hc165.c 文件中的 HC165_GetClockPin()  函数内定义 PIN
     * 请在 74hc165.c 文件中的 HC165_GetClockPort() 函数内定义 PORT
     */
    // GPIOX
    // GPIO_PIN_X

         

    /* -------------------------------------------------- */
    // ● 函数声明（API）
    /* -------------------------------------------------- */
    void HC165_Init();                                   // 74HC165 初始化
    void HC165_ReadDataValues( short number );           // 读取 74HC165 数据
    
    GPIO_TypeDef* HC165_GetDataPort( short number );     // 获取 74HC165 对应芯片上的端口号（数据脚 Q7）
    GPIO_Pin_TypeDef HC165_GetDataPin( short number );   // 获取 74HC165 对应芯片上的针脚号（数据脚 Q7）
        
    GPIO_TypeDef* HC165_GetLoadPort( short number );     // 获取 74HC165 对应芯片上的端口号（数据装载脚 /PL）
    GPIO_Pin_TypeDef HC165_GetLoadPin( short number );   // 获取 74HC165 对应芯片上的针脚号（数据装载脚 /PL）
        
    GPIO_TypeDef* HC165_GetClockPort( short number );    // 获取 74HC165 对应芯片上的端口号（时钟脚 CP）
    GPIO_Pin_TypeDef HC165_GetClockPin( short number );  // 获取 74HC165 对应芯片上的针脚号（时钟脚 CP）
    
    
    
#endif