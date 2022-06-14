/*
 * LED_Control.h
 *
 *  Created on: 2022. 6. 7.
 *      Author: user
 */

#ifndef LED_CONTROL_H_
#define LED_CONTROL_H_

#include "IfxPort_reg.h"
#include "IfxPort_regdef.h"

/* Register Configuration */
#define OMR_PCL_OFFSET 16

/* LED1, 2 Configuration : P10.1, P10.2 */
// IOCR
#define LED_D12_IOCR_BIT_LSB_IDX 11 // D12 : P10_IOCR0.PC1
#define LED_D13_IOCR_BIT_LSB_IDX 19 // D13 : P10_IOCR0.PC2

// Output Register
#define LED_D12_OUT_BIT_LSB_IDX 1
#define LED_D13_OUT_BIT_LSB_IDX 2

// OMR Mask
#define LED_D13_OMR_PCL_MASK    (0x1 << (1 + OMR_PCL_OFFSET))
#define LED_D13_OMR_PS_MASK     (0x1 << 1)
#define LED_D13_OMR_TOGGLE_MASK LED_D13_OMR_PCL_MASK | LED_D13_OMR_PS_MASK

#define LED_D12_OMR_PCL_MASK    (0x1 << (2 + OMR_PCL_OFFSET))
#define LED_D12_OMR_PS_MASK     (0x1 << 2)
#define LED_D12_OMR_TOGGLE_MASK LED_D14_OMR_PCL_MASK | LED_D14_OMR_PS_MASK

/* RGB LED Configuration : P2.7, P10.5, P10.3 */
// IOCR
#define RGB_D09_IOCR_BIT_LSB_IDX 27 // D09 : P02_IOCR4.PC7
#define RGB_D10_IOCR_BIT_LSB_IDX 27 // D10 : P10_IOCR0.PC3
#define RGB_D11_IOCR_BIT_LSB_IDX 11 // D11 : P10_IOCR4.PC5

// Output Register
#define RGB_D09_OUT_BIT_LSB_IDX 7
#define RGB_D10_OUT_BIT_LSB_IDX 5
#define RGB_D11_OUT_BIT_LSB_IDX 3

// OMR Mask
#define RGB_D09_OMR_PCL_MASK    (0x1 << (7 + OMR_PCL_OFFSET))
#define RGB_D09_OMR_PS_MASK     (0x1 << 7)
#define RGB_D09_OMR_TOGGLE_MASK RGB_D09_OMR_PCL_MASK | RGB_D09_OMR_PS_MASK

#define RGB_D10_OMR_PCL_MASK    (0x1 << (3 + OMR_PCL_OFFSET))
#define RGB_D10_OMR_PS_MASK     (0x1 << 3)
#define RGB_D10_OMR_TOGGLE_MASK RGB_D10_OMR_PCL_MASK | RGB_D10_OMR_PS_MASK

#define RGB_D11_OMR_PCL_MASK    (0x1 << (5 + OMR_PCL_OFFSET))
#define RGB_D11_OMR_PS_MASK     (0x1 << 5)
#define RGB_D11_OMR_TOGGLE_MASK RGB_D11_OMR_PCL_MASK | RGB_D11_OMR_PS_MASK

/* Initialization */
void initLED(void);
void initRGBLED(void);

#endif /* LED_CONTROL_H_ */
