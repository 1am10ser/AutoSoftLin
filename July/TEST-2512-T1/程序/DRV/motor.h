#ifndef __MOROR_H
#define __MOROR_H
#include "stm8s.h"

/* -------------------------------------------------- */
// �� ��������
/* -------------------------------------------------- */
void motor_Init(void);
void motor_function(unsigned int f,unsigned short z,unsigned char s,unsigned char x);

#endif
