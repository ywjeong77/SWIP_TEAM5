/*
 * Timer.h
 *
 *  Created on: 2022. 6. 8.
 *      Author: user
 */

#ifndef SOURCE_TIMER_H_
#define SOURCE_TIMER_H_

#include "IfxCcu6_reg.h"
#include "IfxCcu6_regdef.h"

#include "IfxSrc_reg.h"
#include "IfxSrc_regdef.h"

#include "IfxScu_regdef.h"

#include "WDTCON0_def.h"

/* CCU60 Clock Frequency */
#define CCU60_FREQ                      50000000U   // 50MHz
#define CCU60_CLKSEL                    0x2U        // 50MHz / 4 = 12.5MHz
#define CCU60_PRESCALE                  0U          // 12.5MHz (no pre-scale)
#define CCU60_MODIFIED_FREQ             ( CCU60_FREQ >> ( CCU60_CLKSEL + 256U * CCU60_PRESCALE ) )

/* CCU61 Clock Frequency */
#define CCU61_FREQ                      50000000U   // 50MHz
#define CCU61_CLKSEL                    0x2U        // 50MHz / 4 = 12.5MHz
#define CCU61_PRESCALE                  1U          // 12.5MHz (no pre-scale)
#define CCU61_MODIFIED_FREQ             ( CCU61_FREQ >> ( CCU61_CLKSEL + 256U * CCU61_PRESCALE ) )
#define CCU61_PR                        100000U

/* CCU60 Register */
// TCTR0 : Timer Control Register 0
#define TCTR0_T12CLK_BIT_LSB_IDX        0U
#define TCTR0_T12CLK_MASK               ( 0x7 << TCTR0_T12CLK_BIT_LSB_IDX )

#define TCTR0_T12PRE_BIT_LSB_IDX        3U
#define TCTR0_T12PRE_MASK               ( 0x1 << TCTR0_T12PRE_BIT_LSB_IDX )

#define TCTR0_CTM_BIT_LSB_IDX           7U
#define TCTR0_T12CTM_MASK               ( 0x1 << TCTR0_CTM_BIT_LSB_IDX )

// TCTR4 : Timer Control Register 4
#define TCTR4_T12STR_BIT_LSB_IDX        6U
#define TCTR4_T12STR_MASK               ( 0x1 << TCTR4_T12STR_BIT_LSB_IDX )

#define TCTR4_T12RES_BIT_LSB_IDX        2U
#define TCTR4_T12RES_MASK               ( 0x1 << TCTR4_T12RES_BIT_LSB_IDX )

#define TCTR4_T12RS_BIT_LSB_IDX         1U
#define TCTR4_T12RS_MASK                ( 0x1 << TCTR4_T12RS_BIT_LSB_IDX )

#define TCTR4_T12RR_BIT_LSB_IDX         0U
#define TCTR4_T12RR_MASK                ( 0x1 << TCTR4_T12RR_BIT_LSB_IDX )

// IEN : Interrupt ENable Register
#define IEN_ENT12PM_BIT_LSB_IDX         7U
#define IEN_ENT12PM_MASK                ( 0x1 << IEN_ENT12PM_BIT_LSB_IDX )

// INP : Interrupt Node Pointer Register
#define INP_INPT12_BIT_LSB_IDX          10U
#define INP_INPT12_MASK                 ( 0x3 << INP_INPT12_BIT_LSB_IDX )
#define INP_INPT12_SR_NUM               0x0U

/* Clock Cycle for Timer Interrupt */
//#define CLOCK_CYCLE_SHIFT_SIGN          1U        // Sign of shift amount (1 : Negative)
//#define CLOCK_CYCLE_SHIFT_VAL           1U        // Magnitude of shift amount
#define CCU60_TIMER_CNT                 125         // 125 / 12.5MHz = 10us

/* SRC Register : Already Defined in "Button_interrupt.h" */
/* SRC Register Configuration */
// SCUERUm Register
#define SRPN_BIT_LSB_IDX        0u
#define SRPN_MASK   ( 0xFFu << SRPN_BIT_LSB_IDX )

#define SRE_BIT_LSB_IDX         10u
#define SRE_MASK    ( 0x1u << SRE_BIT_LSB_IDX )

#define TOS_BIT_LSB_IDX         11u
#define TOS_MASK    ( 0x3u << TOS_BIT_LSB_IDX )

/* Priority Number Tables of SRC */
#define SRPN_TIMER0                     0x0BU
#define SRPN_TIMER1                     0x0AU

/* Initialization */
void initTimer_CCU60(void);
void initTimer_CCU61(void);

/* Additional function */
void setPeriod_CCU60(unsigned int us);
unsigned int readCounter_CCU61(void);

#endif /* SOURCE_TIMER_H_ */
