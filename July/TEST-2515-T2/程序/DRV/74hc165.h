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
    // ● 函数声明
    /* -------------------------------------------------- */
    void HC165_Init();
    void HC165_ReadDataValues( short number );
    
    GPIO_TypeDef* HC165_GetDataPort( short number );
    GPIO_Pin_TypeDef HC165_GetDataPin( short number );
        
    GPIO_TypeDef* HC165_GetLoadPort( short number );
    GPIO_Pin_TypeDef HC165_GetLoadPin( short number );
        
    GPIO_TypeDef* HC165_GetClockPort( short number );
    GPIO_Pin_TypeDef HC165_GetClockPin( short number );
    
    
    
#endif