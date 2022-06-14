/*
 * Button_Control.c
 *
 *  Created on: 2022. 6. 7.
 *      Author: user
 */

#include "Button_Control.h"

void initButton(void) {
    // Reset P2.0,1 IOCR
    P02_IOCR0.U &= ~(0x1F << BUTTON_D2_IOCR_BIT_LSB_IDX);
    P02_IOCR0.U &= ~(0x1F << BUTTON_D3_IOCR_BIT_LSB_IDX);

    // Set P2.0,1 IOCR to set as 0b0XX10 = 0x02 (general pull-up input mode)
    P02_IOCR0.U &= 0x02 << BUTTON_D2_IOCR_BIT_LSB_IDX;
    P02_IOCR0.U &= 0x02 << BUTTON_D3_IOCR_BIT_LSB_IDX;
}



