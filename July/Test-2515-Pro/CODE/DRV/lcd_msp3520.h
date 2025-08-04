#ifndef __LCD_MSP3520_H_
#define __LCD_MSP3520_H_



    /* -------------------------------------------------- */
    // �� LCD ������Ų��� IOEXT ����
    // �� ʹ��ǰ����ȶ� IOEXT ���г�ʼ��
    /* -------------------------------------------------- */
    // �� �������û��߻�ͼ����Ҫ���� LCD_MSP3520_SpiOpenScreen() �� LCD_MSP3520_SpiClose();
    /* -------------------------------------------------- */



    /* -------------------------------------------------- */
    // �� LCD MSP3520 ��������Ļ����
    /* -------------------------------------------------- */
    // �� ���� MSP3520 3.5�� 480*320 ����Ļ
    // �� ����Ļ���� LCD ILI9488 оƬ����
    //
    // �� ����ʱ���� MSP3520 ��˵ 320 ��������(x)����Ҫ +.. ����ƫ��
    // �� ����ʱ���� MSP3520 ��˵ 480 ��������(y)����Ҫ +.. ����ƫ��
    // 
    // �� ����ʱ���� MSP3520 ��˵ 320 ��������(y)����Ҫ +.. ����ƫ��
    // �� ����ʱ���� MSP3520 ��˵ 480 ��������(x)����Ҫ +.. ����ƫ��
    //
    // �� ����������� LCD_MSP3520_Init() �е� ��Memory Data Access Control�� �δ��붨��
    // �� �Ա���http://www.lcdwiki.com/zh/3.5inch_SPI_Module_ILI9488_SKU:MSP3520

    #define LCD_MSP3520_SCREEN_SHU          1
    #define LCD_MSP3520_SCREEN_SHU_XOFFSET  0   // ����ʱ��������ƫ�ƣ�����ÿ��Ʒ����Ļ�Ĺ��ղ�һ����������ԭ�������ƫ�ƣ���������������Ҫȷ����ԭ���ٵ�ƫ�ƣ�
    #define LCD_MSP3520_SCREEN_SHU_YOFFSET  0   // ����ʱ��������ƫ�ƣ�����ÿ��Ʒ����Ļ�Ĺ��ղ�һ����������ԭ�������ƫ�ƣ���������������Ҫȷ����ԭ���ٵ�ƫ�ƣ�

    #define LCD_MSP3520_SCREEN_HEN          2
    #define LCD_MSP3520_SCREEN_HEN_XOFFSET  0   // ����ʱ��������ƫ�ƣ�����ÿ��Ʒ����Ļ�Ĺ��ղ�һ����������ԭ�������ƫ�ƣ���������������Ҫȷ����ԭ���ٵ�ƫ�ƣ�
    #define LCD_MSP3520_SCREEN_HEN_YOFFSET  0   // ����ʱ��������ƫ�ƣ�����ÿ��Ʒ����Ļ�Ĺ��ղ�һ����������ԭ�������ƫ�ƣ���������������Ҫȷ����ԭ���ٵ�ƫ�ƣ�
    
    #define LCD_MSP3520_SCREEN_LONG         480  // Һ������������
    #define LCD_MSP3520_SCREEN_SHORT        320  // Һ�����̱�����

    #define LCD_MSP3520_SCREEN              LCD_MSP3520_SCREEN_HEN



    /* -------------------------------------------------- */
    // �� LCD MSP3520 ��������Ŷ���
    /* -------------------------------------------------- */

    // Pin7 - SPI ʱ�ӽ�
    // ���ӵ���Ƭ���� SPI_SCK ��
    #define LCD_MSP3520_SCK_PORT    GPIOC
    #define LCD_MSP3520_SCK_PIN     GPIO_PIN_5  

    // Pin6 - SPI �����
    // ���ӵ���Ƭ���� SPI_MOSI ��
    #define LCD_MSP3520_SDI_PORT    GPIOC
    #define LCD_MSP3520_SDI_PIN     GPIO_PIN_6



    /* -------------------------------------------------- */
    // �� LCD MSP3520 SPI ����豸
    /* -------------------------------------------------- */
    #define LCD_MSP3520_SPI_DEVICE_TOUCH    1
    #define LCD_MSP3520_SPI_DEVICE_SCREEN   2
    #define LCD_MSP3520_SPI_DEVICE_STORAGE  3



    /* -------------------------------------------------- */
    // �� �������壨API��
    /* -------------------------------------------------- */
    void LCD_MSP3520_Init( u16 background );       // ��ʼ������
    void LCD_MSP3520_InitIO();                     // ��ʼ�� LCD MSP3520 ���� IO

    void LCD_MSP3520_SpiClose();                   // LCD �ر� SPI
    void LCD_MSP3520_SpiOpenTouch();               // LCD �򿪴��� SPI
    void LCD_MSP3520_SpiOpenScreen();              // LCD ����Ļ SPI
    void LCD_MSP3520_SpiOpenStorage();             // LCD �򿪴洢 SPI

    void LCD_MSP3520_SendCmd( u8 data );           // �� LCD MSP3520 ��������
    void LCD_MSP3520_SendByte( u8 byte );          // �� LCD MSP3520 �������ݣ�������
    void LCD_MSP3520_SendData8( u8 data );         // �� LCD MSP3520 �������ݣ���װ��
    void LCD_MSP3520_SendData16( u16 data );       // �� LCD MSP3520 �������ݣ���װ��
    void LCD_MSP3520_SendColor( u16 color );       // �� LCD MSP3520 ������ɫ���ݣ�����ģʽ��
    void LCD_MSP3520_SendColorSpeed( u16 color );  // �� LCD MSP3520 ������ɫ���ݣ�����ģʽ��
    
    void LCD_MSP3520_Fill( u16 xBeg, u16 yBeg, u16 xEnd, u16 yEnd, u16 color );  // ��������
    void LCD_MSP3520_FillSetArea( u16 xBeg, u16 yBeg, u16 xEnd, u16 yEnd );      // ���ü�����������
    
    void LCD_MSP3520_DrawLine( u16 x1, u16 y1, u16 x2, u16 y2, u16 color );                               // ����
    void LCD_MSP3520_DrawPoint( u16 x, u16 y, u16 color );                                                // ����
    void LCD_MSP3520_DrawCircle( u16 x, u16 y, u8 r, u16 color, bool fill, u8 border );                   // ��Բ          
    void LCD_MSP3520_DrawRectangle( u16 x, u16 y, u16 xlen, u16 ylen, u16 color, bool fill, u8 border );  // ������

    void LCD_MSP3520_ShowIcon( u16 x, u16 y, u8 index, u8 size, u16 color, u16 background, short backfill );        // ��ʾͼ��
    void LCD_MSP3520_ShowChar( u16 x, u16 y, u8 ascii, u8 size, u16 color, u16 background, short backfill );        // ��ʾ�ַ�
    void LCD_MSP3520_ShowImage( u16 x, u16 y, u16 xlen, u16 ylen, const u8 image[], bool reverse );                 // ��ʾͼƬ
    void LCD_MSP3520_ShowString( u16 x, u16 y, u8 *fonts, u8 size, u16 color, u16 background, short backfill );     // ��ʾ�ַ���
    void LCD_MSP3520_ShowChinese( u16 x, u16 y, u8 *fonts, u8 size, u16 color, u16 background, short backfill );    // ��ʾ����
    void LCD_MSP3520_ShowChineseOne( u16 x, u16 y, u8 *font, u8 size, u16 color, u16 background, short backfill );  // ��ʾ������
    
    short LCD_MSP3520_ShowHex( u16 x, u16 y, u8 number, u8 size, u16 color, u16 background, short backfill, bool showfix );          // ��ʾʮ��������
    short LCD_MSP3520_ShowFloat( u16 x, u16 y, double number, u8 size, u16 color, u16 background, short backfill, short showlen );   // ��ʾ������
    short LCD_MSP3520_ShowNumber( u16 x, u16 y, double number, u8 size, u16 color, u16 background, short backfill, short showlen );  // ��ʾ����
    short LCD_MSP3520_ShowInteger( u16 x, u16 y, long number, u8 size, u16 color, u16 background, short backfill, short showlen );   // ��ʾ����

    
    
#endif