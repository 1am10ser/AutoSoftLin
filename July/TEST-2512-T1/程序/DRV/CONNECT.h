#ifndef __CONNECT_H
#define __CONNECT_H
#include "stm8s.h"

/* -------------------------------------------------- */
// ¡ñ º¯ÊýÉùÃ÷
/* -------------------------------------------------- */
extern unsigned short CONNECT_LEDshuzu[42];

extern unsigned char CONNECT_LEDQuantity;

void CONNECT_Init(void);

void CONNECT_CtlBasics(void);
void CONNECT_DatBasics(unsigned char X);

void CONNECT_CtlOpen(short number,short render);
void CONNECT_CtlOpenAll(short render);

void CONNECT_CtlClose(short number,short render);
void CONNECT_CtlCloseAll(short render);

void CONNECT_DatIsConnectAny(short render);   
void CONNECT_DatIsConnectOnly(short number,short render);  
void CONNECT_DatIsConnect(short render);  

#endif

