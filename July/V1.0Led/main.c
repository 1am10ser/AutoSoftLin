#include "common.h"
int main( void )
{
  while(1)
  {
    HC595_SendByte(2,0xFF);
  }
}
