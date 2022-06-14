/*
 * LED_Control.c
 *
 *  Created on: 2022. 6. 7.
 *      Author: user
 */

#include "LED_Control.h"

void initLED(void) {
    // Reset P10.1,2 IOCR
    P10_IOCR0.U &= ~(0x1F << LED_D13_IOCR_BIT_LSB_IDX);
    P10_IOCR0.U &= ~(0x1F << LED_D12_IOCR_BIT_LSB_IDX);

    // Set P10.1,2 IOCR to set as 0b1X000 = 0x10 (general push-pull output mode)
    P10_IOCR0.U |= 0x10 << LED_D13_IOCR_BIT_LSB_IDX;
    P10_IOCR0.U |= 0x10 << LED_D12_IOCR_BIT_LSB_IDX;
}

void initRGBLED(void) {
    // Reset P2.7, P10.5, P10.3
    P02_IOCR4.U &= ~(0x1Fu << RGB_D09_IOCR_BIT_LSB_IDX);
    P10_IOCR0.U &= ~(0x1Fu << RGB_D10_IOCR_BIT_LSB_IDX);
    P10_IOCR4.U &= ~(0x1Fu << RGB_D11_IOCR_BIT_LSB_IDX);

    // Set P2.7, P10.5, P10.3 IOCR to set as 0b1X000 = 0x10 (general push-pull output mode)
    P02_IOCR4.U |= 0x10 << RGB_D09_IOCR_BIT_LSB_IDX;
    P10_IOCR0.U |= 0x10 << RGB_D10_IOCR_BIT_LSB_IDX;
    P10_IOCR4.U |= 0x10 << RGB_D11_IOCR_BIT_LSB_IDX;
}
