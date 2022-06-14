/*
 * VADC.h
 *
 *  Created on: 2022. 6. 10.
 *      Author: user
 */

#ifndef SOURCE_VADC_H_
#define SOURCE_VADC_H_
#include "IfxSrc_reg.h"
#include "IfxSrc_regdef.h"

#include "IfxVadc_reg.h"
#include "IfxVadc_regdef.h"

#include "IfxScu_reg.h"
#include "IfxScu_regdef.h"

#include "WDTCON0_def.h"

/* VADC Register */
// GxARBPR : Group x ARBitration Priority Register (x = 0 ~ 7)
#define GXARBPR_PRIO0_BIT_LSB_IDX       0U
#define GXARBPR_PRIO0_MASK              ( 0x3 << GXARBPR_PRIO0_BIT_LSB_IDX )
#define GXARBPR_PRIO1_BIT_LSB_IDX       5U
#define GXARBPR_PRIO1_MASK              ( 0x3 << GXARBPR_PRIO1_BIT_LSB_IDX )
#define GXARBPR_PRIO2_BIT_LSB_IDX       9U
#define GXARBPR_PRIO2_MASK              ( 0x3 << GXARBPR_PRIO2_BIT_LSB_IDX )

#define GXARBPR_CSM0_BIT_LSB_IDX        3U
#define GXARBPR_CSM0_MASK               ( 0x1 << GXARBPR_CSM0_BIT_LSB_IDX )
#define GXARBPR_CSM1_BIT_LSB_IDX        7U
#define GXARBPR_CSM1_MASK               ( 0x1 << GXARBPR_CSM1_BIT_LSB_IDX )
#define GXARBPR_CSM2_BIT_LSB_IDX        11U
#define GXARBPR_CSM2_MASK               ( 0x1 << GXARBPR_CSM2_BIT_LSB_IDX )

#define GXARBPR_ASEN0_BIT_LSB_IDX       24U
#define GXARBPR_ASEN0_MASK              ( 0x1 << GXARBPR_ASEN0_BIT_LSB_IDX )
#define GXARBPR_ASEN1_BIT_LSB_IDX       25U
#define GXARBPR_ASEN1_MASK              ( 0x1 << GXARBPR_ASEN1_BIT_LSB_IDX )
#define GXARBPR_ASEN2_BIT_LSB_IDX       26U
#define GXARBPR_ASEN2_MASK              ( 0x1 << GXARBPR_ASEN2_BIT_LSB_IDX )

// GxQMR0 : Group x Queue 0 Mode Register
#define GXQMR0_ENGT_BIT_LSB_IDX         0U
#define GXQMR0_ENGT_MASK                ( 0x3 << GXQMR0_ENGT_BIT_LSB_IDX )
#define GXQMR0_TREV_BIT_LSB_IDX         9U
#define GXQMR0_TREV_MASK                ( 0x1 << GXQMR0_TREV_BIT_LSB_IDX )
#define GXQMR0_FLUSH_BIT_LSB_IDX        10U
#define GXQMR0_FLUSH_MASK               ( 0x1 << GXQMR0_FLUSH_BIT_LSB_IDX )

// GxARBCFG : Group x ARBitration ConFiGuration
#define GXARBCFG_ANONC_BIT_LSB_IDX      0U
#define GXARBCFG_ANONC_MASK             ( 0x3 << GXARBCFG_ANONC_BIT_LSB_IDX )

// GxICLASS0 : Group x Input CLASS 0
#define GXICLASS_CMS_BIT_LSB_IDX        8U
#define GXICLASS_CMS_MASK               ( 0x7 << GXICLASS_CMS_BIT_LSB_IDX )
#define GXICLASS_STCS_BIT_LSB_IDX       0U
#define GXICLASS_STCS_MASK              ( 0x1F << GXICLASS_CMS_BIT_LSB_IDX )

// GxCHCTRy : Group x Channel y Control Register
#define GXCHCTRY_ICLSEL_BIT_LSB_IDX     0U
#define GXCHCTRY_ICLSEL_MASK            ( 0x3 << GXCHCTRY_ICLSEL_BIT_LSB_IDX )
#define GXCHCTRY_RESREG_BIT_LSB_IDX     16U
#define GXCHCTRY_RESREG_MASK            ( 0xF << GXCHCTRY_RESREG_BIT_LSB_IDX )
#define GXCHCTRY_RESPOS_BIT_LSB_IDX     21U
#define GXCHCTRY_RESPOS_MASK            ( 0x1 << GXCHCTRY_RESPOS_BIT_LSB_IDX )

// GxCHASS : Group x Channel Assignment Register
#define GXCHASS_ASSCH7_BIT_LSB_IDX      7U
#define GXCHASS_ASSCH7_MASK             ( 0x1 << GXCHASS_ASSCH7_BIT_LSB_IDX )

// GxQINR0 : Group x Queue 0 Input Register
#define GXQINR0_REQCHNR_BIT_LSB_IDX     0U
#define GXQINR0_REQCHNR_MASK            ( 0x1F << GXQINR0_REQCHNR_BIT_LSB_IDX )

// GxRESy : Group x Result Register y
#define GXRESY_VF_BIT_LSB_IDX           31U
#define GXRESY_VF_MASK                  ( 0x1 << GXRESY_VF_BIT_LSB_IDX )
#define GXRESY_RESULT_BIT_LSB_IDX       0U
#define GXRESY_RESULT_MASK              ( 0xFFFF << GXRESY_RESULT_BIT_LSB_IDX )

/* VADC Configuration */
#define VADC_G4RES0_VAILD_MASK          0xFFF

/* Initialization */
void initVADC_SAR4_7(void);

/* Send VADC Conversion request */
void requestVADC_SAR4_7(void);

/* Read VADC Result Register */
unsigned int readVADC_SAR4_7(void);

#endif /* SOURCE_VADC_H_ */
