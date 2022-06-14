/*
 * Button_Control.h
 *
 *  Created on: 2022. 6. 7.
 *      Author: user
 */

#ifndef BUTTON_CONTROL_H_
#define BUTTON_CONTROL_H_

#include "Platform_Types.h"
#include "IfxPort_reg.h"
#include "IfxPort_regdef.h"

/* Button Configuration : P2.0, P2.1 */
// IOCR
#define BUTTON_D2_IOCR_BIT_LSB_IDX 3
#define BUTTON_D3_IOCR_BIT_LSB_IDX 11

// Input Register
#define BUTTON_D2_IN_BIT_LSB_IDX 0
#define BUTTON_D3_IN_BIT_LSB_IDX 1

// Masking
#define BUTTON_D2_IN_MASK 0x1u << BUTTON_D2_IN_BIT_LSB_IDX
#define BUTTON_D3_IN_MASK 0x1u << BUTTON_D3_IN_BIT_LSB_IDX

/* Initialization */
void initButton(void);

#endif /* BUTTON_CONTROL_H_ */
