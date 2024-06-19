#include <stdbool.h>
#include <stm8s.h>
#include <stdio.h>
#include "main.h"
#include "milis.h"
#include "delay.h"
#include "daughterboard.h"
#include "uart1.h"
#include "uart1.h"

void init(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);      // taktovani MCU na 16MHz

    GPIO_Init(PWM_R_PORT, PWM_R_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(PWM_G_PORT, PWM_G_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(PWM_B_PORT, PWM_B_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    
    GPIO_Init(LED1_PORT, LED1_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(LED2_PORT, LED2_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(LED4_PORT, LED4_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);

    GPIO_Init(S1_PORT, S1_PIN, GPIO_MODE_IN_PU_NO_IT);
    GPIO_Init(S2_PORT, S2_PIN, GPIO_MODE_IN_PU_NO_IT);
    GPIO_Init(S3_PORT, S3_PIN, GPIO_MODE_IN_PU_NO_IT);
    //GPIO_Init(LED_PORT, LED_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
#if defined (BTN_PORT) || defined (BTN_PIN)
    GPIO_Init(BTN_PORT, BTN_PIN, GPIO_MODE_IN_FL_NO_IT);
#endif

    init_milis();
    init_uart1();

    TIM2_DeInit();
    TIM2_TimeBaseInit(TIM2_PRESCALER_16, 100);
    TIM2_OC1Init(
        TIM2_OCMODE_PWM1, 
        TIM2_OUTPUTSTATE_ENABLE,
        5000,
        TIM2_OCPOLARITY_HIGH
    );
    TIM2_OC2Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, 1000, TIM2_OCPOLARITY_HIGH);
    TIM2_OC3Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, 9000, TIM2_OCPOLARITY_HIGH);
    
    TIM2_OC1PreloadConfig(ENABLE);
    TIM2_OC2PreloadConfig(ENABLE);
    TIM2_OC3PreloadConfig(ENABLE);

    TIM2_Cmd(ENABLE);
}




int main(void)
{
  
    uint32_t time = 0;
    uint8_t r=0, g=0, b=0;
    bool S1memory = false;
    bool S2memory = false;
    bool S3memory = false;
    uint8_t color = 1;

    init();

    while (1) {

        if (milis() - time > 75 ) {
            time = milis();

            if (PUSH(S3) && S3memory){
         
                if (color == 1){
                    r -=5;
                }
                
                if (color == 2){
                    g -=5;
                }
                
                if(color == 3){
                    b -=5;
                }


            }
            S3memory = PUSH(S3);

            if (PUSH(S2) && S2memory){
                
                if (color == 1){
                    r +=5;
                }
                
                if (color == 2){
                    g +=5;
                }
                
                if(color == 3){
                    b +=5;
                }

                
            }
            S2memory = PUSH(S2);

            if (PUSH(S1) && S1memory){
                color++;
                if (color > 3){
                    color = 1;
                }
                LOW(LED1);
                LOW(LED4);
                LOW(LED2);
                if ( color == 1) HIGH(LED1);
                if ( color == 2) HIGH(LED4);
                if ( color == 3) HIGH(LED2);
            }
            S1memory = PUSH(S1);

            TIM2_SetCompare1(r);
            TIM2_SetCompare2(g);
            TIM2_SetCompare3(b);
        }
    }
}

/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"
