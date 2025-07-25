#ifndef __CONNECT_H_
#define __CONNECT_H_



    /* -------------------------------------------------- */
    // ● CONNECT 采用 74HC164 和 74HC165 控制
    // ● 使用前务必先对 74HC164 和 74HC165 进行初始化
    /* -------------------------------------------------- */



    /* -------------------------------------------------- */
    // ● 宏定义
    /* -------------------------------------------------- */

    /**
     * Connect 常量
     */
    #define CONNECT_STATE_NO       0
    #define CONNECT_STATE_YES      1
    #define CONNECT_STATE_INVALID -1


    
    /* -------------------------------------------------- */
    // ● 函数声明（API）
    /* -------------------------------------------------- */
    void CONNECT_CtlRender();                                        // Connect 控制渲染

    void CONNECT_CtlOpen( short number, bool render );               // Connect 控制连通
    void CONNECT_CtlOpenAll( bool render );                          // Connect 控制全连通
    
    void CONNECT_CtlClose( short number, bool render );              // Connect 控制断开
    void CONNECT_CtlCloseAll( bool render );                         // Connect 控制全断开
    
    void  CONNECT_DatLoad();                                         // Connect 数据装载
    
    short CONNECT_DatIsConnect( short number, bool autoload );       // Connect 数据是否连通
    short CONNECT_DatIsConnectAny( bool autoload );                  // Connect 数据是否连通（任何一路）
    short CONNECT_DatIsConnectOnly( short number, bool autoload );   // Connect 数据是否连通（仅单路连通）
	short CONNECT_DatIsConnectTrust( short number, bool autoload );  // Connect 数据是否连通（信任模式）


    
    /* -------------------------------------------------- */
    // ● 函数声明（主逻辑）
    /* -------------------------------------------------- */
    void CONNECT_MainConnectMapLed();             // 主逻辑：连通点亮灯
    void CONNECT_MainConnectAnyMapBuzzer();       // 主逻辑：任一路连通那么启动蜂鸣器
    void CONNECT_MainConnectMapLedFilterShort();  // 主逻辑：连通点亮灯并处理短路
    
    
    
#endif