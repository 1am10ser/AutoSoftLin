#include "common.h"
int main( void )
{
  HC595_Init();
  while(1)
  {
    
    HC595_SendByte(2,0x55);
    HC595_SendOutput(2);
  }
}
