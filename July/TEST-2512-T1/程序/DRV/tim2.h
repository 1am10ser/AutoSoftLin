#ifndef __TIM2_H_
#define __TIM2_H_


    extern unsigned int fe2;
    extern TIM2_Prescaler_TypeDef Prescaler;    

    void TIME2_INTERRUPTHANDLER13(void);
    
    void TIME2_Cmd1cCHANNEL1(unsigned int c);
    void TIME2_Cmd1oCHANNEL1(unsigned short o);
    void TIME2_Cmd1zCHANNEL1(unsigned short z);
    
    void TIME2_Init(void);
    
#endif