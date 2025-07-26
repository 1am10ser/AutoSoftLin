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
    // ● 函数声明
    /* -------------------------------------------------- */
    void CONNECT_CtlRender();
         
    void CONNECT_CtlOpen( short number, bool render );
    void CONNECT_CtlOpenAll( bool render );
    
    void CONNECT_CtlClose( short number, bool render );
    void CONNECT_CtlCloseAll( bool render );
    
    void  CONNECT_DatLoad();
    
    short CONNECT_DatIsConnect( short number, bool autoload );
    short CONNECT_DatIsConnectAny( bool autoload );
    short CONNECT_DatIsConnectOnly( short number, bool autoload );
    
    void CONNECT_MainConnectMapLed();
    void CONNECT_DemoConnectAnyMapLedOne();
    void CONNECT_MainConnectMapLedFilterShort();
    
    
    
#endif