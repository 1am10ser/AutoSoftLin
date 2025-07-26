#ifndef __LCD_H_
#define __LCD_H_



    /* -------------------------------------------------- */
    // �� LCD ������ɫ
    // �� 565 ��ʾ��ɫռ��5λ����ɫռ��6λ����ɫռ��5λ
    /* -------------------------------------------------- */

    #define LCD_COLOR565_WHITE       0xFFFF  // ��ɫ
    #define LCD_COLOR565_BLACK       0x0000  // ��ɫ

    #define LCD_COLOR565_DGRAY       0x8430  // ���ɫ
    #define LCD_COLOR565_GRAY        0xAD55  // ��ɫ
    #define LCD_COLOR565_LGRAY       0xC618  // ǳ��ɫ

    #define LCD_COLOR565_DRED        0xB800  // ���ɫ
    #define LCD_COLOR565_RED         0xF800  // ��ɫ
    #define LCD_COLOR565_LRED        0xFC0E  // ����ɫ

    #define LCD_COLOR565_DBLUE       0x0019  // ����ɫ
    #define LCD_COLOR565_BLUE        0x001F  // ��ɫ
    #define LCD_COLOR565_LBLUE       0x03FF	 // ����ɫ

    #define LCD_COLOR565_DGREEN      0x2605  // ����ɫ
    #define LCD_COLOR565_GREEN       0x07E0  // ��ɫ
    #define LCD_COLOR565_LGREEN      0x67EB  // ����ɫ

    #define LCD_COLOR565_DYELLOW     0xE5C0  // ���ɫ
    #define LCD_COLOR565_YELLOW      0xFFE0  // ��ɫ
    #define LCD_COLOR565_LYELLOW     0xFFF3  // ����ɫ

    #define LCD_COLOR565_DMAGENTA    0x9999  // ����ɫ
    #define LCD_COLOR565_MAGENTA     0xDA7F  // ��ɫ
    #define LCD_COLOR565_LMAGENTA    0xE67C  // ����ɫ

    #define LCD_COLOR565_DCYAN       0x0451  // ����ɫ
    #define LCD_COLOR565_CYAN        0x2595  // ��ɫ
    #define LCD_COLOR565_LCYAN       0x4E99  // ����ɫ

    #define LCD_COLOR565_DORANGE     0xD343  // ���ɫ
    #define LCD_COLOR565_ORANGE      0xFCC0  // ��ɫ
    #define LCD_COLOR565_LORANGE     0xFD8C  // ����ɫ



    /* -------------------------------------------------- */
    // �� LCD ����ͼ��
    /* -------------------------------------------------- */
    #define LCD_ICON_SUN_ON       1  // ̫������
    #define LCD_ICON_SUN_OFF      0  // ̫���޹�



    /* -------------------------------------------------- */
    // �� �ṹ����
    /* -------------------------------------------------- */

    /**
     * LCD ������ģ�ṹ��
     */
    struct STRUCT_LCD_CHINESE12{ unsigned char Index[2]; unsigned char Masks[24];  }; 
    struct STRUCT_LCD_CHINESE14{ unsigned char Index[2]; unsigned char Masks[28];  }; 
    struct STRUCT_LCD_CHINESE16{ unsigned char Index[2]; unsigned char Masks[32];  }; 
    struct STRUCT_LCD_CHINESE24{ unsigned char Index[2]; unsigned char Masks[72];  }; 
    struct STRUCT_LCD_CHINESE32{ unsigned char Index[2]; unsigned char Masks[128]; }; 



    /* -------------------------------------------------- */
    // �� ��������
    /* -------------------------------------------------- */
    u16 LCD_Color565( u8 red, u8 green, u8 blue );



    /* -------------------------------------------------- */
    // �� ���߼���������
    /* -------------------------------------------------- */
    void LCD_MainInit();
    void LCD_MainLogo();
    void LCD_MainMotorMonitor();
    void LCD_MainAnimateIcons();
    void LCD_MainColorCircles( bool fill );
    

    
#endif