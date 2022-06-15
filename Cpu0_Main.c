/**********************************************************************************************************************
 * \file Cpu0_Main.c
 * \copyright Copyright (C) Infineon Technologies AG 2019
 * 
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of 
 * business you are acting and (ii) Infineon Technologies AG or its licensees. If and as long as no such terms of use
 * are agreed, use of this file is subject to following:
 * 
 * Boost Software License - Version 1.0 - August 17th, 2003
 * 
 * Permission is hereby granted, free of charge, to any person or organization obtaining a copy of the software and 
 * accompanying documentation covered by this license (the "Software") to use, reproduce, display, distribute, execute,
 * and transmit the Software, and to prepare derivative works of the Software, and to permit third-parties to whom the
 * Software is furnished to do so, all subject to the following:
 * 
 * The copyright notices in the Software and this entire statement, including the above license grant, this restriction
 * and the following disclaimer, must be included in all copies of the Software, in whole or in part, and all 
 * derivative works of the Software, unless such copies or derivative works are solely in the form of 
 * machine-executable object code generated by a source language processor.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE 
 * COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN 
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
 * IN THE SOFTWARE.
 *********************************************************************************************************************/
#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"

#include "IfxCcu6_reg.h"
#include "IfxVadc_reg.h"
#include "IfxGtm_reg.h"

// Port registers
#define PC1_BIT_LSB_IDX         11
#define PC2_BIT_LSB_IDX         19
#define P1_BIT_LSB_IDX          1
#define P2_BIT_LSB_IDX          2

#define PC7_BIT_LSB_IDX         27
#define PC5_BIT_LSB_IDX         11
#define PC3_BIT_LSB_IDX         27
#define P3_BIT_LSB_IDX          3
#define P5_BIT_LSB_IDX          5
#define P7_BIT_LSB_IDX          7

#define PC6_BIT_LSB_IDX         19
#define P6_BIT_LSB_IDX          6
#define PC4_BIT_LSB_IDX         3
#define P4_BIT_LSB_IDX          4

// SCU registers
#define LCK_BIT_LSB_IDX         1
#define ENDINIT_BIT_LSB_IDX     0

#define EXIS0_BIT_LSB_IDX       4
#define FEN0_BIT_LSB_IDX        8
#define REN0_BIT_LSB_IDX        9
#define EIEN0_BIT_LSB_IDX       11
#define INP0_BIT_LSB_IDX        12
#define IGP0_BIT_LSB_IDX        14

#define EXIS1_BIT_LSB_IDX       20
#define FEN1_BIT_LSB_IDX        24
#define REN1_BIT_LSB_IDX        25
#define EIEN1_BIT_LSB_IDX       27
#define INP1_BIT_LSB_IDX        28
#define IGP1_BIT_LSB_IDX        30

// SRC registers
#define SRPN_BIT_LSB_IDX        0
#define SRE_BIT_LSB_IDX         10
#define TOS_BIT_LSB_IDX         11

// CCU60 registers
#define DISS_BIT_LSB_IDX        1
#define DISR_BIT_LSB_IDX        0
#define T12PRE_BIT_LSB_IDX      3
#define T12CLK_BIT_LSB_IDX      0
#define CTM_BIT_LSB_IDX         7
#define T12STR_BIT_LSB_IDX      6
#define T12RS_BIT_LSB_IDX       1
#define ENT12PM_BIT_LSB_IDX     7
#define INPT12_BIT_LSB_IDX      10

// VADC registers
#define PRIO0_BIT_LSB_IDX       0
#define CSM0_BIT_LSB_IDX        3
#define ASEN0_BIT_LSB_IDX       24
#define ENGT_BIT_LSB_IDX        0
#define TREV_BIT_LSB_IDX        9
#define FLUSH_BIT_LSB_IDX       10
#define ANONC_BIT_LSB_IDX       0
#define CMS_BIT_LSB_IDX         8
#define RESPOS_BIT_LSB_IDX      21
#define RESREG_BIT_LSB_IDX      16
#define ICLSEL_BIT_LSB_IDX      0
#define ASSCH7_BIT_LSB_IDX      7
#define VF_BIT_LSB_IDX          31
#define RESULT_BIT_LSB_IDX      0

// GTM registers
#define FXCLK_SEL_BIT_LSB_IDX   0
#define EN_FXCLK_BIT_LSB_IDX    22
#define SEL7_BIT_LSB_IDX        14
#define UPEN_CTRL1_LSB_IDX      18
#define HOST_TRIG_LSB_IDX       0
#define ENDIS_CTRL1_LSB_IDX     2
#define OUTEN_CTRL1_LSB_IDX     2
#define SL_BIT_LSB_IDX          11
#define CLK_SRC_SR_LSB_IDX      12
#define OSM_BIT_LSB_IDX         26
#define TRIGOUT_BIT_LSB_IDX     24

// GTM registers for Buzzer
#define SEL3_BIT_LSB_IDX        6
#define UPEN_CTRL3_LSB_IDX      22
#define ENDIS_CTRL3_LSB_IDX     6
#define OUTEN_CTRL3_LSB_IDX     6

/* Ultra-sonic sensor pre-processing */
#define US_THRESHOLD    2000U
#define US_MIN          2U
#define US_MAX          4000U
#define WINDOW_SIZE     3U

// Song
#define c4 3822 // 261.63Hz 도
#define d4 3405 // 293.67Hz 레
#define e4 3034 // 329.63Hz 미
#define f4 2863 // 349.23Hz 파
#define g4 2551 // 392.11Hz 솔
#define g4s 2408 // 415.30Hz 솔#
#define a4 2273 // 440.11Hz 라
#define b4 2025 // 493.88Hz 시
#define c5 1910 // 523.25Hz 도
#define d5 1703 // 587.33Hz 레

#define d5s 1607 // 584.37Hz 레#
#define e5 1517 // 659.26Hz 미
#define f5 1432 // 698.46Hz 파
#define g5 1276 // 783.99Hz 솔
#define a5 1136 // 880.11Hz 라
#define b5 1012 // 987.77Hz 시

int song[] = { e5, d5s, e5, d5s, e5, b4, d5, c5, a4,a4, c4, e4, a4, b4,b4, e4, g4s, b4, c5,c5, e4, e5, d5s, e5, d5s, e5, b4, d5, c5, a4,a4, c4, e4, a4, b4,b4, e4, c5, b4, a4, a4 };


IfxCpu_syncEvent g_cpuSyncEvent = 0;

void initLED(void);
void initButton(void);
void initERU0(void);
void initERU2(void);
void initERU6(void);
void initCCU60(void);
void initRGBLED(void);
void initVADC(void);
void VADC_startConversion(void);
unsigned int VADC_readResult(void);
void initGTM(void);
void initUSonic(void);
void initCCU61(void);
void usonicTrigger(void);
void initBuzzer(void);
void initGTM_Buzzer(void);

unsigned int range;
unsigned char range_valid_flag;

unsigned char button_pushed_flag;

__interrupt(0x0A) __vector_table(0)
void ERU6_ISR(void)
{
    static unsigned int prev_range[5] = {0, };
    static unsigned char prev_idx = 0;

    //if( P00_IN.B.P4 != 0 )
    if( P11_IN.B.P10 != 0 )
    {
        CCU61_TCTR4.B.T12RS = 0x1; // set Request CCU61 T12 Counter
    }
    else {
        CCU61_TCTR4.B.T12RR = 0x1;  // reset Request CCU61 T12 Counter

        unsigned int temp_range = ( ( CCU61_T12.B.T12CV * 1000000 ) / 48828 ) / 58;

        if( temp_range < US_MIN )
            temp_range = US_MIN;
        else if(temp_range > US_MAX )
            temp_range = US_MAX;
        else if( temp_range > US_THRESHOLD )
            temp_range = US_THRESHOLD;
        else {}


        if( prev_idx >= WINDOW_SIZE )
            prev_idx = 0;
        else
            prev_idx++;

        prev_range[prev_idx] = temp_range;

        unsigned int temp_sum = 0;

        for(int i = 0 ; i < WINDOW_SIZE ; i++) {
            if ( prev_range[i] == 0 )
                temp_sum += temp_range;
            else
                temp_sum += prev_range[i];
        }

        range = temp_sum / WINDOW_SIZE;

        range_valid_flag = 1;

        CCU61_TCTR4.B.T12RES = 0x1; // reset CCU61 T12 Counter
    }

    if (button_pushed_flag)
    {
        if (range > 2 && range <10){ // R
            P02_OUT.U |= 0x1 << P7_BIT_LSB_IDX;
            P10_OUT.U &= ~(0x1 << P5_BIT_LSB_IDX);
            P10_OUT.U &= ~(0x1 << P3_BIT_LSB_IDX);
        }
        else if (range >= 10 && range <20){ //Y
            P02_OUT.U |= 0x1 << P7_BIT_LSB_IDX;
            P10_OUT.U |= 0x1 << P5_BIT_LSB_IDX;
            P10_OUT.U &= ~(0x1 << P3_BIT_LSB_IDX);
        }
        else { //G
            P02_OUT.U &= ~(0x1 << P7_BIT_LSB_IDX);
            P10_OUT.U |= 0x1 << P5_BIT_LSB_IDX;
            P10_OUT.U &= ~(0x1 << P3_BIT_LSB_IDX);
        }
    }
    else
    {
        /*---RGB LED Off---*/
        P02_OUT.U &= ~(0x1 << P7_BIT_LSB_IDX);
        P10_OUT.U &= ~(0x1 << P5_BIT_LSB_IDX);
        P10_OUT.U &= ~(0x1 << P3_BIT_LSB_IDX);
    }

    /*
    // USonic Range

    if( (P00_IN.U & (0x1 << P4_BIT_LSB_IDX)) != 0 ) // rising edge of echo
    {
        //                   _______
        // echo     ________|
        //                  ^

        CCU61_TCTR4.U |= 0x1 << T12RS_BIT_LSB_IDX;
    }
    else // falling edge of echo
    {
        CCU61_TCTR4.B.T12RR = 0x1; //stop CCU12 T12 counter

        // (1/t_freq) * counter * 1000000 / 58 = centimeter
        range = ((CCU61_T12.B.T12CV * 1000000 / 48828)) / 58;
        range_valid_flag = 1;

        CCU61_TCTR4.B.T12RES = 0x1;
    }
    */
}

__interrupt(0x0B) __vector_table(0)
void CCU60_T12_ISR(void)
{
    // End of 10us Trigger
    // GPIO P02.6 --> Low
    P02_OUT.U &= ~(0x1 << P6_BIT_LSB_IDX);
}

__interrupt(0x09) __vector_table(0)
void ERU0_ISR(void)
{
    // SW2 interrupt
    P10_OUT.U ^= 0x1 << P2_BIT_LSB_IDX;
}

__interrupt(0x10) __vector_table(0)
void ERU2_ISR(void)
{
    // SW1 interrupt
    P10_OUT.U ^= 0x1 << P2_BIT_LSB_IDX;

    button_pushed_flag ^= 0x1;
}

int core0_main(void)
{
    unsigned int adcResult;

    IfxCpu_enableInterrupts();
    
    /* !!WATCHDOG0 AND SAFETY WATCHDOG ARE DISABLED HERE!!
     * Enable the watchdogs and service them periodically if it is required
     */
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());
    
    /* Wait for CPU sync event */
    IfxCpu_emitEvent(&g_cpuSyncEvent);
    IfxCpu_waitEvent(&g_cpuSyncEvent, 1);
    
    /* Initialization */
    initERU0();
    initERU2();
    initERU6();
    initCCU60();
    initLED();
    initRGBLED();
    initVADC();
    initGTM();
    initButton();
    initUSonic();
    initCCU61();
    initBuzzer();
    initGTM_Buzzer();

    GTM_TOM0_TGC0_GLB_CTRL.U |= 0x1 << HOST_TRIG_LSB_IDX;   // Red LED

    GTM_TOM0_TGC1_GLB_CTRL.U |= 0x1 << HOST_TRIG_LSB_IDX;   // Buzzer

    while(1)
    {
        X:
           if(button_pushed_flag == 1)
           {
               // sonic sensor on
               for(unsigned int i = 0; i<10000000; i++);
               usonicTrigger();

               int x = 0;

               // Play sound
               //for (int x = 0 ; song[x-1] ; x++)
               while(song[x/3])
               {
                   x++;

                   GTM_TOM0_CH11_SR0.U = song[x/3]; // Buzzer Period

                   for (int y = 0 ; y < (1000000 * 3); y++);

                   VADC_startConversion();
                   adcResult = VADC_readResult();

                   GTM_TOM0_CH11_SR1.U = (adcResult / 4) / ((x % 3) + 1); // Buzzer Duty
                   GTM_TOM0_CH1_SR1.U = adcResult * 4; // Red LED

                   usonicTrigger();

                   if(button_pushed_flag != 1){
                       goto X;
                   }
               }

               while(range_valid_flag == 0);
           }
           else
           {
               VADC_startConversion();
               adcResult = VADC_readResult();

               // Stop Sound and Off LED
               GTM_TOM0_CH11_SR0.U = 0; // Buzzer Period
               GTM_TOM0_CH1_SR1.U = adcResult * 4; // Red LED

               P02_OUT.U &= ~(0x1 << P7_BIT_LSB_IDX);
               P10_OUT.U &= ~(0x1 << P5_BIT_LSB_IDX);
               P10_OUT.U &= ~(0x1 << P3_BIT_LSB_IDX);
           }

        /*
        VADC_startConversion();
        adcResult = VADC_readResult();

        GTM_TOM0_CH11_SR1.U = adcResult * 4; // Buzzer Duty
        if(button_pushed_flag == 1)
        {
            for(unsigned int i = 0; i<10000000; i++);
            usonicTrigger();

            while(range_valid_flag == 0);
            if(range >= 60) // Red
            {
               P02_OUT.U |= 0x1 << P7_BIT_LSB_IDX;
               P10_OUT.U &= ~(0x1 << P5_BIT_LSB_IDX);
               P10_OUT.U &= ~(0x1 << P3_BIT_LSB_IDX);
            }
            else if(range >= 40) // Green
            {
                P02_OUT.U &= ~(0x1 << P7_BIT_LSB_IDX);
                P10_OUT.U |= 0x1 << P5_BIT_LSB_IDX;
                P10_OUT.U &= ~(0x1 << P3_BIT_LSB_IDX);
            }
            else if(range >= 20) // Blue
            {
                P02_OUT.U &= ~(0x1 << P7_BIT_LSB_IDX);
                P10_OUT.U &= ~(0x1 << P5_BIT_LSB_IDX);
                P10_OUT.U |= 0x1 << P3_BIT_LSB_IDX;
            }
            else
            {
                P02_OUT.U &= ~(0x1 << P7_BIT_LSB_IDX);
                P10_OUT.U &= ~(0x1 << P5_BIT_LSB_IDX);
                P10_OUT.U &= ~(0x1 << P3_BIT_LSB_IDX);
            }

            for(int x = 0; song[x-1] ; x++)
            {
                if(button_pushed_flag !=0)
                {
                    GTM_TOM0_CH11_SR0.U = song[x];
                    for (int i = 0; i < 10000000 ; i++);
                }
            }
        }
        else
        {
            // Stop Sound and Off LED
            GTM_TOM0_CH11_SR0.U = 0; // Buzzer Period

            P02_OUT.U &= ~(0x1 << P7_BIT_LSB_IDX);
            P10_OUT.U &= ~(0x1 << P5_BIT_LSB_IDX);
            P10_OUT.U &= ~(0x1 << P3_BIT_LSB_IDX);

            GTM_TOM0_CH1_SR1.U = adcResult * 4; // Red LED
        }
        */
    }

    return (1);
}

void initLED(void)
{
    P10_IOCR0.U &= ~(0x1F << PC1_BIT_LSB_IDX);  // Red LED
    P10_IOCR0.U &= ~(0x1F << PC2_BIT_LSB_IDX);  // Blue LED

#if 0
    P10_IOCR0.U |= 0x10 << PC1_BIT_LSB_IDX; // GPIO
#else
    P10_IOCR0.U |= 0x11 << PC1_BIT_LSB_IDX; // PWM
#endif
    P10_IOCR0.U |= 0x10 << PC2_BIT_LSB_IDX;
}

void initButton(void)
{
    P02_IOCR0.U &= ~(0x1F << PC1_BIT_LSB_IDX);
    P02_IOCR0.U |= 0x02 << PC1_BIT_LSB_IDX;
}

void initERU0(void)
{
   SCU_EICR1.U &= ~(0x7 <<EXIS0_BIT_LSB_IDX);
   SCU_EICR1.U |= (0x1 << EXIS0_BIT_LSB_IDX);  // Push button

   SCU_EICR1.U |= 0x1 << FEN0_BIT_LSB_IDX; // Falling Edge

   SCU_EICR1.U |= 0x1 << EIEN0_BIT_LSB_IDX;

   SCU_EICR1.U &= ~(0x7 << INP0_BIT_LSB_IDX); // OGU1
   SCU_EICR1.U |= 0x1 << INP0_BIT_LSB_IDX;

   SCU_IGCR0.U &= ~(0x3 << IGP1_BIT_LSB_IDX);
   SCU_IGCR0.U |= 0x1 << IGP1_BIT_LSB_IDX;

   SRC_SCU_SCU_ERU1.U &= ~(0xFF << SRPN_BIT_LSB_IDX);
   SRC_SCU_SCU_ERU1.U |= 0x08 << SRPN_BIT_LSB_IDX; // SRPN : 0x8

   SRC_SCU_SCU_ERU1.U &= ~(0x3 << TOS_BIT_LSB_IDX);

   SRC_SCU_SCU_ERU1.U |= 0x1 << SRE_BIT_LSB_IDX;
}

void initERU2(void)
{
    SCU_EICR1.U &= ~(0x7 <<EXIS1_BIT_LSB_IDX);
    SCU_EICR1.U |= (0x2 << EXIS1_BIT_LSB_IDX);

    SCU_EICR1.U |= 0x1 << FEN1_BIT_LSB_IDX; // Falling Edge

    SCU_EICR1.U |= 0x1 << EIEN1_BIT_LSB_IDX;

    SCU_EICR1.U &= ~(0x7 << INP1_BIT_LSB_IDX);
    SCU_EICR1.U |= (0x2 << INP1_BIT_LSB_IDX);

    SCU_IGCR1.U &= ~(0x3 << IGP0_BIT_LSB_IDX);
    SCU_IGCR1.U |= 0x1 << IGP0_BIT_LSB_IDX;

    SRC_SCU_SCU_ERU2.U &= ~(0xFF << SRPN_BIT_LSB_IDX);
    SRC_SCU_SCU_ERU2.U |= 0x10 << SRPN_BIT_LSB_IDX;

    SRC_SCU_SCU_ERU2.U &= ~(0x3 << TOS_BIT_LSB_IDX);

    SRC_SCU_SCU_ERU2.U |= 0x1 << SRE_BIT_LSB_IDX;
}

void initERU6(void)
{
    SCU_EICR3.U &= ~(0x7 <<EXIS0_BIT_LSB_IDX);

    SCU_EICR3.U |= (0x3 << EXIS0_BIT_LSB_IDX);  // ERS6 - Input 3 USonic

    SCU_EICR3.U |= 0x1 << FEN0_BIT_LSB_IDX; // Falling Edge
    SCU_EICR3.U |= 0x1 << REN0_BIT_LSB_IDX; // Rising Edge
    SCU_EICR3.U |= 0x1 << EIEN0_BIT_LSB_IDX;

    SCU_EICR3.U &= ~(0x7 << INP0_BIT_LSB_IDX); // OGU0

    SCU_IGCR0.U &= ~(0x3 << IGP0_BIT_LSB_IDX);
    SCU_IGCR0.U |= 0x1 << IGP0_BIT_LSB_IDX;

    SRC_SCU_SCU_ERU0.U &= ~(0xFF << SRPN_BIT_LSB_IDX);
    SRC_SCU_SCU_ERU0.U |= 0x0A << SRPN_BIT_LSB_IDX; // SRPN : 0xA

    SRC_SCU_SCU_ERU0.U &= ~(0x3 << TOS_BIT_LSB_IDX);

    SRC_SCU_SCU_ERU0.U |= 0x1 << SRE_BIT_LSB_IDX;
}

void initCCU60(void)
{
    SCU_WDTCPU0_CON0.U  = ((SCU_WDTCPU0_CON0.U ^ 0xFC) & ~(1 << LCK_BIT_LSB_IDX)) | (1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1 << LCK_BIT_LSB_IDX)) != 0);

    SCU_WDTCPU0_CON0.U  = ((SCU_WDTCPU0_CON0.U ^ 0xFC) | (1 << LCK_BIT_LSB_IDX)) & ~(1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1 << LCK_BIT_LSB_IDX)) == 0);

    CCU60_CLC.U &= ~(1 << DISR_BIT_LSB_IDX);

    SCU_WDTCPU0_CON0.U  = ((SCU_WDTCPU0_CON0.U ^ 0xFC) & ~(1 << LCK_BIT_LSB_IDX)) | (1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1 << LCK_BIT_LSB_IDX)) != 0);

    SCU_WDTCPU0_CON0.U  = ((SCU_WDTCPU0_CON0.U ^ 0xFC) | (1 << LCK_BIT_LSB_IDX)) | (1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1 << LCK_BIT_LSB_IDX)) == 0);

    while((CCU60_CLC.U & (1 << DISS_BIT_LSB_IDX)) != 0);

    CCU60_TCTR0.U &= ~(0x7 << T12CLK_BIT_LSB_IDX);
    CCU60_TCTR0.U |= (0x2 << T12CLK_BIT_LSB_IDX);
#if 0
    CCU60_TCTR0.U |= (0x1 << T12PRE_BIT_LSB_IDX); // 12.5MHz / 256
#endif

    CCU60_TCTR0.U &= ~(0x1 << CTM_BIT_LSB_IDX);

#if 0
    CCU60_T12PR.U = 24414 - 1; //0.5s
#else
    CCU60_T12PR.U = 125 - 1; // 10us, 1/12.5 = 0.08u
#endif

    CCU60_TCTR4.U |= 0x1 << T12STR_BIT_LSB_IDX;

    CCU60_TCTR2.U |= 0x1; // Single Shot -> only one pulse, not continuous

    CCU60_T12.U = 0;

    CCU60_INP.U &= ~(0x3 << INPT12_BIT_LSB_IDX);
    CCU60_IEN.U |= 0x1 << ENT12PM_BIT_LSB_IDX;

    SRC_CCU6_CCU60_SR0.U &= ~(0xFF << SRPN_BIT_LSB_IDX);
    SRC_CCU6_CCU60_SR0.U |= 0x0B << SRPN_BIT_LSB_IDX;

    SRC_CCU6_CCU60_SR0.U |= 0x1 << SRE_BIT_LSB_IDX;

    SRC_CCU6_CCU60_SR0.U &= ~(0x3 << TOS_BIT_LSB_IDX);

#if 0    // call by usonicTrigger function
    CCU60_TCTR4.U |= 0x1 << T12RS_BIT_LSB_IDX; // T12 start counting
#endif
}

void initCCU61(void)
{
    SCU_WDTCPU0_CON0.U  = ((SCU_WDTCPU0_CON0.U ^ 0xFC) & ~(1 << LCK_BIT_LSB_IDX)) | (1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1 << LCK_BIT_LSB_IDX)) != 0);

    SCU_WDTCPU0_CON0.U  = ((SCU_WDTCPU0_CON0.U ^ 0xFC) | (1 << LCK_BIT_LSB_IDX)) & ~(1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1 << LCK_BIT_LSB_IDX)) == 0);

    CCU61_CLC.U &= ~(1 << DISR_BIT_LSB_IDX);

    SCU_WDTCPU0_CON0.U  = ((SCU_WDTCPU0_CON0.U ^ 0xFC) & ~(1 << LCK_BIT_LSB_IDX)) | (1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1 << LCK_BIT_LSB_IDX)) != 0);

    SCU_WDTCPU0_CON0.U  = ((SCU_WDTCPU0_CON0.U ^ 0xFC) | (1 << LCK_BIT_LSB_IDX)) | (1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1 << LCK_BIT_LSB_IDX)) == 0);

    while((CCU61_CLC.U & (1 << DISS_BIT_LSB_IDX)) != 0);

    CCU61_TCTR0.U &= ~(0x7 << T12CLK_BIT_LSB_IDX);
    CCU61_TCTR0.U |= (0x2 << T12CLK_BIT_LSB_IDX);

    CCU61_TCTR0.U |= (0x1 << T12PRE_BIT_LSB_IDX); // 12.5MHz / 256

    CCU61_TCTR0.U &= ~(0x1 << CTM_BIT_LSB_IDX);

    CCU61_T12PR.U = 100000 - 1;

    CCU61_TCTR4.U |= 0x1 << T12STR_BIT_LSB_IDX;

    CCU61_T12.U = 0;
}

void initRGBLED(void)
{
    P02_IOCR4.U &= ~(0x1F << PC7_BIT_LSB_IDX);
    P10_IOCR4.U &= ~(0x1F << PC5_BIT_LSB_IDX);
    P10_IOCR0.U &= ~(0x1F << PC3_BIT_LSB_IDX);

    P02_IOCR4.U |= 0x10 << PC7_BIT_LSB_IDX;
    P10_IOCR4.U |= 0x10 << PC5_BIT_LSB_IDX;
    P10_IOCR0.U |= 0x10 << PC3_BIT_LSB_IDX;
}

void initVADC(void)
{
    SCU_WDTCPU0_CON0.U  = ((SCU_WDTCPU0_CON0.U ^ 0xFC) & ~(1 << LCK_BIT_LSB_IDX)) | (1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1 << LCK_BIT_LSB_IDX)) != 0);

    SCU_WDTCPU0_CON0.U  = ((SCU_WDTCPU0_CON0.U ^ 0xFC) | (1 << LCK_BIT_LSB_IDX)) & ~(1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1 << LCK_BIT_LSB_IDX)) == 0);

    VADC_CLC.U &= ~(0x1 << DISR_BIT_LSB_IDX);

    SCU_WDTCPU0_CON0.U  = ((SCU_WDTCPU0_CON0.U ^ 0xFC) & ~(1 << LCK_BIT_LSB_IDX)) | (1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1 << LCK_BIT_LSB_IDX)) != 0);

    SCU_WDTCPU0_CON0.U  = ((SCU_WDTCPU0_CON0.U ^ 0xFC) | (1 << LCK_BIT_LSB_IDX)) | (1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1 << LCK_BIT_LSB_IDX)) == 0);

    while((VADC_CLC.U & (1 << DISS_BIT_LSB_IDX)) != 0);

    VADC_G4_ARBPR.U |= 0x3 << PRIO0_BIT_LSB_IDX;
    VADC_G4_ARBPR.U &= ~(0x1 << CSM0_BIT_LSB_IDX);
    VADC_G4_ARBPR.U |= 0x1 << ASEN0_BIT_LSB_IDX;

    VADC_G4_QMR0.U &= ~(0x3 << ENGT_BIT_LSB_IDX);
    VADC_G4_QMR0.U |= (0x1 << ENGT_BIT_LSB_IDX);
    VADC_G4_QMR0.U |= (0x1 << FLUSH_BIT_LSB_IDX);

    VADC_G4_ARBCFG.U |= 0x3 << ANONC_BIT_LSB_IDX;

    VADC_G4_ICLASS0.U &= ~(0x7 << CMS_BIT_LSB_IDX);

    VADC_G4_CHCTR7.U |= 0x1 << RESPOS_BIT_LSB_IDX;
    VADC_G4_CHCTR7.U &= ~(0xF << RESREG_BIT_LSB_IDX);
    VADC_G4_CHCTR7.U &= ~(0x3 << ICLSEL_BIT_LSB_IDX);

    VADC_G4_CHASS.U |= 0x1 << ASSCH7_BIT_LSB_IDX;
}

void VADC_startConversion(void)
{
    VADC_G4_QINR0.U |= 0x7;

    VADC_G4_QMR0.U |= (0x1 << TREV_BIT_LSB_IDX);
}

unsigned int VADC_readResult(void)
{
    unsigned int result;

    while( (VADC_G4_RES0.U & (0x1 << VF_BIT_LSB_IDX)) == 0);
    result = VADC_G4_RES0.U  & (0xFFF << RESULT_BIT_LSB_IDX);

    return result;
}

void initGTM(void)
{
    SCU_WDTCPU0_CON0.U  = ((SCU_WDTCPU0_CON0.U ^ 0xFC) & ~(1 << LCK_BIT_LSB_IDX)) | (1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1 << LCK_BIT_LSB_IDX)) != 0);

    SCU_WDTCPU0_CON0.U  = ((SCU_WDTCPU0_CON0.U ^ 0xFC) | (1 << LCK_BIT_LSB_IDX)) & ~(1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1 << LCK_BIT_LSB_IDX)) == 0);

    GTM_CLC.U &= ~(1 << DISR_BIT_LSB_IDX);

    SCU_WDTCPU0_CON0.U  = ((SCU_WDTCPU0_CON0.U ^ 0xFC) & ~(1 << LCK_BIT_LSB_IDX)) | (1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1 << LCK_BIT_LSB_IDX)) != 0);

    SCU_WDTCPU0_CON0.U  = ((SCU_WDTCPU0_CON0.U ^ 0xFC) | (1 << LCK_BIT_LSB_IDX)) | (1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1 << LCK_BIT_LSB_IDX)) == 0);

    while((GTM_CLC.U & (1 << DISS_BIT_LSB_IDX)) != 0);

    GTM_CMU_FXCLK_CTRL.U &= ~(0xF << FXCLK_SEL_BIT_LSB_IDX);
    GTM_CMU_CLK_EN.U |= 0x2 << EN_FXCLK_BIT_LSB_IDX;

    GTM_TOM0_TGC0_GLB_CTRL.U |= 0x2 << UPEN_CTRL1_LSB_IDX;

    GTM_TOM0_TGC0_ENDIS_CTRL.U |= 0x2 << ENDIS_CTRL1_LSB_IDX;

    GTM_TOM0_TGC0_OUTEN_CTRL.U |= 0x2 << OUTEN_CTRL1_LSB_IDX;

    GTM_TOM0_CH1_CTRL.U |= 0x1 << SL_BIT_LSB_IDX;
    GTM_TOM0_CH1_CTRL.U |= 0x1 << CLK_SRC_SR_LSB_IDX;   // clock source 100M/16 = 6250kHz
    GTM_TOM0_CH1_CTRL.U &= ~(0x1 << OSM_BIT_LSB_IDX);   // continuous mode
    GTM_TOM0_CH1_CTRL.U &= ~(0x1 << TRIGOUT_BIT_LSB_IDX); // TRIG[x] = TRIG[x-1]

    GTM_TOM0_CH1_SR0.U = 12500 - 1; // period = 2ms
    GTM_TOM0_CH1_SR1.U = 1250 - 1; // duty cycle = 10%

    GTM_TOUTSEL6.U &= ~(0x3 << SEL7_BIT_LSB_IDX);
}

void initUSonic(void)
{
    P02_IOCR4.U &= ~(0x1F << PC6_BIT_LSB_IDX);
    P00_IOCR4.U &= ~(0x1F << PC4_BIT_LSB_IDX);
    P11_IOCR8.B.PC10 &= 0x0;

    P00_IOCR4.U |= 0x01 << PC4_BIT_LSB_IDX; // Input
    P02_IOCR4.U |= 0x10 << PC6_BIT_LSB_IDX; // Output
    P11_IOCR8.B.PC10 |= 0x01;

    P02_OUT.U &= ~(0x1 << P6_BIT_LSB_IDX);
}

void usonicTrigger(void)
{
    // start of 10us Trigger pulse
    P02_OUT.U |= 0x1 << P6_BIT_LSB_IDX;

    range_valid_flag = 0;

    CCU60_TCTR4.U |= 0x1 << T12RS_BIT_LSB_IDX; // T12 start counting
}

void initGTM_Buzzer(void)
{
    SCU_WDTCPU0_CON0.U  = ((SCU_WDTCPU0_CON0.U ^ 0xFC) & ~(1 << LCK_BIT_LSB_IDX)) | (1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1 << LCK_BIT_LSB_IDX)) != 0);

    SCU_WDTCPU0_CON0.U  = ((SCU_WDTCPU0_CON0.U ^ 0xFC) | (1 << LCK_BIT_LSB_IDX)) & ~(1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1 << LCK_BIT_LSB_IDX)) == 0);

    GTM_CLC.U &= ~(1 << DISR_BIT_LSB_IDX);

    SCU_WDTCPU0_CON0.U  = ((SCU_WDTCPU0_CON0.U ^ 0xFC) & ~(1 << LCK_BIT_LSB_IDX)) | (1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1 << LCK_BIT_LSB_IDX)) != 0);

    SCU_WDTCPU0_CON0.U  = ((SCU_WDTCPU0_CON0.U ^ 0xFC) | (1 << LCK_BIT_LSB_IDX)) | (1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1 << LCK_BIT_LSB_IDX)) == 0);

    while((GTM_CLC.U & (1 << DISS_BIT_LSB_IDX)) != 0);

    GTM_CMU_FXCLK_CTRL.U &= ~(0xF << FXCLK_SEL_BIT_LSB_IDX);
    GTM_CMU_CLK_EN.U |= 0x2 << EN_FXCLK_BIT_LSB_IDX;

    GTM_TOM0_TGC1_GLB_CTRL.U |= 0x2 << UPEN_CTRL3_LSB_IDX;  // TOM0_CH11 -> TGC1, UPEN->CH3

    GTM_TOM0_TGC1_ENDIS_CTRL.U |= 0x2 << ENDIS_CTRL3_LSB_IDX;

    GTM_TOM0_TGC1_OUTEN_CTRL.U |= 0x2 << OUTEN_CTRL3_LSB_IDX;

    GTM_TOM0_CH11_CTRL.U |= 0x1 << SL_BIT_LSB_IDX;  // TOM0_11
    GTM_TOM0_CH11_CTRL.U |= 0x1 << CLK_SRC_SR_LSB_IDX;   // clock source 100M/16 = 6250kHz
    GTM_TOM0_CH11_CTRL.U &= ~(0x1 << OSM_BIT_LSB_IDX);   // continuous mode
    GTM_TOM0_CH11_CTRL.U &= ~(0x1 << TRIGOUT_BIT_LSB_IDX); // TRIG[x] = TRIG[x-1]

    GTM_TOM0_CH11_SR0.U = 12500 - 1; // period = 2ms
    GTM_TOM0_CH11_SR1.U = 1250 - 1; // duty cycle = 10%

    GTM_TOUTSEL0.U &= ~(0x3 << SEL3_BIT_LSB_IDX);   // Output: TOUT3, TOUTSEL0 & SEL3
}

void initBuzzer(void)
{
    P02_IOCR0.U &= ~(0x1F << PC3_BIT_LSB_IDX);

    P02_IOCR0.U |= 0x11 << PC3_BIT_LSB_IDX;
}
