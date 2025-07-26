#include "common.h"



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
//extern const u8 GV_LcdAscii1224[][48];  // ��Լ��Ƭ�� ROM �ռ䣬Ĭ��ע�͵�
//extern const u8 GV_LcdAscii1632[][64];  // ��Լ��Ƭ�� ROM �ռ䣬Ĭ��ע�͵�

extern const u8 GV_LcdIcon16[][32];
extern const u8 GV_LcdIcon24[][72];
extern const u8 GV_LcdIcon32[][128];

extern const short GV_LcdChinese12Total;
extern const short GV_LcdChinese14Total;
extern const short GV_LcdChinese16Total;
//extern const short GV_LcdChinese24Total;
//extern const short GV_LcdChinese32Total;

extern const struct STRUCT_LCD_CHINESE12 GV_LcdChinese12[];
extern const struct STRUCT_LCD_CHINESE14 GV_LcdChinese14[];
extern const struct STRUCT_LCD_CHINESE16 GV_LcdChinese16[];
//extern const struct STRUCT_LCD_CHINESE24 GV_LcdChinese24[];
//extern const struct STRUCT_LCD_CHINESE32 GV_LcdChinese32[];



/* -------------------------------------------------- */
// �� LCD ST7735S ��غ���
/* -------------------------------------------------- */


/**
 * ��ʼ������
 */
void LCD_ST7735S_Init()
{
    // ��ʼ�� LCD ST7735S ���� IO
    LCD_ST7735S_InitIO();

    
    // Һ������λ
    // ������븴λ���룬�����һ���ϵ�ʱ���ܳ����޷����������
    GPIO_WriteLow(LCD_ST7735S_RESET_PORT, LCD_ST7735S_RESET_PIN); delay_ms(100);
    GPIO_WriteHigh(LCD_ST7735S_RESET_PORT, LCD_ST7735S_RESET_PIN); delay_ms(100);
    
    
    /* ���µ������ʼ�����ɹٷ�ʾ�������и�����ֱ��ʹ�ü��� */
    
    
    // ˯���˳�
    // �ο������ֲ�� 120 ҳ
    // �ӳ� 120ms ���ֲ��Ϲ涨
	LCD_ST7735S_SendCmd(0x11);
	delay_ms(120);
    
    
    //--------------------- Frame Rate ---------------------//
    // ֡�ʿ���
    // �ο������ֲ�� 159+ ҳ
    
    // ����ģʽʱ֡�ʿ��ƣ���� Full Colors ��ɫ��
    // Frame rate = fosc/((RTNA x 2 + 40) x (LINE + FPA + BPA +2))
	LCD_ST7735S_SendCmd(0xB1);     
	LCD_ST7735S_SendData8(0x05);  // RTNA
	LCD_ST7735S_SendData8(0x3C);  // FPA
	LCD_ST7735S_SendData8(0x3C);  // BPA

    // ����ģʽʱ֡�ʿ��ƣ���� 8 Colors ��ɫ��
    // Frame rate = fosc/((RTNA x 2 + 40) x (LINE + FPB + BPB +2))
	LCD_ST7735S_SendCmd(0xB2);     
	LCD_ST7735S_SendData8(0x05);  // RTNB 
	LCD_ST7735S_SendData8(0x3C);  // FPB
	LCD_ST7735S_SendData8(0x3C);  // BPB

    // �ֲ�ģʽʱ֡�ʿ��ƣ����Full Colors ��ɫ��
    // Frame rate=fosc/((RTNA x 2 + 40) x (LINE + FPC + BPC +2))
    // SendData8()��1st parameter to 3rd parameter are used in dot inversion mode
    // SendData8()��4th parameter to 6th parameter are used in column inversion mode.
	LCD_ST7735S_SendCmd(0xB3);     
	LCD_ST7735S_SendData8(0x05);  // RTNC
	LCD_ST7735S_SendData8(0x3C);  // FPC
	LCD_ST7735S_SendData8(0x3C);  // BPC
	LCD_ST7735S_SendData8(0x05);  // RTND
	LCD_ST7735S_SendData8(0x3C);  // FPD
	LCD_ST7735S_SendData8(0x3C);  // BPD

    
    //--------------------- Display Inversion Control ---------------------//
    // ��ת��ʾ����
    // �ο������ֲ�� 162 ҳ
    // ��Ҫ���� �㷴ת(Dot Inversion) �� �з�ת(Column Inversion) ���� 
    
    // Һ����ʾ������ʾͼ��ʱ�����ɱ���Ļ����ֱ���������󣬴Ӷ�Ӱ��ʹ�á�Ϊ�˱���ֱ�������ķ���������ʹʩ�ӵ�ѹ��ƽ��ֵΪ�㣬����Ҫ�����ؼ��Է�ת
    // ���м����е����ֻ������м��Է�ת��ʽ��Frame inversion (���л���ȫ����ת)��Row inversion (���л��ذ��з�ת)��Column inversion (���л��ذ��з�ת)��Dot inversion (���л���ȫ����ת)
    // �Ƚ���������֡���棺
    //   1. Frame inversion  ���������滭�ش����ѹ������ͬ�������Ի��߸����ԣ������ص�����������ļ���ͳһ�仯
    //   2. Row inversion    �Ǻ���ÿһ�еļ�����ͬ�������еļ����෴����һ������ʱ����ͬʱ�仯
    //   3. Column inversion ��ÿһ�еļ�����ͬ�������м����෴
    //   4. Dot inversion    �����ڵ�ÿ�����ص�ļ��Զ���ͬ
    // 
    // Frame inversion��ʾЧ����Row inversion �� Column inversion��֮������ʾ������˸��������ԣ��ǲ��ܿ˷���ȱ�㣬�ߴ�Խ����˸����Խ���ԡ�
    // Ŀǰ�������� 7" ���� LCD ����ѡ�� Dot inversion ����ƣ���Ȼ Dot inversion ��ʾЧ���Ϻã����Ǵ�������ʽ���� Dot inversion ����������Ҳ����ӵ�
    // ͬʱ������ Dot inversion ��ÿ�����ص�ļ��Զ���ͬ��LCM�����ص����������RGB, RGB, RGB...,������һ��RGB�л������ֲ�ͬ�ļ��ԣ�������ʹRGB�����ʾ������Ч�����Ǻ���ʵ��ʹ����ʾ�����Ļ���ɫ�ʳ���һ����ƫ�
	LCD_ST7735S_SendCmd(0xB4);
	LCD_ST7735S_SendData8(0x03);   

    
    //--------------------- Power Sequence ---------------------//
    // ��Դ����ʱ��
    // �ο������ֲ�� 163 ҳ

	LCD_ST7735S_SendCmd(0xC0);     
	LCD_ST7735S_SendData8(0x0E);   
	LCD_ST7735S_SendData8(0x0E);   
	LCD_ST7735S_SendData8(0x04);   

	LCD_ST7735S_SendCmd(0xC1);     
	LCD_ST7735S_SendData8(0xC5);   

	LCD_ST7735S_SendCmd(0xC2);     
	LCD_ST7735S_SendData8(0x0d);   
	LCD_ST7735S_SendData8(0x00);   

	LCD_ST7735S_SendCmd(0xC3);     
	LCD_ST7735S_SendData8(0x8D);   
	LCD_ST7735S_SendData8(0x2A);   

	LCD_ST7735S_SendCmd(0xC4);     
	LCD_ST7735S_SendData8(0x8D);   
	LCD_ST7735S_SendData8(0xEE);   
    
    
    //--------------------- VCOM Control ---------------------//
    // VCOM ��ѹ����
    // �ο������ֲ�� 173 ҳ
    
	LCD_ST7735S_SendCmd(0xC5);
	LCD_ST7735S_SendData8(0x06);
    
    
    //--------------------- Memory Data Access Control ---------------------//
    // �ڴ����ݷ��ʿ���
    // �ο������ֲ�� 75 ҳ, �� 142 ҳ
    //
    // This command defines read/write scanning direction of frame memory
    // ����λ��MY(Mirror Y-axis),    MX(Mirror X-axis),                  MV,                     ML,           RGB,                       MH, -, -
    // ����λ��Row Address Order, Column Address Order, Row/Column Exchange, Vertical Refresh Order, RGB-BGR ORDER, Horizontal Refresh Order, -, -
    //
    // MV=0 ��ʾ����, MV=1 ��ʾ����
    // MY �� MX ��ʾ���꾵��ͨ���������ÿ��Խ����� 0,0 �󶨵���Ļ������/����/����/����
    //
    //
    // �� LCD ST7735S ���������о�԰ 0.96�� 80*160 ����Ļ
    // �� ����ʱ���� ST7735S ��˵ 80  ��������(x)����Ҫ +23 ����ƫ��
    // �� ����ʱ���� ST7735S ��˵ 160 ��������(y)����Ҫ +2  ����ƫ��
    // 
    // �� ����ʱ���� ST7735S ��˵ 80  ��������(y)����Ҫ +23 ����ƫ��
    // �� ����ʱ���� ST7735S ��˵ 160 ��������(x)����Ҫ +2  ����ƫ��
    
	LCD_ST7735S_SendCmd(0x36);     
	if( LCD_ST7735S_SCREEN == LCD_ST7735S_SCREEN_SHU  ) LCD_ST7735S_SendData8(0xC8);
	if( LCD_ST7735S_SCREEN == LCD_ST7735S_SCREEN_HEN ) LCD_ST7735S_SendData8(0xA8);
        
    
    //--------------------- Interface Pixel Format ---------------------//
    // �ӿ����ظ�ʽ
    // �ο������ֲ�� 150 ҳ
    
	LCD_ST7735S_SendCmd(0x3A); 
	LCD_ST7735S_SendData8(0x55);
    
		
    //------------------------------------ST7735S Gamma Sequence-----------------------------------------//
	LCD_ST7735S_SendCmd(0xE0);     
	LCD_ST7735S_SendData8(0x0b);   
	LCD_ST7735S_SendData8(0x17);   
	LCD_ST7735S_SendData8(0x0a);   
	LCD_ST7735S_SendData8(0x0d);   
	LCD_ST7735S_SendData8(0x1a);   
	LCD_ST7735S_SendData8(0x19);   
	LCD_ST7735S_SendData8(0x16);   
	LCD_ST7735S_SendData8(0x1d);   
	LCD_ST7735S_SendData8(0x21);   
	LCD_ST7735S_SendData8(0x26);   
	LCD_ST7735S_SendData8(0x37);   
	LCD_ST7735S_SendData8(0x3c);   
	LCD_ST7735S_SendData8(0x00);   
	LCD_ST7735S_SendData8(0x09);   
	LCD_ST7735S_SendData8(0x05);   
	LCD_ST7735S_SendData8(0x10);   

	LCD_ST7735S_SendCmd(0xE1);     
	LCD_ST7735S_SendData8(0x0c);   
	LCD_ST7735S_SendData8(0x19);   
	LCD_ST7735S_SendData8(0x09);   
	LCD_ST7735S_SendData8(0x0d);   
	LCD_ST7735S_SendData8(0x1b);   
	LCD_ST7735S_SendData8(0x19);   
	LCD_ST7735S_SendData8(0x15);   
	LCD_ST7735S_SendData8(0x1d);   
	LCD_ST7735S_SendData8(0x21);   
	LCD_ST7735S_SendData8(0x26);   
	LCD_ST7735S_SendData8(0x39);   
	LCD_ST7735S_SendData8(0x3E);   
	LCD_ST7735S_SendData8(0x00);   
	LCD_ST7735S_SendData8(0x09);   
	LCD_ST7735S_SendData8(0x05);   
	LCD_ST7735S_SendData8(0x10);   
	 
	delay_ms(120);
	LCD_ST7735S_SendCmd(0x29);     // Display on
}


/**
 * ��ʼ�� LCD ST7735S ���� IO
 */
void LCD_ST7735S_InitIO()
{
    // ��������
    // �͵�ƽʱ LCD ���������ߵ�ƽ/���չ���
    GPIO_Init(LCD_ST7735S_RESET_PORT, LCD_ST7735S_RESET_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    
    // RS ����
    // ���� ����͵�ƽ�� ���� ���ݣ��ߵ�ƽ,Ĭ�ϣ� �Ŀ�������
    GPIO_Init(LCD_ST7735S_RS_PORT, LCD_ST7735S_RS_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);

    // SPI Ƭѡ����
    // �͵�ƽʱ SPI �������� LCD ͨѶ
    GPIO_Init(LCD_ST7735S_CS_PORT, LCD_ST7735S_CS_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    
    
    // SPI IO��ʼ
    GPIO_Init(LCD_ST7735S_SCL_PORT, LCD_ST7735S_SCL_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
    GPIO_Init(LCD_ST7735S_SDA_PORT, LCD_ST7735S_SDA_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
  
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
    SPI_Init(SPI_FIRSTBIT_MSB, SPI_BAUDRATEPRESCALER_4, SPI_MODE_MASTER, SPI_CLOCKPOLARITY_LOW, SPI_CLOCKPHASE_1EDGE, SPI_DATADIRECTION_1LINE_TX, SPI_NSS_SOFT, 0x01);
    
    // SPI ����
    SPI_Cmd(ENABLE);
}

/**
 * �� LCD ST7735S ��������
 * �������ɺ������лط�������ģʽ��Ҳ����˵���� LCD ST7735S ��������Ĭ��ģʽ�¶��Ƿ�������
 *
 * @param u8 data  �����ֽ�
 */
void LCD_ST7735S_SendCmd( u8 data )
{
    // ���÷�������ģʽ
	GPIO_WriteLow(LCD_ST7735S_RS_PORT, LCD_ST7735S_RS_PIN);
    
    // ��������
	LCD_ST7735S_SendByte(data);
    
    // ���÷�������ģʽ��Ĭ�ϣ�
    GPIO_WriteHigh(LCD_ST7735S_RS_PORT, LCD_ST7735S_RS_PIN);
}


/**
 * �� LCD ST7735S ��������
 *
 * @param u8 data  �����ֽ�
 */
void LCD_ST7735S_SendData8( u8 data )
{
	LCD_ST7735S_SendByte(data);
}
void LCD_ST7735S_SendData16( u16 data )
{
	LCD_ST7735S_SendByte(data>>8);
	LCD_ST7735S_SendByte(data);
}


/**
 * �� LCD ST7735S ��������
 *
 * @param u8 byte  �����ֽ�
 */
void LCD_ST7735S_SendByte( u8 byte )
{
    // �ȴ� SPI ����
    while(SPI_GetFlagStatus(SPI_FLAG_BSY));
    
    // SPI Ƭѡ�������ͣ���������������
    GPIO_WriteLow(LCD_ST7735S_CS_PORT, LCD_ST7735S_CS_PIN);
    
    // �������ݲ��ȴ��������
    SPI_SendData(byte);
    while( !SPI_GetFlagStatus(SPI_FLAG_TXE) );

    // SPI Ƭѡ�������ߣ��ͷź����������ӡ��Ա������������� SPI �ӻ�����
    GPIO_WriteHigh(LCD_ST7735S_CS_PORT, LCD_ST7735S_CS_PIN);
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
void LCD_ST7735S_Fill( u16 xBeg, u16 yBeg, u16 xEnd, u16 yEnd, u16 color )
{      
    // ��ʼ��
	u16 i,j; 
    
    // ���ü�����������
	LCD_ST7735S_FillSetArea(xBeg, yBeg, xEnd, yEnd);
    
    // ѭ����������
	for( i = yBeg; i < yEnd; i++ ) for( j = xBeg; j < xEnd; j++ ) LCD_ST7735S_SendData16(color); 
}


/**
 * ���ü�����������
 * X/Y ����һ����������꣺���������������ߺ�����X ��ָ�������꣬Y��ָ��������
 * 
 * @param u16 xBeg  ��ʼ��ַ X ��ֵ����������ֵ��
 * @param u16 yBeg  ��ʼ��ַ Y ��ֵ����������ֵ��
 * @param u16 xEnd  ������ַ X ��ֵ����������ֵ��
 * @param u16 yEnd  ������ַ Y ��ֵ����������ֵ��
 */
void LCD_ST7735S_FillSetArea( u16 xBeg, u16 yBeg, u16 xEnd, u16 yEnd )
{
    // ������ʾ
    if( LCD_ST7735S_SCREEN == LCD_ST7735S_SCREEN_SHU ){
        // �е�ַ����
        LCD_ST7735S_SendCmd(0x2A); 
        LCD_ST7735S_SendData16(xBeg+LCD_ST7735S_SCREEN_SHU_XOFFSET);
        LCD_ST7735S_SendData16(xEnd+LCD_ST7735S_SCREEN_SHU_XOFFSET);
        
        // �е�ַ����
        LCD_ST7735S_SendCmd(0x2B); 
        LCD_ST7735S_SendData16(yBeg+LCD_ST7735S_SCREEN_SHU_YOFFSET);
        LCD_ST7735S_SendData16(yEnd+LCD_ST7735S_SCREEN_SHU_YOFFSET);
    }
    
    // ������ʾ
    if( LCD_ST7735S_SCREEN == LCD_ST7735S_SCREEN_HEN ){
        // �е�ַ����
        LCD_ST7735S_SendCmd(0x2A); 
        LCD_ST7735S_SendData16(xBeg+LCD_ST7735S_SCREEN_HEN_XOFFSET);
        LCD_ST7735S_SendData16(xEnd+LCD_ST7735S_SCREEN_HEN_XOFFSET);
        
        // �е�ַ����
        LCD_ST7735S_SendCmd(0x2B); 
        LCD_ST7735S_SendData16(yBeg+LCD_ST7735S_SCREEN_HEN_YOFFSET);
        LCD_ST7735S_SendData16(yEnd+LCD_ST7735S_SCREEN_HEN_YOFFSET);
    }
    
    // ������д
    LCD_ST7735S_SendCmd(0x2C); 
}


/**
 * ����
 * X/Y ����һ����������꣺���������������ߺ�����X ��ָ�������꣬Y��ָ��������
 *
 * @param u16 x      ��ʼ��ַ X ��ֵ����������ֵ��
 * @param u16 y      ��ʼ��ַ Y ��ֵ����������ֵ��
 * @param u16 color  ��ɫ
 */
void LCD_ST7735S_DrawPoint( u16 x, u16 y, u16 color )
{
	LCD_ST7735S_FillSetArea(x, y, x, y);
	LCD_ST7735S_SendData16(color);
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
void LCD_ST7735S_DrawLine( u16 x1, u16 y1, u16 x2, u16 y2, u16 color )
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
		LCD_ST7735S_DrawPoint(x, y, color);
        
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
    LCD_ST7735S_DrawPoint(x2, y2, color);
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
void LCD_ST7735S_DrawCircle( u16 x, u16 y, u8 r, u16 color, bool fill, u8 border )
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
            LCD_ST7735S_DrawPoint(x-ty, y+tx, color);  // ��1�����У������ң�
            LCD_ST7735S_DrawPoint(x-ty, y-tx, color);
            LCD_ST7735S_DrawPoint(x-tx, y+ty, color);  // ��2�����У������ң�
            LCD_ST7735S_DrawPoint(x-tx, y-ty, color);
            LCD_ST7735S_DrawPoint(x+tx, y+ty, color);  // ��3�����У������ң�
            LCD_ST7735S_DrawPoint(x+tx, y-ty, color);
            LCD_ST7735S_DrawPoint(x+ty, y+tx, color);  // ��4�����У������ң�
            LCD_ST7735S_DrawPoint(x+ty, y-tx, color);
            
            // ���õݹ� LCD_ST7735S_DrawCircle �ķ�ʽ�� Border
            // ÿ�εݹ� ���������+1���뾶-1�� ������Բ���������� Bresenham �㷨��ԭ�򣬲��� border ֱ�ӿ��ܻ���һ����Ŀ�϶��������в���
            if( border > 1 ){
                LCD_ST7735S_DrawPoint(x-ty+1, y+tx, color);  // ��1�����У������ң�
                LCD_ST7735S_DrawPoint(x-ty+1, y-tx, color);
                LCD_ST7735S_DrawPoint(x-tx+1, y+ty, color);  // ��2�����У������ң�
                LCD_ST7735S_DrawPoint(x-tx+1, y-ty, color);
                LCD_ST7735S_DrawPoint(x+tx-1, y+ty, color);  // ��3�����У������ң�
                LCD_ST7735S_DrawPoint(x+tx-1, y-ty, color);
                LCD_ST7735S_DrawPoint(x+ty-1, y+tx, color);  // ��4�����У������ң�
                LCD_ST7735S_DrawPoint(x+ty-1, y-tx, color);
            }
        }else{
            LCD_ST7735S_DrawLine(x-ty, y-tx, x-ty, y+tx, color);  // ��1�����У������ң�
            LCD_ST7735S_DrawLine(x-tx, y-ty, x-tx, y+ty, color);  // ��2�����У������ң�
            LCD_ST7735S_DrawLine(x+tx, y-ty, x+tx, y+ty, color);  // ��3�����У������ң�
            LCD_ST7735S_DrawLine(x+ty, y-tx, x+ty, y+tx, color);  // ��4�����У������ң�
        }

        // ȡ����ĵ�
		if( d < 0 ) d += 4*tx + 6;
        
        // ȡ����ĵ�
		else d += 4*(tx-ty)+10, ty--;
 
		tx++;
    }
    
    // �ݹ黭 Border
    if( fill == false && border > 1 ) LCD_ST7735S_DrawCircle(x+1-r, y+1-r, r-1, color, fill, border-1);
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
void LCD_ST7735S_DrawRectangle( u16 x, u16 y, u16 xlen, u16 ylen, u16 color, bool fill, u8 border )
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
        for( i = 0; i <= xlen; i++ ) LCD_ST7735S_DrawLine(i+x, y, i+x, y+ylen, color);
    }

    // �����ľ���
    else{
        // ������
        LCD_ST7735S_DrawLine(x, y, x+xlen, y, color);
        LCD_ST7735S_DrawLine(x, y, x, y+ylen, color);
        LCD_ST7735S_DrawLine(x, y+ylen, x+xlen, y+ylen, color);
        LCD_ST7735S_DrawLine(x+xlen, y, x+xlen, y+ylen, color);
        
        // �ݹ黭 Border
        if( border > 1 ) LCD_ST7735S_DrawRectangle(x+1, y+1, xlen-1, ylen-1, color, fill, border-1);
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
void LCD_ST7735S_ShowChar( u16 x, u16 y, u8 ascii, u8 size, u16 color, u16 background, u8 backfill )
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
    LCD_ST7735S_FillSetArea(x, y, x+sizex-1, y+size-1);  
    
    // ѭ���ַ��� LCD �ֽڼ�
    // ���磺ѭ��һ�� 24*12 ���ص��ַ������� GV_LcdAscii1224 �е���ģ������Ҫ��48���ֽ�
	for( i = 0; i < lcdCharBytes; i++ ){
        // ѭ����ȡ�ַ���LCD�ֽ�
		if( size == 12 ) lcdCharByte = GV_LcdAscii0612[ascii][i];  // ���� 6x12  ����
        if( size == 14 ) lcdCharByte = GV_LcdAscii0714[ascii][i];  // ���� 6x12  ����
		//if( size == 16 ) lcdCharByte = GV_LcdAscii0816[ascii][i];  // ���� 8x16  ����
		//if( size == 24 ) lcdCharByte = GV_LcdAscii1224[ascii][i];  // ���� 12x24 ����
		//if( size == 32 ) lcdCharByte = GV_LcdAscii1632[ascii][i];  // ���� 16x32 ����

        // ѭ������ÿ�� LCD �ֽ�λ
        // lcdCharByte&(0x01<<bit) ��ʾȡ��һ���ֽ���� bit λ��ֵ
		for( bit = 0; bit < 8; bit++ ){
            // �������
			if( backfill ){
                LCD_ST7735S_SendData16(lcdCharByte&(0x01<<bit)?color:background);
				if( ++m%sizex == 0 ){ m = 0; break; }
			}
            
            // ��������
			else{
                // ����
				if( lcdCharByte&(0x01<<bit) ) LCD_ST7735S_DrawPoint(x, y, color);
                
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
void LCD_ST7735S_ShowIcon( u16 x, u16 y, u8 index, u8 size, u16 color, u16 background, u8 backfill )
{
    // ��Ч����
    if( !(size==16||size==24||size==32) ) return;
    
    // ��ʼ��
	u8 i, bit, m = 0;
    u16 lcdIconByte, xorigin = x;

    // ���ü�����������
    LCD_ST7735S_FillSetArea(x, y, x+size-1, y+size-1);  

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
                LCD_ST7735S_SendData16(lcdIconByte&(0x01<<bit)?color:background);
				if( ++m%size == 0 ){ m = 0; break; }
			}
            
            // ��������
			else{
                // ����
				if( lcdIconByte&(0x01<<bit) ) LCD_ST7735S_DrawPoint(x, y, color);
                
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
void LCD_ST7735S_ShowString( u16 x, u16 y, u8 *fonts, u8 size, u16 color, u16 background, u8 backfill )
{      
    // ��Ч����
    if( !(size==12||size==14||size==16||size==24||size==32) ) return;
    
    // ���ַ���
	while( *fonts != '\0' ){       
		LCD_ST7735S_ShowChar(x, y, *fonts, size, color, background, backfill);
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
void LCD_ST7735S_ShowChinese( u16 x, u16 y, u8 *fonts, u8 size, u16 color, u16 background, u8 backfill )
{
    // ��Ч����
    if( !(size==12||size==14||size==16||size==24||size==32) ) return;
    
    // ������
	while( *fonts != 0 ){
        LCD_ST7735S_ShowChineseOne(x, y, fonts, size, color, background, backfill);
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
void LCD_ST7735S_ShowChineseOne( u16 x, u16 y, u8 *font, u8 size, u16 color, u16 background, u8 backfill )
{
    // ��ʼ��
	u16 xorigin = x;
    u8 i, k, bit, find;
    short m = 0, total = 0;

    // ��ʼ���ּ�����
    if( size == 12 ) total = GV_LcdChinese12Total;
    if( size == 14 ) total = GV_LcdChinese14Total;
    if( size == 16 ) total = GV_LcdChinese16Total;
    //if( size == 24 ) total = GV_LcdChinese24Total;
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
        //if( size == 24 && (GV_LcdChinese24[k].Index[0]==*(font)) && (GV_LcdChinese24[k].Index[1]==*(font+1)) ) find = true;
        //if( size == 32 && (GV_LcdChinese32[k].Index[0]==*(font)) && (GV_LcdChinese32[k].Index[1]==*(font+1)) ) find = true;
        if( find == false ) continue;
        
        // ���ü�����������
        LCD_ST7735S_FillSetArea(x, y, x+size-1, y+size-1);

        // ѭ���ַ��� LCD �ֽڼ�
        for( i = 0; i < lcdCharBytes; i++ ){
            // ѭ������ÿ�� LCD �ֽ�λ
            // GV_LcdChineseXX[k].Masks[i]&(0x01<<bit) ��ʾȡ��һ���ֽ���� bit λ��ֵ
            for( bit=0; bit<8; bit++ ){	
                // �������
                if( backfill ){
                    if( size == 12 ) LCD_ST7735S_SendData16(GV_LcdChinese12[k].Masks[i]&(0x01<<bit)?color:background);
                    if( size == 14 ) LCD_ST7735S_SendData16(GV_LcdChinese14[k].Masks[i]&(0x01<<bit)?color:background);
                    if( size == 16 ) LCD_ST7735S_SendData16(GV_LcdChinese16[k].Masks[i]&(0x01<<bit)?color:background);
                    //if( size == 24 ) LCD_ST7735S_SendData16(GV_LcdChinese24[k].Masks[i]&(0x01<<bit)?color:background);
                    //if( size == 32 ) LCD_ST7735S_SendData16(GV_LcdChinese32[k].Masks[i]&(0x01<<bit)?color:background);
                    if( ++m%size == 0 ){ m = 0; break; }
                }

                // ��������
                else{
                    // ����
                    if( size == 12 && GV_LcdChinese12[k].Masks[i]&(0x01<<bit) ) LCD_ST7735S_DrawPoint(x, y, color);
                    if( size == 14 && GV_LcdChinese14[k].Masks[i]&(0x01<<bit) ) LCD_ST7735S_DrawPoint(x, y, color);
                    if( size == 16 && GV_LcdChinese16[k].Masks[i]&(0x01<<bit) ) LCD_ST7735S_DrawPoint(x, y, color);
                    //if( size == 24 && GV_LcdChinese24[k].Masks[i]&(0x01<<bit) ) LCD_ST7735S_DrawPoint(x, y, color);
                    //if( size == 32 && GV_LcdChinese32[k].Masks[i]&(0x01<<bit) ) LCD_ST7735S_DrawPoint(x, y, color);

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
void LCD_ST7735S_ShowImage( u16 x, u16 y, u16 xlen, u16 ylen, const u8 image[], bool reverse )
{
    // ��ʼ��
	u16 i, j;
	u32 datai = 0;
    
    // ���ü�����������
	LCD_ST7735S_FillSetArea(x, y, x+xlen-1, y+ylen-1);
    
    // ��˳������ɫ����
	for( i = 0; i < xlen; i++ ){
		for( j = 0; j < ylen; j++ ){
			LCD_ST7735S_SendData8(reverse?~image[datai*2]:image[datai*2]);
			LCD_ST7735S_SendData8(reverse?~image[datai*2+1]:image[datai*2]);
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
 * @param bool  backfill    ��������
 * @param short showlen     Ҫ��ʾ�ĳ��ȣ��������ź�С���㣩�����ֳ��Ȳ���ʱ�ո�ȫ��0 ��ʾ�Զ�ʶ�𳤶ȣ�������ʾ����룬������ʾ�Ҷ���
 *
 * @param short ����ʵ����ʾ�����ָ���������С����͸��ţ�
 */
short LCD_ST7735S_ShowNumber( u16 x, u16 y, double number, u8 size, u16 color, u16 background, u8 backfill, short showlen )
{
    long intnum = ((long)(number*100));
    if( intnum%100 == 0 ) return LCD_ST7735S_ShowInteger(x, y, (long)number, size, color, background, backfill, showlen);
    else return LCD_ST7735S_ShowFloat(x, y, number, size, color, background, backfill, showlen );
}
void LCD_ST7735S_ShowSplitNumber( u16 x, u16 y, short *splitnums, short splitindex, u8 size, u16 color, u16 background, u8 backfill, short showlen )
{
    // ��ʼ��
	short i, l;
    short sizex = size/2;
    
    // ȫ����ʾ
    if( showlen == 0 ){
        for( i = 0; i < splitindex; i++ ){
            if( splitnums[i] >= 0  ) LCD_ST7735S_ShowChar(x+i*sizex, y, splitnums[i]+48, size, color, background, backfill);
            if( splitnums[i] == -1 ) LCD_ST7735S_ShowChar(x+i*sizex, y, '.', size, color, background, backfill);
            if( splitnums[i] == -2 ) LCD_ST7735S_ShowChar(x+i*sizex, y, '-', size, color, background, backfill);
        }
    }
    
    // ��ʾ�������ƣ�����룩
    if( showlen > 0 ){
        for( l = 0; l < showlen; l++ ){
            i = l;
            if( i < splitindex ){
                if( splitnums[i] >= 0  ) LCD_ST7735S_ShowChar(x+l*sizex, y, splitnums[i]+48, size, color, background, backfill);
                if( splitnums[i] == -1 ) LCD_ST7735S_ShowChar(x+l*sizex, y, '.', size, color, background, backfill);
                if( splitnums[i] == -2 ) LCD_ST7735S_ShowChar(x+l*sizex, y, '-', size, color, background, backfill);
            }else{
                LCD_ST7735S_ShowChar(x+l*sizex, y, ' ', size, color, background, backfill);
            }
        }
    }
    
    // ��ʾ�������ƣ��Ҷ��룩
    if( showlen < 0 ){
        showlen = -showlen;
        for( l = showlen; l > 0; l-- ){
            i = splitindex - (showlen-l) - 1;
            if( i >= 0 && i < splitindex ){
                if( splitnums[i] >= 0  ) LCD_ST7735S_ShowChar(x+(l-1)*sizex, y, splitnums[i]+48, size, color, background, backfill);
                if( splitnums[i] == -1 ) LCD_ST7735S_ShowChar(x+(l-1)*sizex, y, '.', size, color, background, backfill);
                if( splitnums[i] == -2 ) LCD_ST7735S_ShowChar(x+(l-1)*sizex, y, '-', size, color, background, backfill);
            }else{
                LCD_ST7735S_ShowChar(x+(l-1)*sizex, y, ' ', size, color, background, backfill);
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
short LCD_ST7735S_ShowFloat( u16 x, u16 y, double number, u8 size, u16 color, u16 background, u8 backfill, short showlen )
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
    LCD_ST7735S_ShowSplitNumber(x, y, splitnums, splitindex, size, color, background, backfill, showlen);
        
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
short LCD_ST7735S_ShowInteger( u16 x, u16 y, long number, u8 size, u16 color, u16 background, u8 backfill, short showlen )
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
    LCD_ST7735S_ShowSplitNumber(x, y, splitnums, splitindex, size, color, background, backfill, showlen);
    
    // ����ʵ����ʾ�����ָ���������С����͸��ţ�
    showlen = showlen > 0 ? showlen : -showlen;
    return showlen == 0 ? splitindex : (showlen>splitindex?splitindex:showlen);
} 