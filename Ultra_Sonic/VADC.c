/*
 * VADC.c
 *
 *  Created on: 2022. 6. 10.
 *      Author: user
 */

#include "VADC.h"

void initVADC_SAR4_7(void) {

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

    /* enable CLC for VADC (Endinit clear condition) */
    VADC_CLC.U &= ~( 0x1 << CLC_DISR_BIT_LSB_IDX );      // enable Clock Enable for VADC module

    while((VADC_CLC.U & (1 << CLC_DISS_BIT_LSB_IDX)) != 0);

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


    /* Group 4 Arbitration Priority configuration for VADC Request Source 0 */
    VADC_G4_ARBPR.U &= ~( GXARBPR_PRIO0_MASK        // reset priority field
            | GXARBPR_CSM0_MASK                     // set as Wait-for-Start mode
            | GXARBPR_ASEN0_MASK );                 // reset Arbitration priority
    VADC_G4_ARBPR.U |=  ( 0x3 << GXARBPR_PRIO0_BIT_LSB_IDX )    // set highest priority by set 0b11
            | ( 0x1 << GXARBPR_ASEN0_BIT_LSB_IDX );             // enable Arbitration slot 0

    /* Group 4 'Request' Queue 0 Mode Configuration */
    VADC_G4_QMR0.U &= ~( GXQMR0_ENGT_MASK );                    // reset enable gating field
    VADC_G4_QMR0.U |=  ( 0x1 << GXQMR0_ENGT_BIT_LSB_IDX );      // set only refer Conversion Request in Queue 0, Backup Reg.
    VADC_G4_QMR0.U |=  ( 0x1 << GXQMR0_FLUSH_BIT_LSB_IDX );     // clear Queue 0 entries

    /* Group 4 ARBtriation Configuration */
    VADC_G4_ARBCFG.U &= ~( GXARBCFG_ANONC_MASK );    // reset Analog converter control mode field
    VADC_G4_ARBCFG.U |=  ( 0x3 << GXARBCFG_ANONC_BIT_LSB_IDX ); // set Normal Operation

    /* Group 4 Input Class 0 Configuration */
    VADC_G4_ICLASS0.U &= ~( GXICLASS_CMS_MASK );     // set Conversion mode to 12bit conversion

    /* Group 4 Input Channel 7 Control Configuration */
    VADC_G4_CHCTR7.U &= ~( GXCHCTRY_ICLSEL_MASK         // set group-specific class 0 (= Group4 Class 0)
            | GXCHCTRY_RESREG_MASK );                   // set stored result register to GxRES0
    VADC_G4_CHCTR7.U |= ( 0x1 << GXCHCTRY_RESPOS_BIT_LSB_IDX ); // set conversion result data alignment to left-aligned

    /* Group 4 Priority Channel Assignment Configuration */
    VADC_G4_CHASS.U |= ( 0x1 << GXCHASS_ASSCH7_BIT_LSB_IDX );   // enable priority for Group 4 Channel 7
}

void requestVADC_SAR4_7(void) {

    /* Group 4 Queue 0 Input Register Configuration */
    VADC_G4_QINR0.U |= ( 0x7 << GXQINR0_REQCHNR_BIT_LSB_IDX );  // set Request Input Channel as Channel 7

    /* Group 4 Queue 0 Mode Configuration */
    VADC_G4_QMR0.U |=  ( 0x1 << GXQMR0_TREV_BIT_LSB_IDX );      // generate Trigger event

}

unsigned int readVADC_SAR4_7(void) {

    while( !( VADC_G4_RES0.U & GXRESY_VF_MASK ) )
        ; // Wait Until Conversion result updated

    return ( VADC_G4_RES0.U  & VADC_G4RES0_VAILD_MASK ); // return VADC Result Register 0
}
