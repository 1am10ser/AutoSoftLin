#ifndef __LIN_SLAVE_H_
#define __LIN_SLAVE_H_

    
    
    /* -------------------------------------------------- */
    // ¡ñ º¯ÊýÉùÃ÷
    /* -------------------------------------------------- */
    void LIN_SLAVE_SetSignalResponse( short dataNum, char dataHighLow, short data8421Bit, short data8421BitValue );    
    void LIN_SLAVE_SendSignalResponse();
    void LIN_SLAVE_InitSignalResponse();
    
    void LIN_SLAVE_ReceiveStart();
    void LIN_SLAVE_ReceiveHandle( u8 dataByte );
    
    
      
#endif