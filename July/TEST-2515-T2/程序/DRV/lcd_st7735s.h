#ifndef __LCD_ST7735S_H_
#define __LCD_ST7735S_H_



    /* -------------------------------------------------- */
    // �� LCD ST7735S ��������Ļ����
    /* -------------------------------------------------- */
    // �� �����о�԰ 0.96�� 80*160 ����Ļ
    // �� ����Ļ���� LCD ST7735S оƬ����
    //
    // �� ����ʱ���� ST7735S ��˵ 80  ��������(x)����Ҫ +23 ����ƫ��
    // �� ����ʱ���� ST7735S ��˵ 160 ��������(y)����Ҫ +2  ����ƫ��
    // 
    // �� ����ʱ���� ST7735S ��˵ 80  ��������(y)����Ҫ +23 ����ƫ��
    // �� ����ʱ���� ST7735S ��˵ 160 ��������(x)����Ҫ +2  ����ƫ��
    //
    // �� ����������� LCD_ST7735S_Init() �е� ��Memory Data Access Control�� �δ��붨��
    // �� �Ա���https://item.taobao.com/item.htm?spm=a1z09.2.0.0.1f592e8dLHeauk&id=611057315553&_u=l9v5jidd90

    #define LCD_ST7735S_SCREEN              2

    #define LCD_ST7735S_SCREEN_SHU          1
    #define LCD_ST7735S_SCREEN_SHU_XOFFSET  24  // ����ʱ��������ƫ�ƣ�����ÿ��Ʒ����Ļ�Ĺ��ղ�һ����������ԭ�������ƫ�ƣ���������������Ҫȷ����ԭ���ٵ�ƫ�ƣ�
    #define LCD_ST7735S_SCREEN_SHU_YOFFSET  0   // ����ʱ��������ƫ�ƣ�����ÿ��Ʒ����Ļ�Ĺ��ղ�һ����������ԭ�������ƫ�ƣ���������������Ҫȷ����ԭ���ٵ�ƫ�ƣ�

    #define LCD_ST7735S_SCREEN_HEN          2
    #define LCD_ST7735S_SCREEN_HEN_XOFFSET  0   // ����ʱ��������ƫ�ƣ�����ÿ��Ʒ����Ļ�Ĺ��ղ�һ����������ԭ�������ƫ�ƣ���������������Ҫȷ����ԭ���ٵ�ƫ�ƣ�
    #define LCD_ST7735S_SCREEN_HEN_YOFFSET  24  // ����ʱ��������ƫ�ƣ�����ÿ��Ʒ����Ļ�Ĺ��ղ�һ����������ԭ�������ƫ�ƣ���������������Ҫȷ����ԭ���ٵ�ƫ�ƣ�



    /* -------------------------------------------------- */
    // �� LCD ST7735S ��������Ŷ���
    /* -------------------------------------------------- */

    // Pin6 - ��������
    // �͵�ƽʱ LCD ���������ߵ�ƽ/���չ���
    #define LCD_ST7735S_RESET_PORT  GPIOC
    #define LCD_ST7735S_RESET_PIN   GPIO_PIN_7

    // Pin4 - SPI ʱ�ӽ�
    // ���ӵ���Ƭ���� SPI_SCK ��
    #define LCD_ST7735S_SCL_PORT    GPIOC
    #define LCD_ST7735S_SCL_PIN     GPIO_PIN_5  

    // Pin3 - SPI �����
    // ���ӵ���Ƭ���� SPI_MOSI ��
    #define LCD_ST7735S_SDA_PORT    GPIOC
    #define LCD_ST7735S_SDA_PIN     GPIO_PIN_6

    // Pin5 - RS ����
    // ���� ����͵�ƽ�� ���� ���ݣ��ߵ�ƽ�� �Ŀ�������
    #define LCD_ST7735S_RS_PORT     GPIOE
    #define LCD_ST7735S_RS_PIN      GPIO_PIN_5

    // Pin7 - SPI Ƭѡ����
    // �͵�ƽʱ SPI �������� LCD ͨѶ
    #define LCD_ST7735S_CS_PORT     GPIOE
    #define LCD_ST7735S_CS_PIN      GPIO_PIN_7

    // ���� LCD ���ŵ�Ƭ����ֱ�ӿ���
    // �������� ����
    // ����ƽ� Pin12(LEDA) �� 3.3V��Pin11(LEDK) �� GND����Ϊ�ޱ����ʱ������LCD��ʾ�����ݣ�����Ĭ��ֱ�ӵ���



    /* -------------------------------------------------- */
    // �� ��������
    /* -------------------------------------------------- */
    void LCD_ST7735S_Init();
    void LCD_ST7735S_InitIO();
    void LCD_ST7735S_SendCmd( u8 data );
    void LCD_ST7735S_SendByte( u8 byte );
    void LCD_ST7735S_SendData8( u8 data );
    void LCD_ST7735S_SendData16( u16 data );

    void LCD_ST7735S_Fill( u16 xsta, u16 ysta, u16 xend, u16 yend, u16 color );
    void LCD_ST7735S_FillSetArea( u16 xBeg, u16 yBeg, u16 xEnd, u16 yEnd );
    
    void LCD_ST7735S_DrawLine( u16 x1, u16 y1, u16 x2, u16 y2, u16 color );
    void LCD_ST7735S_DrawPoint( u16 x, u16 y, u16 color );
    void LCD_ST7735S_DrawCircle( u16 x, u16 y, u8 r, u16 color, bool fill, u8 border );
    void LCD_ST7735S_DrawRectangle( u16 x, u16 y, u16 xlen, u16 ylen, u16 color, bool fill, u8 border );

    void LCD_ST7735S_ShowIcon( u16 x, u16 y, u8 index, u8 size, u16 color, u16 background, u8 backfill );
    void LCD_ST7735S_ShowChar( u16 x, u16 y, u8 ascii, u8 size, u16 color, u16 background, u8 backfill );
    void LCD_ST7735S_ShowImage( u16 x, u16 y, u16 xlen, u16 ylen, const u8 image[], bool reverse );
    void LCD_ST7735S_ShowString( u16 x, u16 y, u8 *fonts, u8 size, u16 color, u16 background, u8 backfill );
    void LCD_ST7735S_ShowChinese( u16 x, u16 y, u8 *fonts, u8 size, u16 color, u16 background, u8 backfill );
    void LCD_ST7735S_ShowChineseOne( u16 x, u16 y, u8 *font, u8 size, u16 color, u16 background, u8 backfill );
    
    short LCD_ST7735S_ShowFloat( u16 x, u16 y, double number, u8 size, u16 color, u16 background, u8 backfill, short showlen );
    short LCD_ST7735S_ShowNumber( u16 x, u16 y, double number, u8 size, u16 color, u16 background, u8 backfill, short showlen );
    short LCD_ST7735S_ShowInteger( u16 x, u16 y, long number, u8 size, u16 color, u16 background, u8 backfill, short showlen );

    
    
#endif

