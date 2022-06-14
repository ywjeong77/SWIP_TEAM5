/*
 * Timer.h
 *
 *  Created on: 2022. 6. 8.
 *      Author: user
 */

#ifndef SOURCE_TIMER_H_
#define SOURCE_TIMER_H_

#include "IfxScu_reg.h"
#include "IfxScu_regdef.h"

#include "IfxCcu6_reg.h"
#include "IfxCcu6_regdef.h"

#include "IfxSrc_reg.h"
#include "IfxSrc_regdef.h"


/* SCU Register */
// WDTCON0 : WatchDog Timer Control Register 0
#define WDTCON0_ENDINIT_BIT_LSB_IDX     0U
#define WDTCON0_ENDINIT_MASK            ( 0x1 << WDTCON0_ENDINIT_BIT_LSB_IDX )

#define WDTCON0_LCK_BIT_LSB_IDX         1U
#define WDTCON0_LCK_MASK                ( 0x1 << WDTCON0_LCK_BIT_LSB_IDX )

/* CCU Register */
// MOD_CLC : Module Clock Control Register
#define CLC_DISR_BIT_LSB_IDX            0U
#define CLC_DISR_MASK                   ( 0x1 << CLC_DISR_BIT_LSB_IDX )

#define CLC_DISS_BIT_LSB_IDX            1U
#define CLC_DISS_MASK                   ( 0x1 << CLC_DISS_BIT_LSB_IDX )

/* CCU60 Clock Frequency */
#define CCU60_FREQ                      50000000U   // 50MHz
#define CCU60_CLKSEL                    0x2U        // 50MHz / 4 = 12.5MHz
#define CCU60_PRESCALE                  1U          // 12.5MHz / 256 = 48,828Hz
#define CCU60_MODIFIED_FREQ             ( CCU60_FREQ >> ( 2U + 256U * CCU60_PRESCALE ) )

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
#define CLOCK_CYCLE_SHIFT_SIGN          0U      // Sign of shift amount (1 : Negative)
#define CLOCK_CYCLE_SHIFT_VAL           1U      // Magnitude of shift amount

/* SRC Register : Already Defined in "Button_interrupt.h" */
#include "Button_Interrupt.h"

/* Priority Number Tables of SRC */
#define SRPN_TIMER0                     0x0EU

/* Initialization */
void initTimer(void);

#endif /* SOURCE_TIMER_H_ */
