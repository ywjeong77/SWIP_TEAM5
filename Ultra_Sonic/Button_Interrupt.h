/*
 * Button_Interrupt.h
 *
 *  Created on: 2022. 6. 8.
 *      Author: user
 */

#ifndef SOURCE_BUTTON_INTERRUPT_H_
#define SOURCE_BUTTON_INTERRUPT_H_

#include "IfxScu_reg.h"
#include "IfxScu_regdef.h"

#include "IfxSrc_reg.h"
#include "IfxSrc_regdef.h"

/* SCU Registers Configuration */
// EICRm Register
#define EICR_OFFSET             16u

#define EXIS0_BIT_LSB_IDX       4u
#define EXIS0_MASK    ( 0x7u << EXIS0_BIT_LSB_IDX )
#define EXIS1_BIT_LSB_IDX     ( 4u + EICR_OFFSET )
#define EXIS1_MASK    ( 0x7u << EXIS1_BIT_LSB_IDX )

#define FEN0_BIT_LSB_IDX        8u
#define FEN0_MASK     ( 0x1u << FEN0_BIT_LSB_IDX )
#define FEN1_BIT_LSB_IDX      ( 8u + EICR_OFFSET )
#define FEN1_MASK     ( 0x1u << FEN1_BIT_LSB_IDX )

#define REN0_BIT_LSB_IDX        9u
#define REN0_MASK     ( 0x1u << REN0_BIT_LSB_IDX )
#define REN1_BIT_LSB_IDX      ( 9u + EICR_OFFSET )
#define REN1_MASK     ( 0x1u << REN1_BIT_LSB_IDX )

#define LDEN0_BIT_LSB_IDX       10u
#define LDEN0_MASK    ( 0x1u << LDEN0_BIT_LSB_IDX )
#define LDEN1_BIT_LSB_IDX     ( 10u + EICR_OFFSET )
#define LDEN1_MASK    ( 0x1u << LDEN1_BIT_LSB_IDX )

#define EIEN0_BIT_LSB_IDX       11u
#define EIEN0_MASK    ( 0x1u << EIEN0_BIT_LSB_IDX )
#define EIEN1_BIT_LSB_IDX     ( 11u + EICR_OFFSET )
#define EIEN1_MASK    ( 0x1u << EIEN1_BIT_LSB_IDX )

#define INP0_BIT_LSB_IDX        12u
#define INP0_MASK     ( 0x7u << INP0_BIT_LSB_IDX )
#define INP1_BIT_LSB_IDX      ( 12u + EICR_OFFSET )
#define INP1_MASK     ( 0x7u << INP1_BIT_LSB_IDX )

// IGCRm Register
#define IGCR_OFFSET             16u

#define IGP0_BIT_LSB_IDX        14u
#define IGP0_MASK     ( 0x3u << IGP0_BIT_LSB_IDX )
#define IGP1_BIT_LSB_IDX      ( 14u + IGCR_OFFSET )
#define IGP1_MASK     ( 0x3u << IGP1_BIT_LSB_IDX )

/* SRC Register Configuration */
// SCUERUm Register
#define SRPN_BIT_LSB_IDX        0u
#define SRPN_MASK   ( 0xFFu << SRPN_BIT_LSB_IDX )

#define SRE_BIT_LSB_IDX         10u
#define SRE_MASK    ( 0x1u << SRE_BIT_LSB_IDX )

#define TOS_BIT_LSB_IDX         11u
#define TOS_MASK    ( 0x3u << TOS_BIT_LSB_IDX )

/* Priority Number Tables of SRC */
#define SRPN_BUTTON1            0x10u
#define SRPN_BUTTON2            0x0Fu

/* Initialization */
void initInterrupt_Button1(void);
void initInterrupt_Button2(void);

#endif /* SOURCE_BUTTON_INTERRUPT_H_ */
