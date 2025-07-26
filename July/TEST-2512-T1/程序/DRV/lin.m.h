#ifndef __LIN_MASTER_H_
#define __LIN_MASTER_H_


    
    /* -------------------------------------------------- */
    // ¡ñ º¯ÊýÉùÃ÷
    /* -------------------------------------------------- */
    void LIN_MASTER_InitDownASignal();
    void LIN_MASTER_SendDownASignal();
    
    void LIN_MASTER_SetDownASignalBytes( u8 *bytes, u8 length );
    void LIN_MASTER_SetDownASignalLLedOn();
    void LIN_MASTER_SetDownASignalRLedOn();

    void LIN_MASTER_SendMainASignal();
    void LIN_MASTER_SendMainARecResponseToUsb();
    void LIN_MASTER_ReceiveMainARecResponse( u8 byte );
    void LIN_MASTER_ReceiveMainARecResponseReset();
    
    void LIN_MASTER_SendFindASignal( u8 id );
    void LIN_MASTER_ReceiveFindARecResponse( u8 byte );
    
    void LIN_MASTER_MainFindASlaveId( bool process );
    void LIN_MASTER_MainMainAReceiveSlaveToUsb();
    void LIN_MASTER_MainMainAReceiveSlaveAndCheck();

    

#endif