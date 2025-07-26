#ifndef __CONNECT_H_
#define __CONNECT_H_



    /* -------------------------------------------------- */
    // �� CONNECT ���� 74HC164 �� 74HC165 ����
    // �� ʹ��ǰ����ȶ� 74HC164 �� 74HC165 ���г�ʼ��
    /* -------------------------------------------------- */



    /* -------------------------------------------------- */
    // �� �궨��
    /* -------------------------------------------------- */

    /**
     * Connect ����
     */
    #define CONNECT_STATE_NO       0
    #define CONNECT_STATE_YES      1
    #define CONNECT_STATE_INVALID -1


    
    /* -------------------------------------------------- */
    // �� ��������
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