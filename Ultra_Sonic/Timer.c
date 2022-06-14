/*
 * Timer.c
 *
 *  Created on: 2022. 6. 8.
 *      Author: user
 */

#include "Timer.h"

void initTimer_CCU60(void) {

    /* Password Access to unlock WDTCPU0CON0 : SCU_WDTCPU0_CON0 */
    SCU_WDTCPU0_CON0.U = ( ( ( SCU_WDTCPU0_CON0.U ^ 0xFC )  // overwrite inverted PW[5:0] to unlock WDTCPU0CON0
            & ~( 1 << WDTCON0_LCK_BIT_LSB_IDX ) )           // write 0 to unlock WDTCPU0CON0
            |  ( 1 << WDTCON0_ENDINIT_BIT_LSB_IDX ) );      // cannot access ENDINIT until LCK bit is cleared
                                                            // CAUTION! : Must write in SIMULTANEOUSLY

    while( ( SCU_WDTCPU0_CON0.U & ( 1 << WDTCON0_LCK_BIT_LSB_IDX ) ) != 0 ); // wait until LCK bit changed to 0 (unlock)


    /* Modify Access to access Endinit protected register : SCU_WDTCPU0_CON0 */
    SCU_WDTCPU0_CON0.U = ( ( ( SCU_WDTCPU0_CON0.U ^ 0xFC )  // overwrite inverted PW[5:0] to unlock WDTCPU0CON0
                | ( 1 << WDTCON0_LCK_BIT_LSB_IDX ) )        // write 1 to lock WDTCPU0CON0
                & ~( 1 << WDTCON0_ENDINIT_BIT_LSB_IDX ) );  // clear ENDINIT to access System Critical Register

    while( ( SCU_WDTCPU0_CON0.U & ( 1 << WDTCON0_LCK_BIT_LSB_IDX ) ) == 0 )
        ;   // wait until LCK bit changed to 1 (lock)
            // should Lock while write operation is finished


    /* Enable CCU60 Clock : CCU60_CLC */
    CCU60_CLC.U &= ~( 1 << CLC_DISR_BIT_LSB_IDX );      // enable Clock Enable for CCU60 module


    /* Password Access to unlock WDTCPU0CON0 : SCU_WDTCPU0_CON0 */
    SCU_WDTCPU0_CON0.U = ( ( ( SCU_WDTCPU0_CON0.U ^ 0xFC )  // overwrite inverted PW[5:0] to unlock WDTCPU0CON0
            & ~( 1 << WDTCON0_LCK_BIT_LSB_IDX ) )           // write 0 to unlock WDTCPU0CON0
            |  ( 1 << WDTCON0_ENDINIT_BIT_LSB_IDX ) );      // cannot access ENDINIT until LCK bit is cleared

    while( ( SCU_WDTCPU0_CON0.U & ( 1 << WDTCON0_LCK_BIT_LSB_IDX ) ) != 0 )
        ;   // wait until LCK bit changed to 0 (unlock)


    /* Modify Access not to access Endinit protected register : SCU_WDTCPU0_CON0 */
    SCU_WDTCPU0_CON0.U = ( ( ( SCU_WDTCPU0_CON0.U ^ 0xFC )  // overwrite inverted PW[5:0] to unlock WDTCPU0CON0
                | ( 1 << WDTCON0_LCK_BIT_LSB_IDX ) )        // write 1 to lock WDTCPU0CON0
                | ( 1 << WDTCON0_ENDINIT_BIT_LSB_IDX ) );   // set ENDINIT to access System Critical Register

    while( ( SCU_WDTCPU0_CON0.U & ( 1 << WDTCON0_LCK_BIT_LSB_IDX ) ) == 0 )
        ;   // wait until LCK bit changed to 1 (lock)

    /* Check if CCU60 Clock is enabled : CCU60_CLC */
    while( ( CCU60_CLC.U & ( 1 << CLC_DISS_BIT_LSB_IDX ) ) != 0 )
        ;   // wait until DISS bit changed to 0 (finish enable clock)


    /* Modify CCU60 Clock Frequency by Pre-scaler(by 256), Selector(by 2^(0 ~ 7)) */
    CCU60_TCTR0.U &= ~TCTR0_T12CLK_MASK;                            // reset T12CLK field
    CCU60_TCTR0.U |= ( CCU60_CLKSEL << TCTR0_T12CLK_BIT_LSB_IDX )   // set Clock selection to freq_CCU60 / 4 by set 0b010
            | ( CCU60_PRESCALE << TCTR0_T12PRE_BIT_LSB_IDX ) ;      // set Prescale enable to freq_CCU60 / 256

    /* Set T12 to Edge-aligned mode */
    CCU60_TCTR0.U &= ~( 0x1 << TCTR0_CTM_BIT_LSB_IDX );     // T12 auto reset when PM occured

    /* Set Period Match */
    CCU60_T12PR.U = CCU60_TIMER_CNT - 1;    // PM interrupt freq. = freq_T12 / ( T12PR + 1 )
                                            // throw interrupt in every 0.5s period

    CCU60_TCTR2.B.T12SSC = 0x1;             // set single shot control

    /* Load T12PR from shadow Register */
    CCU60_TCTR4.U |= ( 0x1 << TCTR4_T12STR_BIT_LSB_IDX );       // transfer shadow reg. from STE12 to T12

    /* Clear T12 Counter Register */
    //CCU60_T12.U = 0;                                            // reset T12CV to 0
    CCU60_TCTR4.U |= ( 0x1 << TCTR4_T12RES_BIT_LSB_IDX );

    /* CCU60 T12 PM Interrupt Configuration */
    CCU60_INP.U &= ~INP_INPT12_MASK;
    CCU60_INP.U |= ( INP_INPT12_SR_NUM << INP_INPT12_BIT_LSB_IDX ); // Select Interrupt Output to SR0

    CCU60_IEN.U |= ( 0x1 << IEN_ENT12PM_BIT_LSB_IDX );          // enable T12 Period-Match Interrupt


    /* SRC Configuration for CCU60 */
    SRC_CCU6_CCU60_SR0.U &= ~( SRPN_MASK
            | TOS_MASK );                                       // reset SRPN, TOS field of SRC for CCU60
    SRC_CCU6_CCU60_SR0.U |= ( SRPN_TIMER0 << SRPN_BIT_LSB_IDX ) // set CCU60 Interrupt SRPN to 0x0E
            | ( 0x0 << TOS_BIT_LSB_IDX );                       // transfer CCU60 Interrupt to CPU0
    SRC_CCU6_CCU60_SR0.U |= ( 0x1 << SRE_BIT_LSB_IDX );         // enable CCU60 Interrupt Service Request


    /* Run T12 by setting T12R : Disabled */
//    CCU60_TCTR4.U |=  ( 0x1 << TCTR4_T12RS_BIT_LSB_IDX );       // set T12RS to set T12R (T12R is triggered by T12RS, T12RR )
}

void initTimer_CCU61(void) {

    /* Password Access to unlock WDTCPU0CON0 : SCU_WDTCPU0_CON0 */
    SCU_WDTCPU0_CON0.U = ( ( ( SCU_WDTCPU0_CON0.U ^ 0xFC )  // overwrite inverted PW[5:0] to unlock WDTCPU0CON0
            & ~( 1 << WDTCON0_LCK_BIT_LSB_IDX ) )           // write 0 to unlock WDTCPU0CON0
            |  ( 1 << WDTCON0_ENDINIT_BIT_LSB_IDX ) );      // cannot access ENDINIT until LCK bit is cleared
                                                            // CAUTION! : Must write in SIMULTANEOUSLY

    while( ( SCU_WDTCPU0_CON0.U & ( 1 << WDTCON0_LCK_BIT_LSB_IDX ) ) != 0 ); // wait until LCK bit changed to 0 (unlock)


    /* Modify Access to access Endinit protected register : SCU_WDTCPU0_CON0 */
    SCU_WDTCPU0_CON0.U = ( ( ( SCU_WDTCPU0_CON0.U ^ 0xFC )  // overwrite inverted PW[5:0] to unlock WDTCPU0CON0
                | ( 1 << WDTCON0_LCK_BIT_LSB_IDX ) )        // write 1 to lock WDTCPU0CON0
                & ~( 1 << WDTCON0_ENDINIT_BIT_LSB_IDX ) );  // clear ENDINIT to access System Critical Register

    while( ( SCU_WDTCPU0_CON0.U & ( 1 << WDTCON0_LCK_BIT_LSB_IDX ) ) == 0 )
        ;   // wait until LCK bit changed to 1 (lock)
            // should Lock while write operation is finished


    /* Enable CCU61 Clock : CCU60_CLC */
    CCU61_CLC.U &= ~( 1 << CLC_DISR_BIT_LSB_IDX );      // enable Clock Enable for CCU60 module


    /* Password Access to unlock WDTCPU0CON0 : SCU_WDTCPU0_CON0 */
    SCU_WDTCPU0_CON0.U = ( ( ( SCU_WDTCPU0_CON0.U ^ 0xFC )  // overwrite inverted PW[5:0] to unlock WDTCPU0CON0
            & ~( 1 << WDTCON0_LCK_BIT_LSB_IDX ) )           // write 0 to unlock WDTCPU0CON0
            |  ( 1 << WDTCON0_ENDINIT_BIT_LSB_IDX ) );      // cannot access ENDINIT until LCK bit is cleared

    while( ( SCU_WDTCPU0_CON0.U & ( 1 << WDTCON0_LCK_BIT_LSB_IDX ) ) != 0 )
        ;   // wait until LCK bit changed to 0 (unlock)


    /* Modify Access not to access Endinit protected register : SCU_WDTCPU0_CON0 */
    SCU_WDTCPU0_CON0.U = ( ( ( SCU_WDTCPU0_CON0.U ^ 0xFC )  // overwrite inverted PW[5:0] to unlock WDTCPU0CON0
                | ( 1 << WDTCON0_LCK_BIT_LSB_IDX ) )        // write 1 to lock WDTCPU0CON0
                | ( 1 << WDTCON0_ENDINIT_BIT_LSB_IDX ) );   // set ENDINIT to access System Critical Register

    while( ( SCU_WDTCPU0_CON0.U & ( 1 << WDTCON0_LCK_BIT_LSB_IDX ) ) == 0 )
        ;   // wait until LCK bit changed to 1 (lock)

    /* Check if CCU60 Clock is enabled : CCU61_CLC */
    while( ( CCU61_CLC.U & ( 1 << CLC_DISS_BIT_LSB_IDX ) ) != 0 )
        ;   // wait until DISS bit changed to 0 (finish enable clock)


    /* Modify CCU60 Clock Frequency by Pre-scaler(by 256), Selector(by 2^(0 ~ 7)) */
    CCU61_TCTR0.U &= ~TCTR0_T12CLK_MASK;                            // reset T12CLK field
    CCU61_TCTR0.U |= ( CCU61_CLKSEL << TCTR0_T12CLK_BIT_LSB_IDX )   // set Clock selection to freq_CCU60 / 4 by set 0b010
            | ( CCU61_PRESCALE << TCTR0_T12PRE_BIT_LSB_IDX ) ;      // set Prescale enable to freq_CCU60 / 256

    /* Set T12 to Edge-aligned mode */
    CCU61_TCTR0.U &= ~( 0x1 << TCTR0_CTM_BIT_LSB_IDX );     // T12 auto reset when PM occured

    /* Set Period Match */
    CCU61_T12PR.U = CCU61_PR - 1;           // PM interrupt freq. = max value not to occur PM
    CCU61_TCTR2.B.T12SSC = 0x1;             // set single shot control

    /* Load T12PR from shadow Register */
    CCU61_TCTR4.U |= ( 0x1 << TCTR4_T12STR_BIT_LSB_IDX );       // transfer shadow reg. from STE12 to T12

    /* Clear T12 Counter Register */
    //CCU60_T12.U = 0;                                            // reset T12CV to 0
    CCU61_TCTR4.U |= ( 0x1 << TCTR4_T12RES_BIT_LSB_IDX );

    /* CCU60 T12 PM Interrupt Configuration */
    CCU61_INP.U &= ~INP_INPT12_MASK;
    CCU61_INP.U |= ( INP_INPT12_SR_NUM << INP_INPT12_BIT_LSB_IDX ); // Select Interrupt Output to SR0

    CCU61_IEN.U |= ( 0x1 << IEN_ENT12PM_BIT_LSB_IDX );          // enable T12 Period-Match Interrupt


    /* SRC Configuration for CCU61 */
    SRC_CCU6_CCU61_SR0.U &= ~( SRPN_MASK
            | TOS_MASK );                                       // reset SRPN, TOS field of SRC for CCU60
    SRC_CCU6_CCU61_SR0.U |= ( SRPN_TIMER0 << SRPN_BIT_LSB_IDX ) // set CCU61 Interrupt SRPN to 0x0E
            | ( 0x0 << TOS_BIT_LSB_IDX );                       // transfer CCU61 Interrupt to CPU0
    SRC_CCU6_CCU61_SR0.U |= ( 0x1 << SRE_BIT_LSB_IDX );         // enable CCU61 Interrupt Service Request


}

void setPreiod_CCU60(unsigned int us) {
    CCU60_T12PR.U = (unsigned short)( CCU60_TIMER_CNT / 1000000U * us ) - 1;

    /* Load T12PR from shadow Register */
    CCU60_TCTR4.U |= ( 0x1 << TCTR4_T12STR_BIT_LSB_IDX );   // transfer shadow reg. from STE12 to T12
}

unsigned int readCounter_CCU61(void) {
    return 0;
}

