#include "common.h"



/* -------------------------------------------------- */
// �� LCD ������Ų��� IOEXT ����
// �� ʹ��ǰ����ȶ� IOEXT ���г�ʼ��
/* -------------------------------------------------- */
// �� Ϊ����� LCD ��ʾ���ٶȣ��ײ㺯��������ֱ�ӿ��� SPI
// �� ���Գ��˳�ʼ�������������������û��߻�ͼ����Ҫ���� LCD_MSP3520_SpiOpenScreen() �� LCD_MSP3520_SpiClose();
/* -------------------------------------------------- */



/* -------------------------------------------------- */
// �� ȫ�ֱ���
/* -------------------------------------------------- */

/**
 * LCD �ⲿ����
 * ���±��������� lcd.c �ļ���
 */
extern const u8 GV_LcdAscii0612[][12];
extern const u8 GV_LcdAscii0714[][14];
extern const u8 GV_LcdAscii0816[][16];
extern const u8 GV_LcdAscii1224[][48];
extern const u8 GV_LcdAscii1632[][64];

extern const u8 GV_LcdIcon16[][32];
extern const u8 GV_LcdIcon24[][72];
extern const u8 GV_LcdIcon32[][128];

extern const short GV_LcdChinese12Total;
extern const short GV_LcdChinese14Total;
extern const short GV_LcdChinese16Total;
extern const short GV_LcdChinese24Total;
//extern const short GV_LcdChinese32Total;  // ��Լ��Ƭ�� ROM �ռ䣬Ĭ��ע�͵�

extern const struct STRUCT_LCD_CHINESE12 GV_LcdChinese12[];
extern const struct STRUCT_LCD_CHINESE14 GV_LcdChinese14[];
extern const struct STRUCT_LCD_CHINESE16 GV_LcdChinese16[];
extern const struct STRUCT_LCD_CHINESE24 GV_LcdChinese24[];
//extern const struct STRUCT_LCD_CHINESE32 GV_LcdChinese32[];  // ��Լ��Ƭ�� ROM �ռ䣬Ĭ��ע�͵�



/* -------------------------------------------------- */
// �� LCD MSP3520 ��غ���
/* -------------------------------------------------- */


/**
 * ��ʼ������
 *
 * @param u16  background  ����ɫ
 */
void LCD_MSP3520_Init( u16 background )
{
    // ��ʼ�� LCD MSP3520 ���� IO
    LCD_MSP3520_InitIO();

    // Һ������λ
    // �����һ���ϵ�ʱ���ܳ����޷����������
    IOEXT_LcdMsp3520Reset();
    
    // ���� SPI ��Ļ
    LCD_MSP3520_SpiOpenScreen();

    
    //--------------------- Sleep OUT ---------------------//
    // ˯���˳�
    // �ο������ֲ�� 166 ҳ
    // �ֲ��Ϲ涨�� Sleep Out ָ�������� 5ms �����ٷ�����һ��ָ����Ǹ���ѹ��ʱ�ӵ�·���ȶ��ṩ�㹻ʱ�䡣ͬʱ���� 5ms �У���ʾģ��������еĳ���Ĭ��ֵ���Ĵ���
    LCD_MSP3520_SendCmd(0x11);
    delay_ms(50);
        

    //--------------------- Memory Access Control ---------------------//
    // �ڴ����ݷ��ʿ���
    // �ο������ֲ�� 192 ҳ
    //
    // This command defines read/write scanning direction of frame memory
    // ����λ��MY(Mirror Y-axis),    MX(Mirror X-axis),                  MV,                     ML,           RGB,                       MH, -, -
    // ����λ��Row Address Order, Column Address Order, Row/Column Exchange, Vertical Refresh Order, RGB-BGR ORDER, Horizontal Refresh Order, -, -
    //
    // MV=0 ��ʾ����, MV=1 ��ʾ����
    // MY �� MX ��ʾ���꾵��ͨ���������ÿ��Խ����� 0,0 �󶨵���Ļ������/����/����/����
    //
    // �� ����ʱ������涨 �̱� ��������(x)
    // �� ����ʱ������涨 ���� ��������(y)
    // 
    // �� ����ʱ������涨 �̱� ��������(y)
    // �� ����ʱ������涨 ���� ��������(x)
    LCD_MSP3520_SendCmd(0x36);
    if( LCD_MSP3520_SCREEN == LCD_MSP3520_SCREEN_SHU ) LCD_MSP3520_SendData8(0x48);  // 1100 1000
    if( LCD_MSP3520_SCREEN == LCD_MSP3520_SCREEN_HEN ) LCD_MSP3520_SendData8(0x28);  // 1010 1000
    
    
    //--------------------- Interface Pixel Format ---------------------//
    // �ӿ����ظ�ʽ
    // �ο������ֲ�� 200 ҳ
    //
    // ����һ����ɫ����ռ�õ�λ������Ȼ��Һ������ RGB16 ����ɫ����������ʱ��������Ϊ RGB18��ͬʱ������ɫʱ R��G��B Ҫ���԰���1���ֽڷ��� 
    // ���Ϲ����������ֲ�� 119 ҳ�н��ܣ�3�ߵ� SPI ��ɫ�����Ҫ 18-bit
    LCD_MSP3520_SendCmd(0x3A);
    LCD_MSP3520_SendData8(0x06);  // 18-bit
    
    
    //--------------------- VCOM Control ---------------------//
    // �ο������ֲ�� 243 ҳ
    // Ŀǰ���Թ������������ã������ûҶȱ�����ÿ������֮����Ӻ�гϸ�壻ֱ��ʹ���ֳɵ��Ż���������
    LCD_MSP3520_SendCmd(0xC5);
    LCD_MSP3520_SendData8(0x00);
    LCD_MSP3520_SendData8(0x00);
    LCD_MSP3520_SendData8(0x80);
    

    //--------------------- PGAMCTRL (Positive Gamma Control)  ---------------------//
    // ���ûҶȵ�ѹ�Ե��� TFT ���� Gamma ����
    // �ο������ֲ�� 265 ҳ
    // ��Ҫ���þ����ûҶȱ�����ÿ������֮����Ӻ�гϸ�壻���Ƿǳ��˽�ײ㣬����ֱ��ʹ���ֳɵ��Ż���������
    LCD_MSP3520_SendCmd(0xE0);
    LCD_MSP3520_SendData8(0x0B);   
    LCD_MSP3520_SendData8(0x17);   
    LCD_MSP3520_SendData8(0x0A);   
    LCD_MSP3520_SendData8(0x0D);   
    LCD_MSP3520_SendData8(0x1A);   
    LCD_MSP3520_SendData8(0x19);   
    LCD_MSP3520_SendData8(0x16);   
    LCD_MSP3520_SendData8(0x1D);   
    LCD_MSP3520_SendData8(0x21);   
    LCD_MSP3520_SendData8(0x26);   
    LCD_MSP3520_SendData8(0x37);   
    LCD_MSP3520_SendData8(0x3c);   
    LCD_MSP3520_SendData8(0x00);   
    LCD_MSP3520_SendData8(0x09);   
    LCD_MSP3520_SendData8(0x05);   
    LCD_MSP3520_SendData8(0x10);   


    //--------------------- NGAMCTRL (Negative Gamma Control)  ---------------------//
    // ���ûҶȵ�ѹ�Ե��� TFT ���� Gamma ����
    // �ο������ֲ�� 266 ҳ
    // ��Ҫ���þ����ûҶȱ�����ÿ������֮����Ӻ�гϸ�壻���Ƿǳ��˽�ײ㣬����ֱ��ʹ���ֳɵ��Ż���������
    LCD_MSP3520_SendCmd(0xE1);
    LCD_MSP3520_SendData8(0x0C);   
    LCD_MSP3520_SendData8(0x19);   
    LCD_MSP3520_SendData8(0x09);   
    LCD_MSP3520_SendData8(0x0D);   
    LCD_MSP3520_SendData8(0x1b);   
    LCD_MSP3520_SendData8(0x19);   
    LCD_MSP3520_SendData8(0x15);   
    LCD_MSP3520_SendData8(0x1d);   
    LCD_MSP3520_SendData8(0x21);   
    LCD_MSP3520_SendData8(0x26);   
    LCD_MSP3520_SendData8(0x39);   
    LCD_MSP3520_SendData8(0x3E);   
    LCD_MSP3520_SendData8(0x00);   
    LCD_MSP3520_SendData8(0x09);   
    LCD_MSP3520_SendData8(0x05);   
    LCD_MSP3520_SendData8(0x10);   
            
    
    //--------------------- ��ʼ������ ---------------------//
    // �ڿ�����ʾģʽ֮ǰ��ʼ��Һ����������ɫ
    // ���� MSP3520 Һ�������� SPI ͨѶ������ʱ��Ҫͨ�� SPI ����ÿ�����ص���ɫ������ SPI �ٶȽ�����MSP3520��Ļ��̫��
    // ��������ʱ�����Ըо�����Ļ������ɨ����̣�������Һ������ʽ��ʾ����֮ǰ������Ԥ�ȸ��ڴ��������ݣ��Ա�����Һ����ɨ����̵Ļ���
    if( LCD_MSP3520_SCREEN == LCD_MSP3520_SCREEN_HEN ) LCD_MSP3520_Fill(0, 0, LCD_MSP3520_SCREEN_LONG, LCD_MSP3520_SCREEN_SHORT, background);
    if( LCD_MSP3520_SCREEN == LCD_MSP3520_SCREEN_SHU ) LCD_MSP3520_Fill(0, 0, LCD_MSP3520_SCREEN_SHORT, LCD_MSP3520_SCREEN_LONG, background);
    
    LCD_360ADCoordinate(LCD_COLOR565_LBLUE);
    //--------------------- Display On ---------------------//
    // ������ʾģʽ
    // �ο������ֲ�� 174 ҳ
    // �� MSP3520 �е��ڴ�����ʵʱ��ʾ��Һ����
    LCD_MSP3520_SendCmd(0x29);


    // Ĭ�� SPI �ر�
    LCD_MSP3520_SpiClose();
}


/**
 * ��ʼ�� LCD MSP3520 ���� IO
 */
void LCD_MSP3520_InitIO()
{
    // Ĭ������
    IOEXT_LcdMsp3520Enable(true);

    // Ĭ�Ϸ�������
    IOEXT_LcdMsp3520SendModeSetData(true);

    // Ĭ�� SPI �ر�
    IOEXT_LcdMsp3520SpiClose(true);

    // SPI IO��ʼ
    GPIO_Init(LCD_MSP3520_SCK_PORT, LCD_MSP3520_SCK_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
    GPIO_Init(LCD_MSP3520_SDI_PORT, LCD_MSP3520_SDI_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
  
    // SPI ����ʼ
    SPI_DeInit();
    
    // SPI ��ʼ��
    //     ����1��֡��ʽ��SPI_FIRSTBIT_MSB ��ʾ���ȷ��͸�λ
    //     ����2�������ʷ�Ƶֵ
    //     ����3������ģʽ��SPI_MODE_MASTER ��ʾ��ģʽ
    //     ����4��ʱ�Ӽ��ԣ�SPI_CLOCKPOLARITY_LOW ��ʾ SCK �����ڿ���״̬ʱ�ĵ�ƽ
    //     ���� ������ο�������ȭSTM8 56 ����SPI��������ԭ���е� ��6. ʱ���źŵ���λ�ͼ��ԡ�
    //     ����5��ʱ����λ��SPI_CLOCKPHASE_1EDGE ��ʾ���ݲ����ӵ�һ��ʱ�ӱ��ؿ�ʼ
    //     ���� ������ο�������ȭSTM8 56 ����SPI��������ԭ���е� ��6. ʱ���źŵ���λ�ͼ��ԡ�
    //     ����6�����ݷ���SPI_DATADIRECTION_1LINE_TX ��ʾ ֻ����ģʽ
    //     ����7���ӻ�����SPI_NSS_SOFT ��ʾ��ֹ������豸����
    //     ����8��CRCУ�飬�ò������Ϻܶ������� 0x00�����������õ�����޷�ֱ������ 0x00����������ˣ���ô�����ȫ�������޷�����ִ��
    //     ���� ������ִ�������Ĵ���Σ�SPI_Init()                              ---->  assert_param(IS_SPI_CRC_POLYNOMIAL_OK(CRCPolynomial));
    //     ���� ������������������������IS_SPI_CRC_POLYNOMIAL_OK(CRCPolynomial) ---->  #define IS_SPI_CRC_POLYNOMIAL_OK(POLYNOMIAL) ((POLYNOMIAL) > (uint8_t)0x00)
    SPI_Init(SPI_FIRSTBIT_MSB, SPI_BAUDRATEPRESCALER_2, SPI_MODE_MASTER, SPI_CLOCKPOLARITY_LOW, SPI_CLOCKPHASE_1EDGE, SPI_DATADIRECTION_1LINE_TX, SPI_NSS_SOFT, 0x01);
    
    // SPI ����
    SPI_Cmd(ENABLE);
}


/**
 * LCD �򿪴��� SPI
 */
void LCD_MSP3520_SpiOpenTouch()
{
    IOEXT_LcdMsp3520SpiOpen(LCD_MSP3520_SPI_DEVICE_TOUCH, true);
}


/**
 * LCD ����Ļ SPI
 */
void LCD_MSP3520_SpiOpenScreen()
{
    IOEXT_LcdMsp3520SpiOpen(LCD_MSP3520_SPI_DEVICE_SCREEN, true);
}


/**
 * LCD �򿪴洢 SPI
 */
void LCD_MSP3520_SpiOpenStorage()
{
    IOEXT_LcdMsp3520SpiOpen(LCD_MSP3520_SPI_DEVICE_STORAGE, true);
}


/**
 * LCD �ر� SPI
 */
void LCD_MSP3520_SpiClose()
{
    IOEXT_LcdMsp3520SpiClose(true);
}

/**
 * �� LCD MSP3520 ��������
 * �������ɺ������лط�������ģʽ��Ҳ����˵���� LCD MSP3520 ��������Ĭ��ģʽ�¶��Ƿ�������
 *
 * @param u8 data  �����ֽ�
 */
void LCD_MSP3520_SendCmd( u8 data )
{
    // ���÷�������ģʽ
    IOEXT_LcdMsp3520SendModeSetCmd(true);

    // ��������
    LCD_MSP3520_SendByte(data);
    
    // ���÷�������ģʽ��Ĭ�ϣ�
    IOEXT_LcdMsp3520SendModeSetData(true);
}


/**
 * �� LCD MSP3520 ��������
 * ���� LCD_MSP3520_SendByte() ���ͺ�����ȷ�����ݰ�ȫ����ȫ���ͣ����ٶ���Щ��ʧ��
 *
 * @param u8 data  �����ֽ�
 */
void LCD_MSP3520_SendData8( u8 data )
{
    LCD_MSP3520_SendByte(data);
}
void LCD_MSP3520_SendData16( u16 data )
{
    LCD_MSP3520_SendByte(data>>8);
    LCD_MSP3520_SendByte(data);
}
void LCD_MSP3520_SendColor( u16 color )
{
    LCD_MSP3520_SendByte((color>>8)&0xF8);  // R��ȡǰ 5 λ
    LCD_MSP3520_SendByte((color>>3)&0xFC);  // G��ȡ�� 6 λ
    LCD_MSP3520_SendByte((color<<3));       // B��ȡ�� 5 λ
}


/**
 * �� LCD MSP3520 ������ɫ����
 * ���� SPI_SendData() �ײ㷢�ͺ����������������Ƿ�ɹ����ͣ������ٶȣ�
 * Ϊ�˻�ȡ���Ӽ��µ��ٶȣ�����Բ�ͨ������ LCD_MSP3520_SendColorSpeed()������ֱ�ӿ��� LCD_MSP3520_SendColorSpeed() ����Ĵ��뵽ҵ���߼���
 *
 * @param u8 data  �����ֽ�
 */
void LCD_MSP3520_SendColorSpeed( u16 color )
{
    SPI_SendData((color>>8)&0xF8);  // R��ȡǰ 5 λ
    SPI_SendData((color>>3)&0xFC);  // G��ȡ�� 6 λ
    SPI_SendData((color<<3));       // B��ȡ�� 5 λ
}


/**
 * �� LCD MSP3520 ��������
 *
 * @param u8 byte  �����ֽ�
 */
void LCD_MSP3520_SendByte( u8 byte )
{
    // �ȴ� SPI ����
    while(SPI_GetFlagStatus(SPI_FLAG_BSY));

    // �������ݲ��ȴ��������
    SPI_SendData(byte);
    while( !SPI_GetFlagStatus(SPI_FLAG_TXE) );
}


/**
 * ��������
 * X/Y ����һ����������꣺���������������ߺ�����X ��ָ�������꣬Y��ָ��������
 *
 * @param u16 xBeg   ��ʼ��ַ X ��ֵ����������ֵ��
 * @param u16 yBeg   ��ʼ��ַ Y ��ֵ����������ֵ��
 * @param u16 xEnd   ������ַ X ��ֵ����������ֵ��
 * @param u16 yEnd   ������ַ Y ��ֵ����������ֵ��
 * @param u16 color  ��ɫ
 */
void LCD_MSP3520_Fill( u16 xBeg, u16 yBeg, u16 xEnd, u16 yEnd, u16 color )
{      
    // ��ʼ��
    u16 i,j; 
    
    // ���ü�����������
    LCD_MSP3520_FillSetArea(xBeg, yBeg, xEnd, yEnd);

    // ѭ����������
    // ʹ�� LCD_MSP3520_SendColor() ��������ȷ�����ݰ�ȫ����ȫ���ͣ����ٶ���Щ��ʧ
    // ������������Զ�������Ҫ���͵�������ѡ��ʹ�� LCD_MSP3520_SendColor() ����ֱ�ӵ��� SPI ���ͺ���
    if( (u32)(xEnd-xBeg)*(u32)(yEnd-yBeg) < 100 ){
        for( i = yBeg; i <= yEnd; i++ ){
            for( j = xBeg; j <= xEnd; j++ ){
                LCD_MSP3520_SendColor(color); 
            }
        }
    }else{
        for( i = yBeg; i <= yEnd; i++ ){
            for( j = xBeg; j <= xEnd; j++ ){
                SPI_SendData((color>>8)&0xF8);  // R��ȡǰ 5 λ
                SPI_SendData((color>>3)&0xFC);  // G��ȡ�� 6 λ
                SPI_SendData((color<<3));       // B��ȡ�� 5 λ
            }
        }
    }
}


/**
 * ���ü�����������
 * X/Y ����һ����������꣺���������������ߺ�����X ��ָ�������꣬Y��ָ��������
 *
 * ����������󣬸ú�������д��һ�� Memory Write �����Ϊ LCD_MSP3520_FillSetArea() һ�㶼��Ҫ����ɫ����
 * 
 * @param u16 xBeg  ��ʼ��ַ X ��ֵ����������ֵ��
 * @param u16 yBeg  ��ʼ��ַ Y ��ֵ����������ֵ��
 * @param u16 xEnd  ������ַ X ��ֵ����������ֵ��
 * @param u16 yEnd  ������ַ Y ��ֵ����������ֵ��
 */
void LCD_MSP3520_FillSetArea( u16 xBeg, u16 yBeg, u16 xEnd, u16 yEnd )
{
    // ������ʾ
    if( LCD_MSP3520_SCREEN == LCD_MSP3520_SCREEN_SHU ){
        // �е�ַ����
        LCD_MSP3520_SendCmd(0x2A); 
        LCD_MSP3520_SendData16(xBeg+LCD_MSP3520_SCREEN_SHU_XOFFSET);
        LCD_MSP3520_SendData16(xEnd+LCD_MSP3520_SCREEN_SHU_XOFFSET);
        
        // �е�ַ����
        LCD_MSP3520_SendCmd(0x2B); 
        LCD_MSP3520_SendData16(yBeg+LCD_MSP3520_SCREEN_SHU_YOFFSET);
        LCD_MSP3520_SendData16(yEnd+LCD_MSP3520_SCREEN_SHU_YOFFSET);
    }
    
    // ������ʾ
    if( LCD_MSP3520_SCREEN == LCD_MSP3520_SCREEN_HEN ){
        // �е�ַ����
        LCD_MSP3520_SendCmd(0x2A); 
        LCD_MSP3520_SendData16(xBeg+LCD_MSP3520_SCREEN_HEN_XOFFSET);
        LCD_MSP3520_SendData16(xEnd+LCD_MSP3520_SCREEN_HEN_XOFFSET);
        
        // �е�ַ����
        LCD_MSP3520_SendCmd(0x2B); 
        LCD_MSP3520_SendData16(yBeg+LCD_MSP3520_SCREEN_HEN_YOFFSET);
        LCD_MSP3520_SendData16(yEnd+LCD_MSP3520_SCREEN_HEN_YOFFSET);
    }
    
    // Memory Write
    // �ο������ֲ�� 179 ҳ
    // �ڴ�д��˳������ 0x36 ����� D5 λֵ��ͬ����ͬ
    LCD_MSP3520_SendCmd(0x2C); 
}



/**
 * ����
 * X/Y ����һ����������꣺���������������ߺ�����X ��ָ�������꣬Y��ָ��������
 *
 * @param u16 x      ��ʼ��ַ X ��ֵ����������ֵ��
 * @param u16 y      ��ʼ��ַ Y ��ֵ����������ֵ��
 * @param u16 color  ��ɫ
 */
void LCD_MSP3520_DrawPoint( u16 x, u16 y, u16 color )
{
    LCD_MSP3520_FillSetArea(x, y, x, y);
    LCD_MSP3520_SendColor(color);
} 


/**
 * ����
 * ʹ�� Bresenham �㷨������б�ʵ�ֱ��
 * �ο���https://blog.csdn.net/weixin_44350205/article/details/105642536
 *
 * X/Y ����һ����������꣺���������������ߺ�����X ��ָ�������꣬Y��ָ��������
 *
 * @param u16 x1     ��� X ��ֵ����������ֵ��
 * @param u16 y1     ��� Y ��ֵ����������ֵ��
 * @param u16 x2     �յ� X ��ֵ����������ֵ��
 * @param u16 y2     �յ� Y ��ֵ����������ֵ��
 * @param u16 color  ��ɫ
 */
void LCD_MSP3520_DrawLine( u16 x1, u16 y1, u16 x2, u16 y2, u16 color )
{
    // ��ʼ��
    int x = x1;
    int y = y1;
    
    int dx = x2-x1; dx = dx > 0 ? dx : -dx;
    int dy = y2-y1; dy = dy > 0 ? dy : -dy;
    
    int s1 = x2 > x1 ? 1 : -1;
    int s2 = y2 > y1 ? 1 : -1;
 
    // Ĭ�ϲ����� dx��dy
    // ��б�ʴ��� 1 ʱ��dx��dy ����
    bool interchange = false;    
    if( dy > dx ){
        int temp = dx;
        dx = dy; dy = temp;
        interchange = true;
    }
 
    int p = 2*dy - dx;
    
    for( int i = 0; i < dx; i++ ){
        LCD_MSP3520_DrawPoint(x, y, color);
        
        if( p >= 0 ){
            if( !interchange ) y += s2;  // ��б�� < 1 ʱ��ѡȡ�������ص�
            else x += s1;  // ��б�� > 1 ʱ��ѡȡ�������ص�
            p -= 2 * dx;
        }
        
        if( !interchange ) x += s1;  // ��б�� < 1 ʱ��ѡȡ x Ϊ����
        else y += s2;  // ��б�� > 1 ʱ��ѡȡ y Ϊ����
        p += 2 * dy;
    }
    
    // ��ȫ����һ���յ�
    // ��Ϊ����� Bresenham �㷨������ʼ�㣬����������ֹ��
    LCD_MSP3520_DrawPoint(x2, y2, color);
}


/**
 * ��Բ
 * ʹ�� Bresenham �㷨��Բ
 * �ο���https://blog.csdn.net/weixin_44350205/article/details/105665525
 *
 * X/Y ����һ����������꣺���������������ߺ�����X ��ָ�������꣬Y��ָ��������
 *
 * @param u16  x       ��� X ��ֵ����������ֵ��
 * @param u16  y       ��� Y ��ֵ����������ֵ��
 * @param u8   r       �뾶
 * @param u16  color   ��ɫ
 * @param bool fill    ��䣺����Բ �� ʵ��Բ
 * @param u8   border  �߿򣺿���Բʱ������ڵ��� 1��ʵ��Բʱ��Ч������ �ڱ߿� ��ʽ����Ӱ�������ܳߴ�
 */
void LCD_MSP3520_DrawCircle( u16 x, u16 y, u8 r, u16 color, bool fill, u8 border )
{
    // ��ʼ��
    int tx = 0, ty = r, d = 3 - 2 * r;
    
    // �ع���ʼ����
    x = x + r; y = y + r;
    
    // �ع� border �� fill ����
    border = border < 1 ? 1 : (border>r?r:border);
    if( border == r ) fill = true;
    
    // Bresenham �����㷨
    while( tx <= ty ){
        // ����Բ�İ˷ֶԳ��Ի������
        if( fill == false ){
            LCD_MSP3520_DrawPoint(x-ty, y+tx, color);  // ��1�����У������ң�
            LCD_MSP3520_DrawPoint(x-ty, y-tx, color);
            LCD_MSP3520_DrawPoint(x-tx, y+ty, color);  // ��2�����У������ң�
            LCD_MSP3520_DrawPoint(x-tx, y-ty, color);
            LCD_MSP3520_DrawPoint(x+tx, y+ty, color);  // ��3�����У������ң�
            LCD_MSP3520_DrawPoint(x+tx, y-ty, color);
            LCD_MSP3520_DrawPoint(x+ty, y+tx, color);  // ��4�����У������ң�
            LCD_MSP3520_DrawPoint(x+ty, y-tx, color);
            
            // ���õݹ� LCD_MSP3520_DrawCircle �ķ�ʽ�� Border
            // ÿ�εݹ� ���������+1���뾶-1�� ������Բ���������� Bresenham �㷨��ԭ�򣬲��� border ֱ�ӿ��ܻ���һ����Ŀ�϶��������в���
            if( border > 1 ){
                LCD_MSP3520_DrawPoint(x-ty+1, y+tx, color);  // ��1�����У������ң�
                LCD_MSP3520_DrawPoint(x-ty+1, y-tx, color);
                LCD_MSP3520_DrawPoint(x-tx+1, y+ty, color);  // ��2�����У������ң�
                LCD_MSP3520_DrawPoint(x-tx+1, y-ty, color);
                LCD_MSP3520_DrawPoint(x+tx-1, y+ty, color);  // ��3�����У������ң�
                LCD_MSP3520_DrawPoint(x+tx-1, y-ty, color);
                LCD_MSP3520_DrawPoint(x+ty-1, y+tx, color);  // ��4�����У������ң�
                LCD_MSP3520_DrawPoint(x+ty-1, y-tx, color);
            }
        }else{
            LCD_MSP3520_DrawLine(x-ty, y-tx, x-ty, y+tx, color);  // ��1�����У������ң�
            LCD_MSP3520_DrawLine(x-tx, y-ty, x-tx, y+ty, color);  // ��2�����У������ң�
            LCD_MSP3520_DrawLine(x+tx, y-ty, x+tx, y+ty, color);  // ��3�����У������ң�
            LCD_MSP3520_DrawLine(x+ty, y-tx, x+ty, y+tx, color);  // ��4�����У������ң�
        }

        // ȡ����ĵ�
        if( d < 0 ) d += 4*tx + 6;
        
        // ȡ����ĵ�
        else d += 4*(tx-ty)+10, ty--;
 
        tx++;
    }
    
    // �ݹ黭 Border
    if( fill == false && border > 1 ) LCD_MSP3520_DrawCircle(x+1-r, y+1-r, r-1, color, fill, border-1);
}


/**
 * ������
 * X/Y ����һ����������꣺���������������ߺ�����X ��ָ�������꣬Y��ָ��������
 *
 * @param u16  x       ��� X ��ֵ����������ֵ��
 * @param u16  y       ��� Y ��ֵ����������ֵ��
 * @param u16  xlen    ���ο�ȣ�����ֵ��
 * @param u16  ylen    ���θ߶ȣ�����ֵ��
 * @param u16  color   ��ɫ
 * @param bool fill    ��䣺���ľ��� �� ʵ�ľ���
 * @param u8   border  �߿򣺿��ľ���ʱ������ڵ��� 1��ʵ�ľ���ʱ��Ч������ �ڱ߿� ��ʽ����Ӱ�������ܳߴ�
 */
void LCD_MSP3520_DrawRectangle( u16 x, u16 y, u16 xlen, u16 ylen, u16 color, bool fill, u8 border )
{
    // ��ʼ��
    short i;

    // �ع� xlen �� ylen
    // ���α߿�Ҳ���ڸ߶ȺͿ�ȷ�Χ����
    xlen = xlen-1; ylen = ylen-1;
    
    // �ع� border �� fill ����
    if( border < 1 ) border = 1;
    if( border*2 > xlen || border*2 > ylen ) fill = true;

    // ��ʵ�ľ���
    if( fill ){
        for( i = 0; i <= xlen; i++ ) LCD_MSP3520_DrawLine(i+x, y, i+x, y+ylen, color);
    }

    // �����ľ���
    else{
        // ������
        LCD_MSP3520_DrawLine(x, y, x+xlen, y, color);
        LCD_MSP3520_DrawLine(x, y, x, y+ylen, color);
        LCD_MSP3520_DrawLine(x, y+ylen, x+xlen, y+ylen, color);
        LCD_MSP3520_DrawLine(x+xlen, y, x+xlen, y+ylen, color);
        
        // �ݹ黭 Border
        if( border > 1 ) LCD_MSP3520_DrawRectangle(x+1, y+1, xlen-1, ylen-1, color, fill, border-1);
    }
}


/**
 * ��ʾ�ַ�
 * ��ģ��Ҫ�������з�ʽ���ɣ�ͬʱ���������ֽ�����(sizex/8+((sizex%8)?1:0))*size
 *
 * X/Y ����һ����������꣺���������������ߺ�����X ��ָ�������꣬Y��ָ��������
 *
 * @param u16  x           ��� X ��ֵ����������ֵ��
 * @param u16  y           ��� Y ��ֵ����������ֵ��
 * @param u8   ascii       ASCII ֵ
 * @param u8   size        �����С���� Y ��ֵΪ��׼����֧�֣�12, 14, 16, 24, 32
 * @param u16  color       ǰ��ɫ
 * @param u16  background  ����ɫ
 * @param bool backfill    ��������
 */
void LCD_MSP3520_ShowChar( u16 x, u16 y, u8 ascii, u8 size, u16 color, u16 background, bool backfill )
{
    // ��Ч����
    if( ascii > 32+94 ) return;
    if( !(size==12||size==14||size==16||size==24||size==32) ) return;
    
    // ��ʼ��
    u8 i, bit, m = 0, sizex = size/2;
    u16 xorigin = x;

    // ' ' �ո������� ASCII ֵ�� 32
    // �˴��ո����Ǵ� 0 ��ʼ����������ֱ�Ӽ�ȥ ' '
    ascii = ascii - ' ';

    // һ���ַ���ռ���ֽ���
    // ����ʾһ���ַ�ʱ��Ҫ���͸�LCD�������ֽ���
    u16 lcdCharByte;
    u16 lcdCharBytes = (sizex/8+((sizex%8)?1:0))*size;  

    // ���ü�����������
    LCD_MSP3520_FillSetArea(x, y, x+sizex-1, y+size-1);  
    
    // ѭ���ַ��� LCD �ֽڼ�
    // ���磺ѭ��һ�� 24*12 ���ص��ַ������� GV_LcdAscii1224 �е���ģ������Ҫ��48���ֽ�
    for( i = 0; i < lcdCharBytes; i++ ){
        // ѭ����ȡ�ַ���LCD�ֽ�
        if( size == 12 ) lcdCharByte = GV_LcdAscii0612[ascii][i];  // ���� 6x12  ����
        if( size == 14 ) lcdCharByte = GV_LcdAscii0714[ascii][i];  // ���� 6x12  ����
        if( size == 16 ) lcdCharByte = GV_LcdAscii0816[ascii][i];  // ���� 8x16  ����
        if( size == 24 ) lcdCharByte = GV_LcdAscii1224[ascii][i];  // ���� 12x24 ����
        if( size == 32 ) lcdCharByte = GV_LcdAscii1632[ascii][i];  // ���� 16x32 ����

        // ѭ������ÿ�� LCD �ֽ�λ
        // lcdCharByte&(0x01<<bit) ��ʾȡ��һ���ֽ���� bit λ��ֵ
        for( bit = 0; bit < 8; bit++ ){
            // �������
            if( backfill ){
                LCD_MSP3520_SendColor(lcdCharByte&(0x01<<bit)?color:background);
                if( ++m%sizex == 0 ){ m = 0; break; }
            }
            
            // ��������
            else{
                // ����
                if( lcdCharByte&(0x01<<bit) ) LCD_MSP3520_DrawPoint(x, y, color);
                
                // Y ��������ʼ������һ��
                // ������ģ�������з�ʽ���ɣ�������ʾ�ַ��Ĵ���ҲҪ�������д���
                if( (++x-xorigin) == sizex ){ x = xorigin; y++; break; }
            }
        }
    }              
}


/**
 * ��ʾͼ��
 * X/Y ����һ����������꣺���������������ߺ�����X ��ָ�������꣬Y��ָ��������
 *
 * @param u16  x           ��� X ��ֵ����������ֵ��
 * @param u16  y           ��� Y ��ֵ����������ֵ��
 * @param u8   index       ͼ������
 * @param u8   size        ͼ���С���� Y ��ֵΪ��׼����֧�֣�16, 24, 32
 * @param u16  color       ǰ��ɫ
 * @param u16  background  ����ɫ
 * @param bool backfill    ��������
 */
void LCD_MSP3520_ShowIcon( u16 x, u16 y, u8 index, u8 size, u16 color, u16 background, bool backfill )
{
    // ��Ч����
    if( !(size==16||size==24||size==32) ) return;
    
    // ��ʼ��
    u8 i, bit, m = 0;
    u16 lcdIconByte, xorigin = x;

    // ���ü�����������
    LCD_MSP3520_FillSetArea(x, y, x+size-1, y+size-1);  

    // ѭ��ͼ��� LCD �ֽڼ�
    // ���磺ѭ��һ�� 16*16 ���ص�ͼ�꣬���� GV_LcdIcon16 �е���ģ������Ҫ��32���ֽ�
    for( i = 0; i < size*size/8; i++ ){
        // ѭ����ȡ�ַ���LCD�ֽ�
        if( size == 16 ) lcdIconByte = GV_LcdIcon16[index][i];
        if( size == 24 ) lcdIconByte = GV_LcdIcon24[index][i];
        if( size == 32 ) lcdIconByte = GV_LcdIcon32[index][i];

        // ѭ������ÿ�� LCD �ֽ�λ
        // lcdIconByte&(0x01<<bit) ��ʾȡ��һ���ֽ���� bit λ��ֵ
        for( bit = 0; bit < 8; bit++ ){
            // �������
            if( backfill ){
                LCD_MSP3520_SendColor(lcdIconByte&(0x01<<bit)?color:background);
                if( ++m%size == 0 ){ m = 0; break; }
            }
            
            // ��������
            else{
                // ����
                if( lcdIconByte&(0x01<<bit) ) LCD_MSP3520_DrawPoint(x, y, color);
                
                // Y ��������ʼ������һ��
                // ������ģ�������з�ʽ���ɣ�������ʾ�ַ��Ĵ���ҲҪ�������д���
                if( (++x-xorigin) == size ){ x = xorigin; y++; break; }
            }
        }
    }   
}


/**
 * ��ʾ�ַ���
 * X/Y ����һ����������꣺���������������ߺ�����X ��ָ�������꣬Y��ָ��������
 *
 * @param u16  x           ��� X ��ֵ����������ֵ��
 * @param u16  y           ��� Y ��ֵ����������ֵ��
 * @param u8   *fonts      �ּ�
 * @param u8   size        �����С���� Y ��ֵΪ��׼����֧�֣�12, 14, 16, 24, 32
 * @param u16  color       ǰ��ɫ
 * @param u16  background  ����ɫ
 * @param bool backfill    ��������
 */
void LCD_MSP3520_ShowString( u16 x, u16 y, u8 *fonts, u8 size, u16 color, u16 background, bool backfill )
{      
    // ��Ч����
    if( !(size==12||size==14||size==16||size==24||size==32) ) return;
    
    // ���ַ���
    while( *fonts != '\0' ){       
        LCD_MSP3520_ShowChar(x, y, *fonts, size, color, background, backfill);
        x += size/2; fonts++;
    }  
}


/**
 * ��ʾ����
 * X/Y ����һ����������꣺���������������ߺ�����X ��ָ�������꣬Y��ָ��������
 *
 * @param u16  x           ��� X ��ֵ����������ֵ��
 * @param u16  y           ��� Y ��ֵ����������ֵ��
 * @param u8   *fonts      �ּ�
 * @param u8   size        �����С���� Y ��ֵΪ��׼����֧�֣�12, 14, 16, 24, 32
 * @param u16  color       ǰ��ɫ
 * @param u16  background  ����ɫ
 * @param bool backfill    ��������
 */
void LCD_MSP3520_ShowChinese( u16 x, u16 y, u8 *fonts, u8 size, u16 color, u16 background, bool backfill )
{
    // ��Ч����
    if( !(size==12||size==14||size==16||size==24||size==32) ) return;
    
    // ������
    while( *fonts != 0 ){
        LCD_MSP3520_ShowChineseOne(x, y, fonts, size, color, background, backfill);
        fonts += 2; x += size;
    }
}


/**
 * ��ʾ������
 * ��ģ��Ҫ�������з�ʽ���ɣ�ͬʱ���������ֽ�����(size/8+((size%8)?1:0))*size
 *
 * X/Y ����һ����������꣺���������������ߺ�����X ��ָ�������꣬Y��ָ��������
 *
 * @param u16  x           ��� X ��ֵ����������ֵ��
 * @param u16  y           ��� Y ��ֵ����������ֵ��
 * @param u8   *font       ������
 * @param u16  color       ǰ��ɫ
 * @param u16  background  ����ɫ
 * @param bool backfill    ��������
 */
void LCD_MSP3520_ShowChineseOne( u16 x, u16 y, u8 *font, u8 size, u16 color, u16 background, bool backfill )
{
    // ��ʼ��
    u16 xorigin = x;
    u8 i, k, bit, find;
    short m = 0, total = 0;

    // ��ʼ���ּ�����
    if( size == 12 ) total = GV_LcdChinese12Total;
    if( size == 14 ) total = GV_LcdChinese14Total;
    if( size == 16 ) total = GV_LcdChinese16Total;
    if( size == 24 ) total = GV_LcdChinese24Total;
    //if( size == 32 ) total = GV_LcdChinese32Total;

    // һ��������ռ���ֽ���
    // ����ʾһ���ַ�ʱ��Ҫ���͸�LCD�������ֽ���
    u16 lcdCharBytes = (size/8+((size%8)?1:0))*size;

    // ѭ�����ּ����ҵ�Ҫ��ʾ�Ķ�Ӧ����
    for( k = 0; k < total; k++ ){
        // ���Ҷ�Ӧ�ĵ�����
        find = false;
        if( size == 12 && (GV_LcdChinese12[k].Index[0]==*(font)) && (GV_LcdChinese12[k].Index[1]==*(font+1)) ) find = true;
        if( size == 14 && (GV_LcdChinese14[k].Index[0]==*(font)) && (GV_LcdChinese14[k].Index[1]==*(font+1)) ) find = true;
        if( size == 16 && (GV_LcdChinese16[k].Index[0]==*(font)) && (GV_LcdChinese16[k].Index[1]==*(font+1)) ) find = true;
        if( size == 24 && (GV_LcdChinese24[k].Index[0]==*(font)) && (GV_LcdChinese24[k].Index[1]==*(font+1)) ) find = true;
        //if( size == 32 && (GV_LcdChinese32[k].Index[0]==*(font)) && (GV_LcdChinese32[k].Index[1]==*(font+1)) ) find = true;  // ��Լ��Ƭ�� ROM �ռ䣬Ĭ��ע�͵�
        if( find == false ) continue;
        
        // ���ü�����������
        LCD_MSP3520_FillSetArea(x, y, x+size-1, y+size-1);

        // ѭ���ַ��� LCD �ֽڼ�
        for( i = 0; i < lcdCharBytes; i++ ){
            // ѭ������ÿ�� LCD �ֽ�λ
            // GV_LcdChineseXX[k].Masks[i]&(0x01<<bit) ��ʾȡ��һ���ֽ���� bit λ��ֵ
            for( bit=0; bit<8; bit++ ){    
                // �������
                if( backfill ){
                    if( size == 12 ) LCD_MSP3520_SendColor(GV_LcdChinese12[k].Masks[i]&(0x01<<bit)?color:background);
                    if( size == 14 ) LCD_MSP3520_SendColor(GV_LcdChinese14[k].Masks[i]&(0x01<<bit)?color:background);
                    if( size == 16 ) LCD_MSP3520_SendColor(GV_LcdChinese16[k].Masks[i]&(0x01<<bit)?color:background);
                    if( size == 24 ) LCD_MSP3520_SendColor(GV_LcdChinese24[k].Masks[i]&(0x01<<bit)?color:background);
                    //if( size == 32 ) LCD_MSP3520_SendColor(GV_LcdChinese32[k].Masks[i]&(0x01<<bit)?color:background);  // ��Լ��Ƭ�� ROM �ռ䣬Ĭ��ע�͵�
                    if( ++m%size == 0 ){ m = 0; break; }
                }

                // ��������
                else{
                    // ����
                    if( size == 12 && GV_LcdChinese12[k].Masks[i]&(0x01<<bit) ) LCD_MSP3520_DrawPoint(x, y, color);
                    if( size == 14 && GV_LcdChinese14[k].Masks[i]&(0x01<<bit) ) LCD_MSP3520_DrawPoint(x, y, color);
                    if( size == 16 && GV_LcdChinese16[k].Masks[i]&(0x01<<bit) ) LCD_MSP3520_DrawPoint(x, y, color);
                    if( size == 24 && GV_LcdChinese24[k].Masks[i]&(0x01<<bit) ) LCD_MSP3520_DrawPoint(x, y, color);  // ��Լ��Ƭ�� ROM �ռ䣬Ĭ��ע�͵�
                    //if( size == 32 && GV_LcdChinese32[k].Masks[i]&(0x01<<bit) ) LCD_MSP3520_DrawPoint(x, y, color);  // ��Լ��Ƭ�� ROM �ռ䣬Ĭ��ע�͵�

                    // Y ��������ʼ������һ��
                    // ������ģ�������з�ʽ���ɣ�������ʾ�ַ��Ĵ���ҲҪ�������д���
                    if( (++x-xorigin) == size ){ x = xorigin; y++; break; }
                }
            }
        }
        
        // ���ҵ���Ӧ�����ֿ������˳�
        // ��ֹ��������ظ�ȡģ����Ӱ��
        break;  
    }
}


/**
 * ��ʾͼƬ
 * ʹ�� Image2Lcd ���ɵ����ݣ�����ʱ����Ҫѡ��ˮƽɨ�裬16λ���ɫ����ɫ��565��
 *
 * X/Y ����һ����������꣺���������������ߺ�����X ��ָ�������꣬Y��ָ��������
 *
 * @param u16  x           ��� X ��ֵ����������ֵ��
 * @param u16  y           ��� Y ��ֵ����������ֵ��
 * @param u16  xlen        ͼƬ��ȣ�����ֵ��
 * @param u16  ylen        ͼƬ�߶ȣ�����ֵ��
 * @param u8*  image       ͼƬ����ָ��
 * @param bool reverse     ��ɫ�Ƿ�ȡ��
 */
void LCD_MSP3520_ShowImage( u16 x, u16 y, u16 xlen, u16 ylen, const u8 image[], bool reverse )
{
    // ��ʼ��
    u16 i, j;
    u32 datai = 0;
    
    // ���ü�����������
    LCD_MSP3520_FillSetArea(x, y, x+xlen-1, y+ylen-1);
    
    // ��˳������ɫ����
    for( i = 0; i < xlen; i++ ){
        for( j = 0; j < ylen; j++ ){
            LCD_MSP3520_SendData8(reverse?~image[datai*2]:image[datai*2]);
            LCD_MSP3520_SendData8(reverse?~image[datai*2+1]:image[datai*2]);
            datai++;
        }
    }            
}


/**
 * ��ʾ����
 * �Զ�ѡ�񸡵�����������
 * ���ִӵ�λ��ʼ��ʾ�������ʾ�ĳ���С�����ֳ��ȣ���ô���ָ�λ����ʾ
 * X/Y ����һ����������꣺���������������ߺ�����X ��ָ�������꣬Y��ָ��������
 *
 * @param u16   x           ��� X ��ֵ����������ֵ��
 * @param u16   y           ��� Y ��ֵ����������ֵ��
 * @param float number      ������
 * @param u8    size        �����С���� Y ��ֵΪ��׼����֧�֣�12, 14, 16, 24, 32
 * @param u16   color       ǰ��ɫ
 * @param u16   background  ����ɫ
 * @param u8    backfill    ��������
 * @param short showlen     Ҫ��ʾ�ĳ��ȣ��������ź�С���㣩�����ֳ��Ȳ���ʱ�ո�ȫ��0 ��ʾ�Զ�ʶ�𳤶ȣ�������ʾ����룬������ʾ�Ҷ���
 *
 * @param short ����ʵ����ʾ�����ָ���������С����͸��ţ�
 */
short LCD_MSP3520_ShowNumber( u16 x, u16 y, double number, u8 size, u16 color, u16 background, bool backfill, short showlen )
{
    long intnum = ((long)(number*100));
    if( intnum%100 == 0 ) return LCD_MSP3520_ShowInteger(x, y, (long)number, size, color, background, backfill, showlen);
    else return LCD_MSP3520_ShowFloat(x, y, number, size, color, background, backfill, showlen );
}
void LCD_MSP3520_ShowSplitNumber( u16 x, u16 y, short *splitnums, short splitindex, u8 size, u16 color, u16 background, bool backfill, short showlen )
{
    // ��ʼ��
    short i, l;
    short sizex = size/2;
    
    // ȫ����ʾ
    if( showlen == 0 ){
        for( i = 0; i < splitindex; i++ ){
            if( splitnums[i] >=  0 ) LCD_MSP3520_ShowChar(x+i*sizex, y, splitnums[i]+48, size, color, background, backfill);
            if( splitnums[i] == -1 ) LCD_MSP3520_ShowChar(x+i*sizex, y, '.', size, color, background, backfill);
            if( splitnums[i] == -2 ) LCD_MSP3520_ShowChar(x+i*sizex, y, '-', size, color, background, backfill);
        }
    }
    
    // ��ʾ�������ƣ�����룩
    if( showlen > 0 ){
        for( l = 0; l < showlen; l++ ){
            i = l;
            if( i < splitindex ){
                if( splitnums[i] >=  0 ) LCD_MSP3520_ShowChar(x+l*sizex, y, splitnums[i]+48, size, color, background, backfill);
                if( splitnums[i] == -1 ) LCD_MSP3520_ShowChar(x+l*sizex, y, '.', size, color, background, backfill);
                if( splitnums[i] == -2 ) LCD_MSP3520_ShowChar(x+l*sizex, y, '-', size, color, background, backfill);
            }else{
                LCD_MSP3520_ShowChar(x+l*sizex, y, ' ', size, color, background, backfill);
            }
        }
    }
    
    // ��ʾ�������ƣ��Ҷ��룩
    if( showlen < 0 ){
        showlen = -showlen;
        for( l = showlen; l > 0; l-- ){
            i = splitindex - (showlen-l) - 1;
            if( i >= 0 && i < splitindex ){
                if( splitnums[i] >= 0  ) LCD_MSP3520_ShowChar(x+(l-1)*sizex, y, splitnums[i]+48, size, color, background, backfill);
                if( splitnums[i] == -1 ) LCD_MSP3520_ShowChar(x+(l-1)*sizex, y, '.', size, color, background, backfill);
                if( splitnums[i] == -2 ) LCD_MSP3520_ShowChar(x+(l-1)*sizex, y, '-', size, color, background, backfill);
            }else{
                LCD_MSP3520_ShowChar(x+(l-1)*sizex, y, ' ', size, color, background, backfill);
            }
        }
    }
}

/**
 * ��ʾ������
 * Ĭ����ʾ��λС����
 * ���ִӵ�λ��ʼ��ʾ�������ʾ�ĳ���С�����ֳ��ȣ���ô���ָ�λ����ʾ
 * X/Y ����һ����������꣺���������������ߺ�����X ��ָ�������꣬Y��ָ��������
 *
 * @param u16   x           ��� X ��ֵ����������ֵ��
 * @param u16   y           ��� Y ��ֵ����������ֵ��
 * @param float number      ������
 * @param u8    size        �����С���� Y ��ֵΪ��׼����֧�֣�12, 14, 16, 24, 32
 * @param u16   color       ǰ��ɫ
 * @param u16   background  ����ɫ
 * @param bool  backfill    ��������
 * @param short showlen     Ҫ��ʾ�ĳ��ȣ��������ź�С���㣩�����ֳ��Ȳ���ʱ�ո�ȫ��0 ��ʾ�Զ�ʶ�𳤶ȣ�������ʾ����룬������ʾ�Ҷ���
 *
 * @param short ����ʵ����ʾ�����ָ���������С����͸��ţ�
 */
short LCD_MSP3520_ShowFloat( u16 x, u16 y, double number, u8 size, u16 color, u16 background, bool backfill, short showlen )
{
    // ��ʼ��
    short i, onenum;

    long  intnum = (long)(number*100);
    long  absintnum = intnum >= 0 ? intnum : -intnum;
    long  absintnumsub = intnum >= 0 ? intnum : -intnum;

    short splitnums[15] = {-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9};  // 0~9 ��ʾ���֣�-1 ��ʾ ��.����-2 ��ʾ ��-����-9 ��ʾ��ʼֵ
    short splitindex = 0;

    // �ֽ�����
    while( absintnumsub > 0 ){
        // ��ȡ����
        onenum = absintnumsub%10;
        absintnumsub = absintnumsub/10;
        
        // �������ݺ���һλ���ڳ�����Ϊ 0 ������ռ䲢��ֵ��ǰ�ֽ���ĵ�������
        for( i = splitindex++; i > 0; i-- ) splitnums[i] = splitnums[i-1];
        splitnums[0] = onenum;
        
        // С���㴦��
        if( (splitindex==1&&absintnum<10) || splitindex == 2 ){
            for( i = splitindex++; i > 0; i-- ) splitnums[i] = splitnums[i-1];
            splitnums[0] = -1;
        }
    }
    
    // �������С�� 1����ô��ֵǰ�� ��0��
    // ������� number = 0.12����������� ���ֽ����֡� ����ηֽ�󣬻��� .12����������Ĵ����������ǰ�渳ֵһ��0����� 0.12
    if( absintnum > 0 && absintnum < 100 ){
        for( i = splitindex++; i > 0; i-- ) splitnums[i] = splitnums[i-1];
        splitnums[0] = 0;
    }

    // ������ֵ��� 0����ô��ֵ 0
    // ����� ���ֽ����֡� ����β����ᴦ�� number = 0 ʱ�����������Ҫ��������
    if( intnum == 0 ){
        for( i = splitindex++; i > 0; i-- ) splitnums[i] = splitnums[i-1]; splitnums[0] = 0;
        for( i = splitindex++; i > 0; i-- ) splitnums[i] = splitnums[i-1]; splitnums[0] = 0;
        for( i = splitindex++; i > 0; i-- ) splitnums[i] = splitnums[i-1]; splitnums[0] = -1;
        for( i = splitindex++; i > 0; i-- ) splitnums[i] = splitnums[i-1]; splitnums[0] = 0;
    }
    
    // �������С�� 0����ô��ֵǰ�� ������ ��
    if( intnum < 0 ){
        for( i = splitindex++; i > 0; i-- ) splitnums[i] = splitnums[i-1];
        splitnums[0] = -2;
    }
    
    // ��ʾ���
    LCD_MSP3520_ShowSplitNumber(x, y, splitnums, splitindex, size, color, background, backfill, showlen);
        
    // ����ʵ����ʾ�����ָ���������С����͸��ţ�
    showlen = showlen > 0 ? showlen : -showlen;
    return showlen == 0 ? splitindex : (showlen>splitindex?splitindex:showlen);
}


/**
 * ��ʾ����
 * ���ִӵ�λ��ʼ��ʾ�������ʾ�ĳ���С�����ֳ��ȣ���ô���ָ�λ����ʾ
 * X/Y ����һ����������꣺���������������ߺ�����X ��ָ�������꣬Y��ָ��������
 *
 * @param u16   x           ��� X ��ֵ����������ֵ��
 * @param u16   y           ��� Y ��ֵ����������ֵ��
 * @param long  number      ��������
 * @param u8    size        �����С���� Y ��ֵΪ��׼����֧�֣�12, 14, 16, 24, 32
 * @param u16   color       ǰ��ɫ
 * @param u16   background  ����ɫ
 * @param bool  backfill    ��������
 * @param short showlen     Ҫ��ʾ�ĳ��ȣ��������ţ������ֳ��Ȳ���ʱ�ո�ȫ��0 ��ʾ�Զ�ʶ�𳤶ȣ�������ʾ����룬������ʾ�Ҷ���
 *
 * @param short ����ʵ����ʾ�����ָ���������С����͸��ţ�
 */
short LCD_MSP3520_ShowInteger( u16 x, u16 y, long number, u8 size, u16 color, u16 background, bool backfill, short showlen )
{
    // ��ʼ��
    short i, onenum;
    long  absnumsub = number < 0 ? -number : number;

    short splitnums[15] = {-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9};  // 0~9 ��ʾ���֣�-1 ��ʾ ��.����-2 ��ʾ ��-����-9 ��ʾ��ʼֵ
    short splitindex = 0;

    // �ֽ�����
    while( absnumsub > 0 ){
        // ��ȡ����
        onenum = absnumsub%10;
        absnumsub = absnumsub/10;
        
        // �������ݺ���һλ���ڳ�����Ϊ 0 ������ռ䲢��ֵ��ǰ�ֽ���ĵ�������
        for( i = splitindex++; i > 0; i-- ) splitnums[i] = splitnums[i-1];
        splitnums[0] = onenum;
    }
    
    // ������ֵ��� 0����ô��ֵ 0
    // ����� ���ֽ����֡� ����β����ᴦ�� number = 0 ʱ�����������Ҫ��������
    if( number == 0 ){
        for( i = splitindex++; i > 0; i-- ) splitnums[i] = splitnums[i-1];
        splitnums[0] = 0;
    }

    // �������С�� 0����ô��ֵǰ�� ������ ��
    if( number < 0 ){
        for( i = splitindex++; i > 0; i-- ) splitnums[i] = splitnums[i-1];
        splitnums[0] = -2;
    }

    // ��ʾ���
    LCD_MSP3520_ShowSplitNumber(x, y, splitnums, splitindex, size, color, background, backfill, showlen);
    
    // ����ʵ����ʾ�����ָ���������С����͸��ţ�
    showlen = showlen > 0 ? showlen : -showlen;
    return showlen == 0 ? splitindex : (showlen>splitindex?splitindex:showlen);
} 

