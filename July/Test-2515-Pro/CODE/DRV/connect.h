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
    // �� ����������API��
    /* -------------------------------------------------- */
    void CONNECT_CtlRender();                                        // Connect ������Ⱦ

    void CONNECT_CtlOpen( short number, bool render );               // Connect ������ͨ
    void CONNECT_CtlOpenAll( bool render );                          // Connect ����ȫ��ͨ
    
    void CONNECT_CtlClose( short number, bool render );              // Connect ���ƶϿ�
    void CONNECT_CtlCloseAll( bool render );                         // Connect ����ȫ�Ͽ�
    
    void  CONNECT_DatLoad();                                         // Connect ����װ��
    
    short CONNECT_DatIsConnect( short number, bool autoload );       // Connect �����Ƿ���ͨ
    short CONNECT_DatIsConnectAny( bool autoload );                  // Connect �����Ƿ���ͨ���κ�һ·��
    short CONNECT_DatIsConnectOnly( short number, bool autoload );   // Connect �����Ƿ���ͨ������·��ͨ��
	short CONNECT_DatIsConnectTrust( short number, bool autoload );  // Connect �����Ƿ���ͨ������ģʽ��


    
    /* -------------------------------------------------- */
    // �� �������������߼���
    /* -------------------------------------------------- */
    void CONNECT_MainConnectMapLed();             // ���߼�����ͨ������
    void CONNECT_MainConnectAnyMapBuzzer();       // ���߼�����һ·��ͨ��ô����������
    void CONNECT_MainConnectMapLedFilterShort();  // ���߼�����ͨ�����Ʋ������·
    
    
    
#endif