/**
  ******************************************************************************
  * @file    LCD/LCD_SegmentsDrive/stm8l15x_it.c
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x_it.h"

/** @addtogroup STM8L15x_StdPeriph_Examples
  * @{
  */
extern void Set_time();
extern void Delay(uint32_t nCount);
extern void display_temp();
extern void ADC_Stop();
extern void ADC_Config();
/** @addtogroup LCD_SegmentsDrive
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

#ifdef _COSMIC_
/**
  * @brief Dummy interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(NonHandledInterrupt, 0)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif

/**
  * @brief TRAP interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER_TRAP(TRAP_IRQHandler)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
/**
  * @brief FLASH Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(FLASH_IRQHandler, 1)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
/**
  * @brief DMA1 channel0 and channel1 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(DMA1_CHANNEL0_1_IRQHandler, 2)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
/**
  * @brief DMA1 channel2 and channel3 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(DMA1_CHANNEL2_3_IRQHandler, 3)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief RTC / CSS_LSE Interrupt routine.
  * @param  None
  * @retval None
  */
extern void Time_Show();
INTERRUPT_HANDLER(RTC_CSSLSE_IRQHandler, 4)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  Time_Show();
 // ADC_Stop();
  RTC_ClearITPendingBit(RTC_IT_WUT);
}

/**
  * @brief External IT PORTE/F and PVD Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTIE_F_PVD_IRQHandler, 5)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External IT PORTB / PORTG Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTIB_G_IRQHandler, 6)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External IT PORTD /PORTH Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTID_H_IRQHandler, 7)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External IT PIN0 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI0_IRQHandler, 8)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External IT PIN1 Interrupt routine.
  * @param  None
  * @retval None
  */
char mode=0;
int8_t count_minute_set=0, count_hour_set=0;

INTERRUPT_HANDLER(EXTI1_IRQHandler, 9) //DOWN
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  ///////
  GPIO_Init(GPIOB, GPIO_Pin_1, GPIO_Mode_In_PU_No_IT);
  if((GPIO_ReadInputData(GPIOB)==0x0c)&(mode>0))//1100
  {
       Delay(500);
       if((GPIO_ReadInputData(GPIOB)==0x0c)&(mode>0))//1100
       {
            Delay(500);
            if((GPIO_ReadInputData(GPIOB)==0x0c)&(mode>0))//1100
            {
             if(mode==2)
              {
                count_minute_set--;
                if(count_minute_set<0) count_minute_set=59;
              }
              else if(mode==1)
              {
                count_hour_set--;
                if(count_hour_set<0) count_hour_set=23;
              }
              ////
              Set_time();    
            }
       }
  }
  while(GPIO_ReadInputData(GPIOB)==0x0c);
  GPIO_Init(GPIOB, GPIO_Pin_1, GPIO_Mode_In_PU_IT);   
  ////
  EXTI_ClearITPendingBit(EXTI_IT_Pin1);

}

/**
  * @brief External IT PIN2 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI2_IRQHandler, 10)//SET
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  GPIO_Init(GPIOB, GPIO_Pin_2, GPIO_Mode_In_PU_No_IT);
  if(GPIO_ReadInputData(GPIOB)==0x0a)///1010
  {
      Delay(500);
      if(GPIO_ReadInputData(GPIOB)==0x0a)///1010
      {
          Delay(500);
          if(GPIO_ReadInputData(GPIOB)==0x0a)///1010
          {
            mode++;
            if(mode==3) mode=0; 
          }
      }
  }
  while(GPIO_ReadInputData(GPIOB)==0x0a);
  GPIO_Init(GPIOB, GPIO_Pin_2, GPIO_Mode_In_PU_IT);
  EXTI_ClearITPendingBit(EXTI_IT_Pin2);
  
}

/**
  * @brief External IT PIN3 Interrupt routine.
  * @param  None
  * @retval None
  */
//
INTERRUPT_HANDLER(EXTI3_IRQHandler, 11) //UP
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  GPIO_Init(GPIOB, GPIO_Pin_3, GPIO_Mode_In_PU_No_IT);
  if((GPIO_ReadInputData(GPIOB)==0x06)&(mode>0))
  {
      Delay(500);
      if((GPIO_ReadInputData(GPIOB)==0x06)&(mode>0))
      {
        Delay(500);
        if((GPIO_ReadInputData(GPIOB)==0x06)&(mode>0))
        {
          if(mode==2)
              {
                count_minute_set++;
                if(count_minute_set>59) count_minute_set=0;
              }
              else if(mode==1)
              {
                count_hour_set++;
                if(count_hour_set>23) count_hour_set=0;
              }
              ////
              Set_time();   
        }
      }
  }
  if((GPIO_ReadInputData(GPIOB)==0x06)&(mode==0))
    {
      GPIO_Init(GPIOA, GPIO_Pin_2, GPIO_Mode_Out_PP_Low_Slow);
      GPIO_WriteBit(GPIOA,GPIO_Pin_2,SET);
    }
  while(GPIO_ReadInputData(GPIOB)==0x06);
  GPIO_Init(GPIOA, GPIO_Pin_2, GPIO_Mode_Out_OD_Low_Slow);
  GPIO_Init(GPIOB, GPIO_Pin_3, GPIO_Mode_In_PU_IT);
  EXTI_ClearITPendingBit(EXTI_IT_Pin3);
  
  
}

/**
  * @brief External IT PIN4 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI4_IRQHandler, 12)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External IT PIN5 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI5_IRQHandler, 13)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External IT PIN6 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI6_IRQHandler, 14)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External IT PIN7 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI7_IRQHandler, 15)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
/**
  * @brief LCD /AES Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(LCD_AES_IRQHandler, 16)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
/**
  * @brief CLK switch/CSS/TIM1 break /DAC Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(SWITCH_CSS_BREAK_DAC_IRQHandler, 17)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief ADC1/Comparator Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(ADC1_COMP_IRQHandler, 18)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief TIM2 Update/Overflow/Trigger/Break /USART2 TX Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM2_UPD_OVF_TRG_BRK_USART2_TX_IRQHandler, 19)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Timer2 Capture/Compare / USART2 RX Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM2_CC_USART2_RX_IRQHandler, 20)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}


/**
  * @brief Timer3 Update/Overflow/Trigger/Break/ USART3_TX  Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM3_UPD_OVF_TRG_BRK_USART3_TX_IRQHandler, 21)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
/**
  * @brief Timer3 Capture/Compare /USART3 RX Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM3_CC_USART3_RX_IRQHandler, 22)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
/**
  * @brief TIM1 Update/Overflow/Trigger/Commutation Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_COM_IRQHandler, 23)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
/**
  * @brief TIM1 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_CC_IRQHandler, 24)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief TIM4 Update/Overflow/Trigger Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM4_UPD_OVF_TRG_IRQHandler, 25)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
/**
  * @brief SPI1 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(SPI1_IRQHandler, 26)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief USART1 TX / TIM5 Update/Overflow/Trigger/Break Interrupt  routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(USART1_TX_TIM5_UPD_OVF_TRG_BRK_IRQHandler, 27)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief USART1 RX / Timer5 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(USART1_RX_TIM5_CC_IRQHandler, 28)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief I2C1 / SPI2 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(I2C1_SPI2_IRQHandler, 29)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @}
  */

/**
  * @}
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/