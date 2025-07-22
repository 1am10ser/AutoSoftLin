#include "common.h"



/* -------------------------------------------------- */
// �� ���߼�����
/* -------------------------------------------------- */

/**
 * ���߼�����ʼ��
 */
void LCD_MainInit()
{
    LCD_MSP3520_Init(LCD_COLOR_WHITE);
}


/**
 * ���߼�����ʾ LOGO
 */
void LCD_MainLogo()
{
    //static int i = 0; 
    //LCD_MSP3520_ShowImage(0, 0, 160, 80, GV_LcdPicLogoKede, i?false:true);  // ��Լ��Ƭ�� ROM �ռ䣬Ĭ��ע�͵�
    //delay_ms(1000);
}


/**
 * ���߼���������м���
 */
void LCD_MainMotorMonitor()
{
    // ��ʼ������
    short i, x, y, len;
    short strSpace  = 5;
    short lineSpace = 5;
    short fontSize  = 14;
        
    // ��ʼ����ɫ
    u16 color = LCD_COLOR_BLACK;
    u16 background = LCD_COLOR_WHITE;
    
    // ��ȡȦ���ͽǶ�
    long   circle = MOTOR_GetCountCircle();
    double degree = MOTOR_GetCountDegree();
    double length = MOTOR_CIRCLE_LEN*circle+MOTOR_CIRCLE_LEN/360.0*degree;

    // ȡ���Ƕ�ֵ
    degree = degree >= 0 ? degree : (360+degree);

    // ���� SPI
    LCD_MSP3520_SpiOpenScreen();
    
    // ��ʾȦ��
    x = strSpace; y = lineSpace;
    LCD_MSP3520_ShowChinese(x, y, "Ȧ����", fontSize, color, background, false);
    
    x = strSpace+3*fontSize;
    len = LCD_MSP3520_ShowInteger(x, y, circle, fontSize, color, background, true, 0);
    
    x = strSpace + 3*fontSize + len*(fontSize/2);
    for( i = 0; i < 6-len; i++ ) LCD_MSP3520_ShowChar(x+i*(fontSize/2), y, ' ', fontSize, color, background, true);  // Ĩȥ���������Ĳ�Ӱ

    
    // ��ʾ�Ƕ�
    x = strSpace; y = lineSpace*2+fontSize;
    LCD_MSP3520_ShowChinese(x, y, "�Ƕȣ�", fontSize, color, background, false);
    
    x = strSpace + 3*fontSize;
    len = LCD_MSP3520_ShowFloat(x, y, degree, fontSize, color, background, true, 0);
    
    x = strSpace + 3*fontSize + len*(fontSize/2);
    LCD_MSP3520_ShowChinese(x, y, "��", fontSize, color, background, true);  // ��ʾ��λ
    
    x = strSpace + 3*fontSize + (len+1)*(fontSize/2);
    for( i = 0; i < 6-len; i++ ) LCD_MSP3520_ShowChar(x+i*(fontSize/2), y, ' ', fontSize, color, background, true);  // Ĩȥ���������Ĳ�Ӱ
    

    // ��ʾ�н�
    x = strSpace; y = lineSpace*3+fontSize*2;
    LCD_MSP3520_ShowChinese(x, y, "�н���", fontSize, color, background, false);
    
    x = strSpace + 3*fontSize;
    len = LCD_MSP3520_ShowFloat(x, y, length, fontSize, color, background, true, 0);
    
    x = strSpace + 3*fontSize + len*(fontSize/2) + 1;
    LCD_MSP3520_ShowString(x, y, "mm", fontSize, color, background, true);  // ��ʾ��λ
    
    x = strSpace + 3*fontSize + (len+2)*(fontSize/2) + 1;
    LCD_MSP3520_DrawLine(x+1, y, x+1, y+fontSize, background);
    for( i = 0; i < 6-len; i++ ) LCD_MSP3520_ShowChar(x+i*(fontSize/2), y, ' ', fontSize, color, background, true);  // Ĩȥ���������Ĳ�Ӱ
            
    // �ر� SPI
    LCD_MSP3520_SpiClose();
}


/**
 * ���߼�����ʾ��̬ͼ��
 */
void LCD_MainAnimateIcons()
{
    // ��ʼ��
    static u8 i = 0;

    // ���� SPI
    LCD_MSP3520_SpiOpenScreen();
    
    // ����ͼ��
    if( i ){ 
        LCD_MSP3520_ShowIcon(0,   0, LCD_ICON_SUN_ON, 16, LCD_COLOR_RED    , LCD_COLOR_WHITE, false); 
        LCD_MSP3520_ShowIcon(0,  17, LCD_ICON_SUN_ON, 16, LCD_COLOR_RED    , LCD_COLOR_WHITE, false); 
        LCD_MSP3520_ShowIcon(0,  34, LCD_ICON_SUN_ON, 16, LCD_COLOR_RED    , LCD_COLOR_WHITE, false); 
        LCD_MSP3520_ShowIcon(0,  51, LCD_ICON_SUN_ON, 16, LCD_COLOR_RED    , LCD_COLOR_WHITE, false); 
        LCD_MSP3520_ShowIcon(0,  68, LCD_ICON_SUN_ON, 16, LCD_COLOR_RED    , LCD_COLOR_WHITE, false); 
        LCD_MSP3520_ShowIcon(0,  85, LCD_ICON_SUN_ON, 16, LCD_COLOR_RED    , LCD_COLOR_WHITE, false);
        
        LCD_MSP3520_ShowIcon(20,  0, LCD_ICON_SUN_ON, 24, LCD_COLOR_DBLUE  , LCD_COLOR_WHITE, false); 
        LCD_MSP3520_ShowIcon(20, 25, LCD_ICON_SUN_ON, 24, LCD_COLOR_DBLUE  , LCD_COLOR_WHITE, false); 
        LCD_MSP3520_ShowIcon(20, 50, LCD_ICON_SUN_ON, 24, LCD_COLOR_DBLUE  , LCD_COLOR_WHITE, false); 
        LCD_MSP3520_ShowIcon(20, 75, LCD_ICON_SUN_ON, 24, LCD_COLOR_DBLUE  , LCD_COLOR_WHITE, false); 
        
        LCD_MSP3520_ShowIcon(45,  0, LCD_ICON_SUN_ON, 32, LCD_COLOR_DYELLOW, LCD_COLOR_WHITE, false); 
        LCD_MSP3520_ShowIcon(45, 33, LCD_ICON_SUN_ON, 32, LCD_COLOR_DYELLOW, LCD_COLOR_WHITE, false); 
        LCD_MSP3520_ShowIcon(45, 66, LCD_ICON_SUN_ON, 32, LCD_COLOR_DYELLOW, LCD_COLOR_WHITE, false); 
        
        i = 0;
    }
    else{ 
        LCD_MSP3520_ShowIcon(0,   0, LCD_ICON_SUN_OFF, 16, LCD_COLOR_WHITE, LCD_COLOR_WHITE, true); 
        LCD_MSP3520_ShowIcon(0,  17, LCD_ICON_SUN_OFF, 16, LCD_COLOR_WHITE, LCD_COLOR_WHITE, true); 
        LCD_MSP3520_ShowIcon(0,  34, LCD_ICON_SUN_OFF, 16, LCD_COLOR_WHITE, LCD_COLOR_WHITE, true); 
        LCD_MSP3520_ShowIcon(0,  51, LCD_ICON_SUN_OFF, 16, LCD_COLOR_WHITE, LCD_COLOR_WHITE, true); 
        LCD_MSP3520_ShowIcon(0,  68, LCD_ICON_SUN_OFF, 16, LCD_COLOR_WHITE, LCD_COLOR_WHITE, true); 
        LCD_MSP3520_ShowIcon(0,  85, LCD_ICON_SUN_OFF, 16, LCD_COLOR_WHITE, LCD_COLOR_WHITE, true);
        
        LCD_MSP3520_ShowIcon(20,  0, LCD_ICON_SUN_OFF, 24, LCD_COLOR_WHITE, LCD_COLOR_WHITE, true); 
        LCD_MSP3520_ShowIcon(20, 25, LCD_ICON_SUN_OFF, 24, LCD_COLOR_WHITE, LCD_COLOR_WHITE, true); 
        LCD_MSP3520_ShowIcon(20, 50, LCD_ICON_SUN_OFF, 24, LCD_COLOR_WHITE, LCD_COLOR_WHITE, true); 
        LCD_MSP3520_ShowIcon(20, 75, LCD_ICON_SUN_OFF, 24, LCD_COLOR_WHITE, LCD_COLOR_WHITE, true); 
        
        LCD_MSP3520_ShowIcon(45,  0, LCD_ICON_SUN_OFF, 32, LCD_COLOR_WHITE, LCD_COLOR_WHITE, true); 
        LCD_MSP3520_ShowIcon(45, 33, LCD_ICON_SUN_OFF, 32, LCD_COLOR_WHITE, LCD_COLOR_WHITE, true); 
        LCD_MSP3520_ShowIcon(45, 66, LCD_ICON_SUN_OFF, 32, LCD_COLOR_WHITE, LCD_COLOR_WHITE, true); 
        
        i = 1;
    }
        
    // �ر� SPI
    LCD_MSP3520_SpiClose();
}


/**
 * ���߼�����ʾ������ɫ��Բ 
 *
 * @param bool fill �Ƿ����
 */
void LCD_MainColorCircles( bool fill )
{
    // ���� SPI
    LCD_MSP3520_SpiOpenScreen();
    
    // ����
    LCD_MSP3520_Fill(0, 0, 200, 200, LCD_COLOR_WHITE);
    
    // ����ԲȦ
    LCD_MSP3520_DrawCircle( 0 , 0, 8, LCD_COLOR_DRED     , fill, 1);
    LCD_MSP3520_DrawCircle( 20, 0, 8, LCD_COLOR_DBLUE    , fill, 2);
    LCD_MSP3520_DrawCircle( 40, 0, 8, LCD_COLOR_DGREEN   , fill, 3);
    LCD_MSP3520_DrawCircle( 60, 0, 8, LCD_COLOR_DMAGENTA , fill, 4);
    LCD_MSP3520_DrawCircle( 80, 0, 8, LCD_COLOR_DYELLOW  , fill, 5);
    LCD_MSP3520_DrawCircle(100, 0, 8, LCD_COLOR_DCYAN    , fill, 6);
    LCD_MSP3520_DrawCircle(120, 0, 8, LCD_COLOR_DORANGE  , fill, 7);
    LCD_MSP3520_DrawCircle(140, 0, 8, LCD_COLOR_DGRAY    , fill, 8);
    
    LCD_MSP3520_DrawCircle(  0, 20, 8, LCD_COLOR_RED     , fill, 8);
    LCD_MSP3520_DrawCircle( 20, 20, 8, LCD_COLOR_BLUE    , fill, 7);
    LCD_MSP3520_DrawCircle( 40, 20, 8, LCD_COLOR_GREEN   , fill, 6);
    LCD_MSP3520_DrawCircle( 60, 20, 8, LCD_COLOR_MAGENTA , fill, 5);
    LCD_MSP3520_DrawCircle( 80, 20, 8, LCD_COLOR_YELLOW  , fill, 4);
    LCD_MSP3520_DrawCircle(100, 20, 8, LCD_COLOR_CYAN    , fill, 3);
    LCD_MSP3520_DrawCircle(120, 20, 8, LCD_COLOR_ORANGE  , fill, 2);
    LCD_MSP3520_DrawCircle(140, 20, 8, LCD_COLOR_GRAY    , fill, 1);
    
    LCD_MSP3520_DrawCircle(  0, 40, 8, LCD_COLOR_LRED    , fill, 1);
    LCD_MSP3520_DrawCircle( 20, 40, 8, LCD_COLOR_LBLUE   , fill, 2);
    LCD_MSP3520_DrawCircle( 40, 40, 8, LCD_COLOR_LGREEN  , fill, 3);
    LCD_MSP3520_DrawCircle( 60, 40, 8, LCD_COLOR_LMAGENTA, fill, 4);
    LCD_MSP3520_DrawCircle( 80, 40, 8, LCD_COLOR_LYELLOW , fill, 5);
    LCD_MSP3520_DrawCircle(100, 40, 8, LCD_COLOR_LCYAN   , fill, 6);
    LCD_MSP3520_DrawCircle(120, 40, 8, LCD_COLOR_LORANGE , fill, 7);
    LCD_MSP3520_DrawCircle(140, 40, 8, LCD_COLOR_LGRAY   , fill, 8);
    
    // �ر� SPI
    LCD_MSP3520_SpiClose();
}



/* -------------------------------------------------- */
// �� ���߼����룺LED ����
// �� ���� MSP3520(480*320) ����ģʽ�µĲ���
/* -------------------------------------------------- */

/**
 * ���߼���LED ������ȫ�ֱ�����
 */
short GV_MainLedSceneLedTotal         = 32;  //���������á�LED������������Ҫ�������ã�������ܳ��� 40 ����
short GV_MainLedSceneTitleHeight      = 80;  //���Ƽ�Ĭ�ϡ�����߶�
short GV_MainLedSceneRowTotal         = 4;   //���Ƽ�Ĭ�ϡ�LED ������       ��32��LED�����Ƽ�ֵ��4 ��32��LED�����Ƽ�ֵ��5 ��
short GV_MainLedSceneLedWidth         = 50;  //���Ƽ�Ĭ�ϡ�LED ���         ��32��LED�����Ƽ�ֵ��50��32��LED�����Ƽ�ֵ��58��
short GV_MainLedSceneLedHeight        = 50;  //���Ƽ�Ĭ�ϡ�LED �߶�         ��32��LED�����Ƽ�ֵ��50��32��LED�����Ƽ�ֵ��46��
short GV_MainLedSceneNumberSize       = 24;  //���Ƽ�Ĭ�ϡ�LED ��������С
short GV_MainLedSceneLedMarginTop     = 10;   //���Ƽ�Ĭ�ϡ�LED Margin Top   ��32��LED�����Ƽ�ֵ��10��32��LED�����Ƽ�ֵ��2��
short GV_MainLedSceneLedMarginLeft    = 9;   //���Ƽ�Ĭ�ϡ�LED Margin Left  ��32��LED�����Ƽ�ֵ��9 ��32��LED�����Ƽ�ֵ��2��

short GV_MainLedSceneLedStates[40]    = {0};  //��ϵͳʹ�á�LED �Ƽ���״̬
short GV_MainLedSceneColumnTotal      = 0;    //���Ƽ�Ĭ�ϡ�LED ��������Ϊ 0 ʱ��ֵ�� LCD_MainLedScene() ���ж�̬���ã��������Զ���Ĺ̶�ֵ
short GV_MainLedSceneLedSetMarginLeft = 0;    //���Ƽ�Ĭ�ϡ�LED ���� Margin Left����Ҫ���� LED ���Ͼ�����ʾ����Ϊ 0 ʱ��ֵ�� LCD_MainLedScene() ���ж�̬���ã��������Զ���Ĺ̶�ֵ


/**
 * ���߼���LED ��������ʼ����  
 */
void LCD_MainLedSceneInit()
{
    // ��ʼ��
    u8 i, j, num;
    
    // LCD ��ʼ��
    LCD_MSP3520_Init(LCD_COLOR_WHITE);
    
    // LED ������
    // ��̬��ʼ����Ĭ�ϲ�������ʾ 4 �� LED ʱ����������
    if( GV_MainLedSceneColumnTotal == 0 ) GV_MainLedSceneColumnTotal = GV_MainLedSceneLedTotal/GV_MainLedSceneRowTotal+(GV_MainLedSceneLedTotal%GV_MainLedSceneRowTotal>=1?1:0);

    // LED ������ʾʱ LED ���� Margin Left ��ֵ
    if( GV_MainLedSceneLedSetMarginLeft == 0 ) GV_MainLedSceneLedSetMarginLeft = (LCD_MSP3520_SCREEN_LONG-(GV_MainLedSceneColumnTotal*(GV_MainLedSceneLedMarginLeft+GV_MainLedSceneLedWidth)-GV_MainLedSceneLedMarginLeft))/2;
    if( GV_MainLedSceneLedSetMarginLeft <= 0 ) GV_MainLedSceneLedSetMarginLeft = 0;
    
    // ���� SPI
    LCD_MSP3520_SpiOpenScreen();

    // ���� ����
    LCD_MainLedSceneDrawTitle("KD-281", "ͨ��·����̨", "KD-281-1/2/3/4/5, KD-281-50/60", "TEST-281-T1");
    
    // ���� LED ����
    for( i = 0; i < GV_MainLedSceneRowTotal; i++ ){
        for( j = 0; j < GV_MainLedSceneColumnTotal; j++ ){
            // �������
            num = i*GV_MainLedSceneColumnTotal+j+1;
            if( num > GV_MainLedSceneLedTotal ) break;

            // ���� LED ��
            LCD_MainLedSceneDrawItem(i, j, num, false);
        }
    }

    // �ر� SPI
    LCD_MSP3520_SpiClose();
}


/**
 * ���߼���LED ������LED ������
 *
 * @param u8 number LED�Ʊ�ţ���Ŵ�1��ʼ
 */
void LCD_MainLedSceneOpen( u8 number )
{
    if( LCD_MainLedSceneState(number) == false ) LCD_MainLedSceneAction(number, 1);
}


/**
 * ���߼���LED ������LED �رգ�
 *
 * @param u8 number LED�Ʊ�ţ���Ŵ�1��ʼ
 */
void LCD_MainLedSceneClose( u8 number )
{
    if( LCD_MainLedSceneState(number) == true ) LCD_MainLedSceneAction(number, 0);
}


/**
 * ���߼���LED ��������ȡָ�� LED ��״̬��
 *
 * @param u8 number LED�Ʊ�ţ���Ŵ�1��ʼ
 */
bool LCD_MainLedSceneState( u8 number )
{
    short state = number ? GV_MainLedSceneLedStates[number-1] : 0;
    return state ? true : false;
}


/**
 * ���߼���LED ������LED ������
 *
 * @param u8   number LED�Ʊ�ţ���Ŵ�1��ʼ
 * @param bool state  LED��״̬��true ��ʾ����false ��ʾ��
 */
void LCD_MainLedSceneAction( u8 number, bool state )
{
    // ��ʼ��
    u8 i, j, num;

    // ���� SPI
    LCD_MSP3520_SpiOpenScreen();

    // ���� LED
    for( i = 0; i < GV_MainLedSceneRowTotal; i++ ){
        for( j = 0; j < GV_MainLedSceneColumnTotal; j++ ){
            // �������
            num = i*GV_MainLedSceneColumnTotal+j+1;
            if( num > GV_MainLedSceneLedTotal ) break;
            if( num != number ) continue;

            // ���� LED ��
            LCD_MainLedSceneDrawItem(i, j, num, state);

            // ��¼ LED ��״̬
            GV_MainLedSceneLedStates[number-1] = state; 
            
            // ����
            break;
        }
    }
    
    // �ر� SPI
    LCD_MSP3520_SpiClose();
}


/**
 * ���߼���LED ���������� LED �
 */
void LCD_MainLedSceneDrawItem( u8 row, u8 column, u8 number, bool state )
{
    // ���� LED
    u16 x = (GV_MainLedSceneLedWidth+GV_MainLedSceneLedMarginLeft)*column + GV_MainLedSceneLedSetMarginLeft;
    u16 y = (GV_MainLedSceneLedHeight+GV_MainLedSceneLedMarginTop)*row + GV_MainLedSceneTitleHeight;
    LCD_MSP3520_Fill(x, y, x+GV_MainLedSceneLedWidth, y+GV_MainLedSceneLedHeight, state?LCD_COLOR_ORANGE:LCD_COLOR_GRAY);  // ���þ��Σ���ʾ�ٶȿ죩

    // ���� LED ���
    u16 numx = x+(GV_MainLedSceneLedWidth/2-GV_MainLedSceneNumberSize*(number>=10?2:1)/4);
    u16 numy = y+(GV_MainLedSceneLedHeight/2-GV_MainLedSceneNumberSize/2);
    LCD_MSP3520_ShowNumber(numx, numy, number, GV_MainLedSceneNumberSize, LCD_COLOR_WHITE, LCD_COLOR_WHITE, false, 0);  // ��ɫ����
}


/**
 * ���߼���LED ���������� LED ���⣩
 *
 * @param u8* titleEnglish  �����⣺Ӣ�Ĳ���
 * @param u8* titleChinese  �����⣺���Ĳ���
 * @param u8* testProduct   ����̨���ò�Ʒ
 * @param u8* testSn        ����̨�ͺ�
 */
void LCD_MainLedSceneDrawTitle( u8* titleEnglish, u8* titleChinese, u8* testProduct, u8* testSn )
{
    // ��ʼ��
    u16 x, y;
    u8 fontSizeTitle = 24, fontSizeProduct = 12, fontSizeSn = 12;

    // ��ȡ���ⳤ��
    u8 titleEnglishLength = 0;
    while( *titleEnglish != 0 ){ titleEnglishLength++; titleEnglish+=1; }
    titleEnglish -= titleEnglishLength;
    
    u8 titleChineseLength = 0;
    while( *titleChinese != 0 ){ titleChineseLength++; titleChinese+=2; }
    titleChinese -= titleChineseLength*2;

    u8 testSnLength = 0;
    while( *testSn != 0 ){ testSnLength++; testSn+=1; }
    testSn -= testSnLength;
    
    // ������������
    x = (LCD_MSP3520_SCREEN_LONG-titleEnglishLength*(fontSizeTitle/2)-titleChineseLength*fontSizeTitle)/2;  // �������
    x = 15; // �������
    y = (GV_MainLedSceneTitleHeight-GV_MainLedSceneLedMarginTop-fontSizeTitle-fontSizeProduct-5)/2;

    // ���Ʊ��⣺����
    LCD_MSP3520_Fill(0, 0, LCD_MSP3520_SCREEN_LONG, GV_MainLedSceneTitleHeight-GV_MainLedSceneLedMarginTop, LCD_COLOR_BLACK);
    
    // ���Ʊ��⣺����
    LCD_MSP3520_ShowString(x, y, titleEnglish, fontSizeTitle, LCD_COLOR_WHITE, LCD_COLOR_WHITE, false);
    LCD_MSP3520_ShowChinese(x+titleEnglishLength*(fontSizeTitle/2), y, titleChinese, fontSizeTitle, LCD_COLOR_WHITE, LCD_COLOR_WHITE, false);
    
    // ���Ʊ��⣺���ò�Ʒ
    y = y + fontSizeTitle + 5;
    LCD_MSP3520_ShowString(x, y, testProduct, fontSizeProduct, LCD_COLOR_WHITE, LCD_COLOR_WHITE, false);

    // ���Ʊ��⣺����̨�ͺ�
    x = LCD_MSP3520_SCREEN_LONG - GV_MainLedSceneLedMarginLeft - testSnLength*(fontSizeSn/2);
    y = GV_MainLedSceneTitleHeight - fontSizeSn - GV_MainLedSceneLedMarginLeft - 5;
    LCD_MSP3520_ShowString(x, y, testSn, fontSizeSn, LCD_COLOR_DGRAY, LCD_COLOR_DGRAY, false);
}
