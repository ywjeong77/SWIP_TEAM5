/*
 * PWM.c
 *
 *  Created on: 2022. 6. 13.
 *      Author: user
 */

#include "PWM.h"
#include "LED_Control.h"

void initGTM(pwm_port_t sel) {

    /* SCU Configuration for enable GTM CLC*/
    // Password Access to unlock WDTCPU0CON0 : SCU_WDTCPU0_CON0
    SCU_WDTCPU0_CON0.U = ( ( ( SCU_WDTCPU0_CON0.U ^ 0xFC )  // overwrite inverted PW[5:0] to unlock WDTCPU0CON0
            & ~( 1 << WDTCON0_LCK_BIT_LSB_IDX ) )           // write 0 to unlock WDTCPU0CON0
            |  ( 1 << WDTCON0_ENDINIT_BIT_LSB_IDX ) );      // cannot access ENDINIT until LCK bit is cleared
                                                            // CAUTION! : Must write in SIMULTANEOUSLY

    while( ( SCU_WDTCPU0_CON0.U & ( 1 << WDTCON0_LCK_BIT_LSB_IDX ) ) != 0 )
        ; // wait until LCK bit changed to 0 (unlock)

    // Modify Access to access Endinit protected register : SCU_WDTCPU0_CON0
    SCU_WDTCPU0_CON0.U = ( ( ( SCU_WDTCPU0_CON0.U ^ 0xFC )  // overwrite inverted PW[5:0] to unlock WDTCPU0CON0
                | ( 1 << WDTCON0_LCK_BIT_LSB_IDX ) )        // write 1 to lock WDTCPU0CON0
                & ~( 1 << WDTCON0_ENDINIT_BIT_LSB_IDX ) );  // clear ENDINIT to access System Critical Register

    while( ( SCU_WDTCPU0_CON0.U & ( 1 << WDTCON0_LCK_BIT_LSB_IDX ) ) == 0 )
        ;   // wait until LCK bit changed to 1 (lock)
            // should Lock while write operation is finished

    GTM_CLC.U &= ~( 0x1 << CLC_DISR_BIT_LSB_IDX );      // enable Module Disable Request for GTM module

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

    /* GTM_CMU Configuration */
    GTM_CMU_FXCLK_CTRL.U &= ~( FXCLK_SEL_MASK );            // set FXCLK Select field as 0b0000 to select CMU_GCLK_EN

//    GTM_CMU_CLK_EN.U &= ~( EN_FXCLK_MASK );                 // reset EN_FXCLK field
    GTM_CMU_CLK_EN.U |= ( 0x2 << EN_FXCLK_BIT_LSB_IDX );    // set EN_FXCLK field as 0b10 to enable all CMU_FXCLK
    // after selection, enable clock input to avoid glitch

    /* GTM_TOM Configuration */
    switch(sel) {
        case PORT_10_1: // TOUT103 (TOM0_1 for Timer A)
            // for TOM0_1 (TOM0, TGC0, CH1)

            /* TOM i global control register : TOMi_TGCn_GLB_CTRL */
//            GTM_TOM0_TGC0_GLB_CTRL.U &= ~( UPEN_CTRL_MASK(0x1U) );                  // reset UPEN_CTRL1 field
            GTM_TOM0_TGC0_GLB_CTRL.U |= ( 0x2U << UPEN_CTRL_BIT_LSB_IDX(0x1U) );    // set UPEN_CTRL1 to enable update of register CM0, CM1, CLK_SRC

//            GTM_TOM0_TGC0_GLB_CTRL.U |= ( 0x1U << HOST_TRIG_BIT_LSB_IDX );          // Trigger request signal to update ENDIS_STAT, OUTEN_STAT
            // like CCU6 Shadow register mechanism

            /* TOM i Enable/Disable Control register : TOMi_TGCn_ENDIS_CTRL */
//            GTM_TOM0_TGC0_ENDIS_CTRL.U &= ~( ENDIS_CTRL_MASK(0x1U) );               // reset ENDIS_CTRL1 field
            GTM_TOM0_TGC0_ENDIS_CTRL.U |= ( 0x2U << ENDIS_CTRL_BIT_LSB_IDX(0x1U) ); // set ENDIS_CTRL1 to send update trigger to Channel 1

            /* TOM i Output Enable Control register : TOMi_TGCn_OUTEN_CTRL */
//            GTM_TOM0_TGC0_OUTEN_CTRL.U &= ~( OUTEN_CTRL_MASK(0x1U) );               // reset OUTEN_CTRL1 field
            GTM_TOM0_TGC0_OUTEN_CTRL.U |= ( 0x2U << OUTEN_CTRL_BIT_LSB_IDX(0x1U) ); // set OUTEN_CTRL1 to accept trigger event of Channel 1

            /* TOM i Channel x Control Register : TOMi_CHx_CTRL (x=0 ~ 14)*/
            GTM_TOM0_CH1_CTRL.U |= ( 0x1U << SL_BIT_LSB_IDX );      // set PWM High during Duty cycle

//            GTM_TOM0_CH1_CTRL.U &= ~( CLK_SRC_SR_MASK );                // reset CLK_SRC_SR field
            GTM_TOM0_CH1_CTRL.U |= ( 0x1U << CLK_SRC_SR_BIT_LSB_IDX );  // set CLK_SRC_SR as CMU_CLK_1 (= CMU_FREQ / ( 2 ^ 4 ) )

            GTM_TOM0_CH1_CTRL.U &= ~( OSM_MASK );       // disable One-shot mode(generate continuous PWM signal)

            GTM_TOM0_CH1_CTRL.U &= ~( TRIG_OUT_MASK );  // set assign TRIG_[x] to TRIG_[x-1]

//            GTM_TOM0_CH1_SR0.U &= ~( SR0_MASK );       // reset shadow register 0 field for CCU0
            GTM_TOM0_CH1_SR0.U = CMU_PERIOD_CNT - 1;    // set PWM Period as 2ms (6250kHz / 500 = 12500)

//            GTM_TOM0_CH1_SR1.U &= ~( SR1_MASK );       // reset shadow register 1 field for CCU0
            GTM_TOM0_CH1_SR1.U = CMU_DUTY_CNT - 1; // set Duty rate as 10% (12500 / 10 = 1250)

            /* Timer Selection for TOUT103 : TOUTSELn */
            GTM_TOUTSEL6.U &= ~( TOUTSEL_SEL_MASK(0x7U) );  // set SEL7 in TOUTSEL6 to assign Timer A to TOUT103;
        break;

        case PORT_10_2: // TOUT104 (TOM0_2 for Timer A)
            // for TOM0_2 (TOM0, TGC0, CH2)
            /* TOM i global control register : TOMi_TGCn_GLB_CTRL */
           GTM_TOM0_TGC0_GLB_CTRL.U |= ( 0x2U << UPEN_CTRL_BIT_LSB_IDX(0x2U) );

            /* TOM i Enable/Disable Control register : TOMi_TGCn_ENDIS_CTRL */
            GTM_TOM0_TGC0_ENDIS_CTRL.U |= ( 0x2U << ENDIS_CTRL_BIT_LSB_IDX(0x2U) );

            /* TOM i Output Enable Control register : TOMi_TGCn_OUTEN_CTRL */
            GTM_TOM0_TGC0_OUTEN_CTRL.U |= ( 0x2U << OUTEN_CTRL_BIT_LSB_IDX(0x2U) );

            /* TOM i Channel x Control Register : TOMi_CHx_CTRL (x=0 ~ 14)*/
            GTM_TOM0_CH2_CTRL.U |= ( 0x1U << SL_BIT_LSB_IDX );
            GTM_TOM0_CH2_CTRL.U |= ( 0x1U << CLK_SRC_SR_BIT_LSB_IDX );
            GTM_TOM0_CH2_CTRL.U &= ~( OSM_MASK );       // disable One-shot mode(generate continuous PWM signal)
            GTM_TOM0_CH2_CTRL.U &= ~( TRIG_OUT_MASK );

            GTM_TOM0_CH2_SR0.U = CMU_PERIOD_CNT - 1;    // set PWM Period as 2ms (6250kHz / 500 = 12500)
            GTM_TOM0_CH2_SR1.U = CMU_DUTY_CNT - 1; // set Duty rate as 10% (12500 / 10 = 1250)

            /* Timer Selection for TOUT103 : TOUTSELn */
            GTM_TOUTSEL7.U &= ~( TOUTSEL_SEL_MASK(0x0U) );  // set SEL0 in TOUTSEL7 to assign Timer A to TOUT104
        break;

        case PORT_2_7: // TOUT7 (TOM0_15 for Timer A)
            // for TOM0_15 (TOM0, TGC1, CH7)
        break;

        case PORT_10_5: // TOUT107 (TOM0_2 for Timer A, TOM2_10 for Timer B)
        break;

        case PORT_10_3: // TOUT105 (TOM0_3 for Timer A)
            // for TOM0_3 (TOM0, TGC0, CH3)
        break;

        case PORT_2_3: // TOUT3 (TOM0_11 for Timer A)
            // for TOM0_11 (TOM0, TGC1, CH3)
            /* TOM i global control register : TOMi_TGCn_GLB_CTRL */
            GTM_TOM0_TGC1_GLB_CTRL.U |= ( 0x2U << UPEN_CTRL_BIT_LSB_IDX(0x3U) );

            /* TOM i Enable/Disable Control register : TOMi_TGCn_ENDIS_CTRL */
            GTM_TOM0_TGC1_ENDIS_CTRL.U |= ( 0x2U << ENDIS_CTRL_BIT_LSB_IDX(0x3U) );

            /* TOM i Output Enable Control register : TOMi_TGCn_OUTEN_CTRL */
            GTM_TOM0_TGC1_OUTEN_CTRL.U |= ( 0x2U << OUTEN_CTRL_BIT_LSB_IDX(0x3U) );

            /* TOM i Channel x Control Register : TOMi_CHx_CTRL (x=0 ~ 14)*/
            GTM_TOM0_CH11_CTRL.U |= ( 0x1U << SL_BIT_LSB_IDX );
            GTM_TOM0_CH11_CTRL.U |= ( 0x1U << CLK_SRC_SR_BIT_LSB_IDX );
            GTM_TOM0_CH11_CTRL.U &= ~( OSM_MASK );       // disable One-shot mode(generate continuous PWM signal)
            GTM_TOM0_CH11_CTRL.U &= ~( TRIG_OUT_MASK );

            GTM_TOM0_CH11_SR0.U = CMU_PERIOD_CNT - 1;    // set PWM Period as 2ms (6250kHz / 500 = 12500)
            GTM_TOM0_CH11_SR1.U = CMU_DUTY_CNT - 1; // set Duty rate as 10% (12500 / 10 = 1250)

            /* Timer Selection for TOUT3 : TOUTSELn */
            GTM_TOUTSEL0.U &= ~( TOUTSEL_SEL_MASK(0x3U) );  // set SEL3 in TOUTSEL0 to assign Timer A to TOUT104
        break;
    }
}

void startPWM(pwm_port_t sel) {
    switch(sel) {
        case PORT_10_1: // TOUT103 (TOM0_1 for Timer A)
            GTM_TOM0_TGC0_GLB_CTRL.U |= ( 0x1U << HOST_TRIG_BIT_LSB_IDX );
        break;

        case PORT_10_2: // TOUT104 (TOM0_2 for Timer A)
            GTM_TOM0_TGC0_GLB_CTRL.U |= ( 0x1U << HOST_TRIG_BIT_LSB_IDX );
        break;

        case PORT_2_7: // TOUT7 (TOM0_15 for Timer A)

        break;

        case PORT_10_5: // TOUT107 (TOM0_2 for Timer A, TOM2_10 for Timer B)

        break;

        case PORT_10_3: // TOUT105 (TOM0_3 for Timer A)

        break;

        case PORT_2_3: // TOUT3 (TOM0_11 for Timer A)
            GTM_TOM0_TGC1_GLB_CTRL.U |= ( 0x1U << HOST_TRIG_BIT_LSB_IDX );
        break;
    }

}

void controlPWM(pwm_port_t sel, int enable){
    switch(sel) {
        case PORT_10_1: // TOUT103 (TOM0_1 for Timer A)
            GTM_TOM0_TGC0_ENDIS_CTRL.U |= ( ( (enable) ? 0x2U : 0x1U ) << ENDIS_CTRL_BIT_LSB_IDX(0x1U) );
        break;

        case PORT_10_2: // TOUT104 (TOM0_2 for Timer A)
            GTM_TOM0_TGC0_ENDIS_CTRL.U |= ( ( (enable) ? 0x2U : 0x1U ) << ENDIS_CTRL_BIT_LSB_IDX(0x2U) );
        break;

        case PORT_2_7: // TOUT7 (TOM0_15 for Timer A)
        break;

        case PORT_10_5: // TOUT107 (TOM0_2 for Timer A, TOM2_10 for Timer B)
        break;

        case PORT_10_3: // TOUT105 (TOM0_3 for Timer A)
        break;

        case PORT_2_3: // TOUT3 (TOM0_11 for Timer A)
            GTM_TOM0_TGC1_ENDIS_CTRL.U |= ( ( (enable) ? 0x2U : 0x1U ) << ENDIS_CTRL_BIT_LSB_IDX(0x3U) );
        break;
    }
}

void changePWM(pwm_port_t sel, unsigned short period, unsigned short duty) {
    switch(sel) {
        case PORT_10_1: // TOUT103 (TOM0_1 for Timer A)
            if(period)
                GTM_TOM0_CH1_SR0.U = period - 1;

            if(duty)
                GTM_TOM0_CH1_SR1.U = duty - 1;

            // for TOM0_1 (TOM0, TGC0, CH1)
        break;

        case PORT_10_2: // TOUT104 (TOM0_2 for Timer A)
            if(period)
                GTM_TOM0_CH2_SR0.U = period - 1;

            if(duty)
                GTM_TOM0_CH2_SR1.U = duty - 1;
            // for TOM0_2 (TOM0, TGC0, CH2)
        break;

        case PORT_2_7: // TOUT7 (TOM0_15 for Timer A)
            // for TOM0_15 (TOM0, TGC1, CH7)
            break;

        case PORT_10_5: // TOUT107 (TOM0_2 for Timer A, TOM2_10 for Timer B)
            // for TOM2_10 (TOM2, TGC1, CH2)
        break;

        case PORT_10_3: // TOUT105 (TOM0_3 for Timer A)
            // for TOM0_3 (TOM0, TGC0, CH3)
        break;

        case PORT_2_3: // TOUT3 (TOM0_11 for Timer A)
            if(period)
                GTM_TOM0_CH11_SR0.U = period - 1;

            if(duty)
                GTM_TOM0_CH11_SR1.U = duty - 1;
            // for TOM0_11 (TOM0, TGC1, CH3)
        break;
    }
}

void initLED1_PWM(void) {
    // Reset P10.1 IOCR
    P10_IOCR0.U &= ~(0x1F << LED_D12_IOCR_BIT_LSB_IDX);

    // Set P10.1 IOCR to set as 0b1X001 = 0x11 (pwm output mode : TOUT103, 104)
    P10_IOCR0.U |= 0x11 << LED_D12_IOCR_BIT_LSB_IDX;
}

void initLED2_PWM(void) {
    // Reset P10.2 IOCR
    P10_IOCR0.U &= ~(0x1F << LED_D13_IOCR_BIT_LSB_IDX);

    // Set P10.2 IOCR to set as 0b1X001 = 0x11 (pwm output mode : TOUT103, 104)
    P10_IOCR0.U |= 0x11 << LED_D13_IOCR_BIT_LSB_IDX;
}

void initRGBLED_PWM(void) {
    // Reset P2.7, P10.5, P10.3
    P02_IOCR4.U &= ~(0x1Fu << RGB_D09_IOCR_BIT_LSB_IDX);
    P10_IOCR0.U &= ~(0x1Fu << RGB_D10_IOCR_BIT_LSB_IDX);
    P10_IOCR4.U &= ~(0x1Fu << RGB_D11_IOCR_BIT_LSB_IDX);

    // Set P2.7, P10.5, P10.3 IOCR to set as 0b1X001 = 0x11 (pwm output mode : TOUT7, 107, 105)
    P02_IOCR4.U |= 0x11 << RGB_D09_IOCR_BIT_LSB_IDX;
    P10_IOCR0.U |= 0x11 << RGB_D10_IOCR_BIT_LSB_IDX;
    P10_IOCR4.U |= 0x11 << RGB_D11_IOCR_BIT_LSB_IDX;
}

void initBuzzer_PWM(void) {
    // Reset P2.3 IOCR
    P02_IOCR0.B.PC3 &= ~0x1Fu;

    // Set P2.3 to set as 0b1X001 = 0x11 (pwm output mode : TOUT3)
    P02_IOCR0.B.PC3 |= 0x11;
}
