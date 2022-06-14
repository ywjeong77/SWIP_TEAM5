/*
 * Button_Interrupt.c
 *
 *  Created on: 2022. 6. 8.
 *      Author: user
 */

#include "Button_Interrupt.h"

void initInterrupt_Button1(void) {
    /* Button1 Interrupt Configuration : P2.0(REG6) = ERS3 -> EICR1.EXIS1 */
    SCU_EICR1.U &= ~( EXIS1_MASK                  /* Reset Input Selection of ERS2 */
            | FEN1_MASK                           /* Disable Falling-edge */
            | REN1_MASK                           /* Disable Rising-edge */
            | EIEN1_MASK                          /* Disable External Interrupt */
            | INP1_MASK );                        /* Reset Input-Output Connection */

    SCU_EICR1.U |= ( 0x2 << EXIS1_BIT_LSB_IDX     /* Select P2.0 by set EICR1.EXIS1 to 0b010 */
            | 0x1 << FEN1_BIT_LSB_IDX             /* Enable Falling-edge by set EICR1.FEN1 */
            | 0x2 << INP1_BIT_LSB_IDX );          /* Select OGU2 by set EICR1.INP1 to 0b010 */
    SCU_EICR1.U |= ( 0x1 << EIEN1_BIT_LSB_IDX );  /* Enable External Interrupt by set EICR.EIEN1 */

    /* Interrupt Output Configuration : OGU0 -> IGCR0  */
    SCU_IGCR1.U &= ~IGP0_MASK;                    /* Reset Interrupt Gating Pattern of OGU2 */

    SCU_IGCR1.U |= ( 0x1 << IGP0_BIT_LSB_IDX );   /* Set only detect Trigger event by IGCR0.IGP0 */

    /* Service Request Control Configuration : ERU2 */
    // OGU2 -> SCUERU2
    SRC_SCU_SCU_ERU2.U &= ~( SRPN_MASK      /* Reset Service Request Priority Number for ERU Request */
            | SRE_MASK                      /* Disable Service Request */
            | TOS_MASK );                   /* Reset Type of Service Control to initialize CPU 0 */

    SRC_SCU_SCU_ERU2.U |= ( SRPN_BUTTON1 << SRPN_BIT_LSB_IDX );   /* Set Request Priority to 0x0F (Lowest 0x00 -> Highest 0xFF) */
    SRC_SCU_SCU_ERU2.U |= ( 0x1 << SRE_BIT_LSB_IDX );             /* Enable SRC for ERU */
}


void initInterrupt_Button2(void) {
    /* Button2 Interrupt Configuration : P2.1(REG14) = ERS2 -> EICR1.EXIS0 */
    SCU_EICR1.U &= ~( EXIS0_MASK                  /* Reset Input Selection of ERS2 */
            | FEN0_MASK                           /* Disable Falling-edge */
            | REN0_MASK                           /* Disable Rising-edge */
            | EIEN0_MASK                          /* Disable External Interrupt */
            | INP0_MASK );                        /* Select OGU0 by set EICR1.INP0 to 0b000 */

    SCU_EICR1.U |= ( 0x1 << EXIS0_BIT_LSB_IDX     /* Select P2.1 by set EICR1.EXIS0 to 0b001 */
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

    SRC_SCU_SCU_ERU0.U |= ( SRPN_BUTTON2 << SRPN_BIT_LSB_IDX );   /* Set Request Priority to 0x0A (Lowest 0x00 -> Highest 0xFF) */
    SRC_SCU_SCU_ERU0.U |= ( 0x1 << SRE_BIT_LSB_IDX );             /* Enable SRC for ERU */
}
