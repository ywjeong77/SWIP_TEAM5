/*
 * PWM.h
 *
 *  Created on: 2022. 6. 13.
 *      Author: user
 */

#ifndef SOURCE_PWM_H_
#define SOURCE_PWM_H_

#include "IfxGtm_reg.h"
#include "IfxGtm_regdef.h"

#include "IfxScu_reg.h"
#include "IfxScu_regdef.h"

#include "WDTCON0_def.h"

/* GTM_CMU Register */
// CMU_FXCLK_CTRL Register
#define FXCLK_SEL_BIT_LSB_IDX           0U
#define FXCLK_SEL_MASK                  ( 0xF << FXCLK_SEL_BIT_LSB_IDX )

// CMU_CLK_EN Register
#define EN_FXCLK_BIT_LSB_IDX            22U
#define EN_FXCLK_MASK                   ( 0x3 << EN_FXCLK_BIT_LSB_IDX )

/* GTM_TOM Register */
// TOUTSELn Register
#define TOUTSEL_SEL_BIT_LSB_IDX(N)      ( 2U * ( N ) )
#define TOUTSEL_SEL_MASK(N)             ( 0x3U << ( TOUTSEL_SEL_BIT_LSB_IDX(N) ) )

// TOMi_TGCn_GBL_CTRL (i = 0-2, n = 0-1)
#define UPEN_CTRL_BIT_LSB_IDX(N)        ( 2U * ( N ) + 16 )
#define UPEN_CTRL_MASK(N)               ( 0x3U << ( UPEN_CTRL_BIT_LSB_IDX(N) ) )
#define HOST_TRIG_BIT_LSB_IDX           0U
#define HOST_TRIG_MASK                  ( 0x1U << HOST_TRIG_BIT_LSB_IDX )

// TOMi_TGCn_ENDIS_CTRL
#define ENDIS_CTRL_BIT_LSB_IDX(N)       ( 2U * ( N ) )
#define ENDIS_CTRL_MASK(N)              ( 0x3U << ( ENDIS_CTRL_BIT_LSB_IDX(N) ) )

// TOMi_TGCn_OUTEN_CTRL
#define OUTEN_CTRL_BIT_LSB_IDX(N)       ( 2U * ( N ) )
#define OUTEN_CTRL_MASK(N)              ( 0x3U << ( OUTEN_CTRL_BIT_LSB_IDX(N) ) )

// TOMi_CHx_CTRL
#define SL_BIT_LSB_IDX                  11U
#define SL_MAKS                         ( 0x1U << SL_BIT_LSB_IDX )

#define CLK_SRC_SR_BIT_LSB_IDX          12U
#define CLK_SRC_SR_MASK                 ( 0x7U << CLK_SRC_SR_BIT_LSB_IDX )

#define OSM_BIT_LSB_IDX                 26U
#define OSM_MASK                        ( 0x1U << OSM_BIT_LSB_IDX )

#define TRIG_OUT_BIT_LSB_IDX            24U
#define TRIG_OUT_MASK                   ( 0x1U << TRIG_OUT_BIT_LSB_IDX )

// TOMi_CHx_SR0
#define SR0_BIT_LSB_IDX                 0U
#define SR0_MASK                        ( 0xFFU << SR0_BIT_LSB_IDX )

// TOMi_CHx_SR1
#define SR1_BIT_LSB_IDX                 0U
#define SR1_MASK                        ( 0xFFU << SR1_BIT_LSB_IDX )

/* PWM Configuration */
#define CMU_CLK_FREQ                    10000000U                        // 100MHz (kHz)
#define CMU_PERIOD_CNT                  12500U                          // Period : 2ms
#define CMU_DUTY_CNT                    1250U                           // Duty rate : 0.5ms

/* User-defined enum */
typedef enum {
    PORT_10_1 = 0x1,    // TOUT103
    PORT_10_2,          // TOUT104
    PORT_2_7,           // TOUT7
    PORT_10_5,          // TOUT107
    PORT_10_3,          // TOUT105
    PORT_2_3,           // Buzzer, TOUT3
} pwm_port_t;

/* Initialization */
void initGTM(pwm_port_t sel);
void initLED1_PWM(void);
void initLED2_PWM(void);
void initRGBLED_PWM(void);
void initBuzzer_PWM(void);

/* Trigger TOMn to start/stop PWM signal */
void startPWM(pwm_port_t sel);
void controlPWM(pwm_port_t sel, int enable);

/* Modulation PWM for each port */
void changePWM(pwm_port_t sel, unsigned short period, unsigned short duty);

#endif /* SOURCE_PWM_H_ */
