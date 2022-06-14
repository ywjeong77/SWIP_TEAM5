/*
 * Subsonic_Sensor.c
 *
 *  Created on: 2022. 6. 14.
 *      Author: user
 */

#include "Ultrasonic.h"
#include "Timer.h"


void initUltrasonic(void) {
    // Reset P02.6, P00.4 IOCR
    P02_IOCR4.B.PC6 &= 0x0;
    P00_IOCR4.B.PC4 &= 0x0;

    // Set P02.6, P00.4 IOCR
    P02_IOCR4.B.PC6 |= 0x10; // set TRIG pin as push-pull general output
    P00_IOCR4.B.PC4 |= 0x01; // set ECHO pin as general input (pull-down connected)

    // Set P02.6 Output as Low (TRIG pin)
    P02_OUT.B.P6 &= 0x0;
}

void trigUltrasonic(void) {
    // to start 10us TRIG flag
    P02_OUT.B.P6 |= 0x1;
    range_valid_flag = 0;       // reset flag during TRIG transmit phase

    /* Run T12 by setting T12R */
    CCU60_TCTR4.B.T12RS |= 0x1; // Run request to start CCU60 T12 Timer
}

void initInterrupt_CCU61(void) {
    /* CCU60 Interrupt Configuration : P0.1(REG14) = ERS2 -> EICR1.EXIS0 */
    SCU_EICR1.U &= ~( EXIS0_MASK                  /* Reset Input Selection of ERS2 */
            | FEN0_MASK                           /* Disable Falling-edge */
            | REN0_MASK                           /* Disable Rising-edge */
            | EIEN0_MASK                          /* Disable External Interrupt */
            | INP0_MASK );                        /* Select OGU0 by set EICR1.INP0 to 0b000 */

    SCU_EICR1.U |= ( 0x2 << EXIS0_BIT_LSB_IDX     /* Select P0.4 by set EICR1.EXIS0 to 0b001 */
            | 0x1 << FEN0_BIT_LSB_IDX             /* Enable Falling-edge by set EICR1.FEN0 */
            | 0x1 << REN0_BIT_LSB_IDX);           /* Enable Rising-edge by set EICR1.REN0 */
    SCU_EICR1.U |= ( 0x1 << EIEN0_BIT_LSB_IDX );  /* Enable External Interrupt by set EICR.EIEN0 */

    /* Interrupt Output Configuration : OGU0 -> IGCR0  */
    SCU_IGCR0.U &= ~IGP0_MASK;                    /* Reset Interrupt Gating Pattern of OGU0 */

    SCU_IGCR0.U |= ( 0x1 << IGP0_BIT_LSB_IDX );   /* Set only detect Trigger event by IGCR0.IGP0 */

    /* Service Request Control Configuration : ERU2 */
    // ERS2 -> ERU0 -> SCUERU0
    SRC_SCU_SCU_ERU0.U &= ~( SRPN_MASK      /* Reset Service Request Priority Number for ERU Request */
            | SRE_MASK                      /* Disable Service Request */
            | TOS_MASK );                   /* Reset Type of Service Control to initialize CPU 0 */

    SRC_SCU_SCU_ERU0.U |= ( SRPN_CCU61 << SRPN_BIT_LSB_IDX );   /* Set Request Priority to 0x0A (Lowest 0x00 -> Highest 0xFF) */
    SRC_SCU_SCU_ERU0.U |= ( 0x1 << SRE_BIT_LSB_IDX );             /* Enable SRC for ERU */
}

