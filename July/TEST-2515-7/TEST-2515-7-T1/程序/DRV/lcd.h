#ifndef __LCD_H_
#define __LCD_H_



    /* -------------------------------------------------- */
    // �� LCD ������ɫ
    // �� ���� 565 ��ʾ��ɫ����ɫռ5λ����ɫռ6λ����ɫռ5λ
    /* -------------------------------------------------- */

    #define LCD_COLOR_WHITE       0xFFFF  // ��ɫ
    #define LCD_COLOR_BLACK       0x0000  // ��ɫ

    #define LCD_COLOR_DGRAY       0x62CA  // ���ɫ
    #define LCD_COLOR_GRAY        0x8410  // ��ɫ
    #define LCD_COLOR_LGRAY       0xD69A  // ǳ��ɫ

    #define LCD_COLOR_DRED        0xB800  // ���ɫ
    #define LCD_COLOR_RED         0xF800  // ��ɫ
    #define LCD_COLOR_LRED        0xFC0E  // ����ɫ

    #define LCD_COLOR_DBLUE       0x0019  // ����ɫ
    #define LCD_COLOR_BLUE        0x03FF  // ��ɫ
    #define LCD_COLOR_LBLUE       0xAEDC  // ����ɫ

    #define LCD_COLOR_DGREEN      0x2605  // ����ɫ
    #define LCD_COLOR_GREEN       0x07E0  // ��ɫ
    #define LCD_COLOR_LGREEN      0x67EB  // ����ɫ

    #define LCD_COLOR_DYELLOW     0xE5C0  // ���ɫ
    #define LCD_COLOR_YELLOW      0xFFE0  // ��ɫ
    #define LCD_COLOR_LYELLOW     0xFFF3  // ����ɫ

    #define LCD_COLOR_DMAGENTA    0x9999  // ����ɫ
    #define LCD_COLOR_MAGENTA     0xDA7F  // ��ɫ
    #define LCD_COLOR_LMAGENTA    0xE67C  // ����ɫ

    #define LCD_COLOR_DCYAN       0x0451  // ����ɫ
    #define LCD_COLOR_CYAN        0x2595  // ��ɫ
    #define LCD_COLOR_LCYAN       0x4E99  // ����ɫ

    #define LCD_COLOR_DORANGE     0xD343  // ���ɫ
    #define LCD_COLOR_ORANGE      0xFCC0  // ��ɫ
    #define LCD_COLOR_LORANGE     0xFD8C  // ����ɫ



    /* -------------------------------------------------- */
    // �� LCD ����ͼ��
    /* -------------------------------------------------- */
    #define LCD_ICON_SUN_ON       1  // ̫������
    #define LCD_ICON_SUN_OFF      0  // ̫���޹�



    /* -------------------------------------------------- */
    // �� �������������߼���
    /* -------------------------------------------------- */
    void LCD_MainInit();                     // ���߼�����ʼ��
    void LCD_MainLogo();                     // ���߼�����ʾ LOGO
    void LCD_MainMotorMonitor();             // ���߼���������м���
    void LCD_MainAnimateIcons();             // ���߼�����ʾ��̬ͼ��
    void LCD_MainColorCircles( bool fill );  // ���߼�����ʾ������ɫ��Բ 
    
    void LCD_MainLedSceneInit();                           // ���߼���LED ��������ʼ����  
    void LCD_MainLedSceneOpen( u8 number );                // ���߼���LED ������LED ������
    void LCD_MainLedSceneClose( u8 number );               // ���߼���LED ������LED �رգ�
    bool LCD_MainLedSceneState( u8 number );               // ���߼���LED ��������ȡָ�� LED ��״̬��
    void LCD_MainLedSceneAction( u8 number, bool state );  // ���߼���LED ������LED ������
    void LCD_MainLedSceneDrawItem( u8 row, u8 column, u8 number, bool state );                          // ���߼���LED ���������� LED �
    void LCD_MainLedSceneDrawTitle( u8* titleEnglish, u8* titleChinese, u8* titleRemark, u8* testSn );  // ���߼���LED ���������� LED ���⣩
    
    
    
#endif