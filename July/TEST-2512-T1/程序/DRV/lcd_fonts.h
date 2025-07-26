#ifndef __LCD_FONTS_H_
#define __LCD_FONTS_H_


    /* -------------------------------------------------- */
    // ● 结构定义
    /* -------------------------------------------------- */

    /**
     * LCD 汉字字模结构体
     */
    struct STRUCT_LCD_CHINESE12{ unsigned char Index[2]; unsigned char Masks[24];  }; 
    struct STRUCT_LCD_CHINESE14{ unsigned char Index[2]; unsigned char Masks[28];  }; 
    struct STRUCT_LCD_CHINESE16{ unsigned char Index[2]; unsigned char Masks[32];  }; 
    struct STRUCT_LCD_CHINESE24{ unsigned char Index[2]; unsigned char Masks[72];  }; 
    struct STRUCT_LCD_CHINESE32{ unsigned char Index[2]; unsigned char Masks[128]; }; 

    
#endif