/*
 * WDTCON0.h
 *
 *  Created on: 2022. 6. 13.
 *      Author: user
 */

#ifndef SOURCE_WDTCON0_DEF_H_
#define SOURCE_WDTCON0_DEF_H_

#include "IfxScu_reg.h"
#include "IfxScu_regdef.h"

/* SCU Register */
// WDTCON0 : WatchDog Timer Control Register 0
#define WDTCON0_ENDINIT_BIT_LSB_IDX     0U
#define WDTCON0_ENDINIT_MASK            ( 0x1 << WDTCON0_ENDINIT_BIT_LSB_IDX )

#define WDTCON0_LCK_BIT_LSB_IDX         1U
#define WDTCON0_LCK_MASK                ( 0x1 << WDTCON0_LCK_BIT_LSB_IDX )

/* CCU Register for all MOD */
// MOD_CLC : Module Clock Control Register
#define CLC_DISR_BIT_LSB_IDX            0U
#define CLC_DISR_MASK                   ( 0x1 << CLC_DISR_BIT_LSB_IDX )

#define CLC_DISS_BIT_LSB_IDX            1U
#define CLC_DISS_MASK                   ( 0x1 << CLC_DISS_BIT_LSB_IDX )

#endif /* SOURCE_WDTCON0_DEF_H_ */
