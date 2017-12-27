/**
  ******************************************************************************
  * @file    LCD/LCD_SegmentsDrive/main.c
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
  * @brief   Main program body
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
#include "stm8l15x.h"

RTC_InitTypeDef   RTC_InitStr;
RTC_TimeTypeDef   RTC_TimeStr;
RTC_DateTypeDef   RTC_DateStr;
////
void RTC_Config(void);
void LSE_StabTime(void);
void Calendar_Init(void);
void Time_Show();
void ADC_Stop();
void ADC_Config();
void display_temp();
float table_temp[]={27350,26198.88,25102.27,24057.34,23061.39,22111.92,
                            21206.53,20342.95,19519.08,18732.89,
                            
                            17982.48,17266.05 ,16581.92 ,15928.46,15304.16,
                            14707.58,14137.37 ,13592.23 ,13070.93,12572.34,
                            
                            12095.35,11638.93,11202.09,10783.90,10383.49,10000,
                            9632.65,9280.68,8943.38,8620.07,
                              
                            8310.11,8012.87,7727.79,7454.30,7191.88,6940.04,
                            6698.29,6466.20,6243.32,6029.25,
                            
                            5823.60,5626.00,5436.11,5253.57,5078.09,4909.34,
                            4747.05,4590.94,4440.74,4296.20,
                            
                            4157.09,4023.17,3894.23,3770.06,3650.47,3535.26,
                            3424.25,3317.28,3214.17,3114.78,
                            
                            3018.94,2926.53,2837.39,2751.40,2668.44,2588.39,
                            2511.12,2436.53,2364.52,2294.99,
                            
                            2227.84,2162.97,2100.31
                              
};
/** @addtogroup STM8L15x_StdPeriph_Examples
  * @{
  */

/**
  * @addtogroup LCD_SegmentsDrive
  * @{
  */


/* Private function prototypes -----------------------------------------------*/
void Delay(uint32_t nCount);
void Display(char digit, char num);
void LCD_Config(void);
void Dot_time(char status);
void Set_time(void);
float Res_Value();
float temperature();
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
extern int8_t count_minute_set, count_hour_set;
extern char mode;
char test=0;
void main(void)
{  
  LCD_Config();
  RTC_Config();
  Calendar_Init();
  //////////////////////////
  /* Get the current Time*/
  RTC_GetTime(RTC_Format_BIN, &RTC_TimeStr);
  count_hour_set=RTC_TimeStr.RTC_Hours;
  count_minute_set=RTC_TimeStr.RTC_Minutes;
  ///////////////////////////
  RTC_SetWakeUpCounter(2047);
  RTC_WakeUpCmd(ENABLE);
  //
  GPIO_Init(GPIOB, GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3, GPIO_Mode_In_PU_IT);//
  EXTI_SetPinSensitivity((EXTI_Pin_TypeDef)(EXTI_Pin_1|EXTI_Pin_2|EXTI_Pin_3), EXTI_Trigger_Falling);
  //
  enableInterrupts();
  while (1)
  {
    /////////////    
    GPIO_Init(GPIOA, GPIO_Pin_All, GPIO_Mode_Out_OD_Low_Slow);  //
    //GPIO_Init(GPIOA, GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7, GPIO_Mode_Out_OD_Low_Slow);  //
    GPIO_Init(GPIOB, GPIO_Pin_0|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7, GPIO_Mode_Out_OD_Low_Slow);  //
    GPIO_Init(GPIOC, GPIO_Pin_All, GPIO_Mode_Out_OD_Low_Slow);  //
    GPIO_Init(GPIOD, GPIO_Pin_All, GPIO_Mode_Out_OD_Low_Slow);  //
    GPIO_Init(GPIOE, GPIO_Pin_All, GPIO_Mode_Out_OD_Low_Slow);  //
    GPIO_Init(GPIOF, GPIO_Pin_All, GPIO_Mode_Out_OD_Low_Slow);  //
    ///
    halt();
    //display_temp();
  }
}
/**
  * @brief  Set all Pixel in the Matrix ON
  * @param  None
  * @retval None
  */

///
void Dot_time(char status)
{
  if(status==0) LCD->RAM[LCD_RAMRegister_10]&=0xef;
  else 
  {
    LCD->RAM[LCD_RAMRegister_10]&=0xff;
    LCD->RAM[LCD_RAMRegister_10]|=0x10;
  }
}
void Display(char digit, char num)
{
  //
  if(digit==0)
  {
    LCD->RAM[LCD_RAMRegister_0]&=0xfc;
    LCD->RAM[LCD_RAMRegister_3]&=0xcf;
    LCD->RAM[LCD_RAMRegister_7]&=0xfc;
    LCD->RAM[LCD_RAMRegister_10]&=0xcf;
  }
  else  if(digit==1)
  {
    LCD->RAM[LCD_RAMRegister_0]&=0xf3;
    LCD->RAM[LCD_RAMRegister_3]&=0x3f;
    LCD->RAM[LCD_RAMRegister_7]&=0xf3;
    LCD->RAM[LCD_RAMRegister_10]&=0x3f;
  }
  else  if(digit==2)
  {
    LCD->RAM[LCD_RAMRegister_0]&=0xcf;
    LCD->RAM[LCD_RAMRegister_4]&=0xfc;
    LCD->RAM[LCD_RAMRegister_7]&=0xcf;
    LCD->RAM[LCD_RAMRegister_11]&=0xfc;
  }
  else  if(digit==3)
  {
    LCD->RAM[LCD_RAMRegister_0]&=0x3f;
    LCD->RAM[LCD_RAMRegister_4]&=0xf3;
    LCD->RAM[LCD_RAMRegister_7]&=0x3f;
    LCD->RAM[LCD_RAMRegister_11]&=0xf3;
  }
  //
  switch(num)
  {
    case 0:
    /////
    LCD->RAM[LCD_RAMRegister_0]|=0x03<<(digit*2); //digit 1 ok
    if(digit<2) LCD->RAM[LCD_RAMRegister_3]|=0x10<<(digit*2); //
    else LCD->RAM[LCD_RAMRegister_4]|=0x01<<((digit-2)*2); //
    LCD->RAM[LCD_RAMRegister_7]|=0x03<<(digit*2);//
    if(digit<2) LCD->RAM[LCD_RAMRegister_10]|=0x20<<(digit*2);//
    else LCD->RAM[LCD_RAMRegister_11]|=0x02<<((digit-2)*2); //
    break;
    case 1: 
    LCD->RAM[LCD_RAMRegister_0]|=0x02<<(digit*2); //digit 1 ok
    //
    if(digit<2) LCD->RAM[LCD_RAMRegister_3]|=0x00<<(digit*2); //
    else LCD->RAM[LCD_RAMRegister_4]|=0x00<<((digit-2)*2); //
    //
    LCD->RAM[LCD_RAMRegister_7]|=0x02<<(digit*2);//
    //
    if(digit<2) LCD->RAM[LCD_RAMRegister_10]|=0x00<<(digit*2);//
    else LCD->RAM[LCD_RAMRegister_11]|=0x00<<((digit-2)*2); //
    //
    break;
    case 2:
    LCD->RAM[LCD_RAMRegister_0]|=0x03<<(digit*2); //digit 1 ok
    if(digit<2) LCD->RAM[LCD_RAMRegister_3]|=0x20<<(digit*2); //
    else LCD->RAM[LCD_RAMRegister_4]|=0x02<<((digit-2)*2); //
    LCD->RAM[LCD_RAMRegister_7]|=0x01<<(digit*2);//
    if(digit<2) LCD->RAM[LCD_RAMRegister_10]|=0x20<<(digit*2);//
    else LCD->RAM[LCD_RAMRegister_11]|=0x02<<((digit-2)*2); //
    break;
    case 3:
    LCD->RAM[LCD_RAMRegister_0]|=0x03<<(digit*2); //digit 1 ok
    if(digit<2) LCD->RAM[LCD_RAMRegister_3]|=0x20<<(digit*2); //
    else LCD->RAM[LCD_RAMRegister_4]|=0x02<<((digit-2)*2); //
    LCD->RAM[LCD_RAMRegister_7]|=0x02<<(digit*2);//
    if(digit<2) LCD->RAM[LCD_RAMRegister_10]|=0x20<<(digit*2);//
    else LCD->RAM[LCD_RAMRegister_11]|=0x02<<((digit-2)*2); //
    break;
    case 4:
    LCD->RAM[LCD_RAMRegister_0]|=0x02<<(digit*2); //digit 1 ok
    if(digit<2) LCD->RAM[LCD_RAMRegister_3]|=0x30<<(digit*2); //
    else LCD->RAM[LCD_RAMRegister_4]|=0x03<<((digit-2)*2); //
    LCD->RAM[LCD_RAMRegister_7]|=0x02<<(digit*2);//
    if(digit<2) LCD->RAM[LCD_RAMRegister_10]|=0x00<<(digit*2);//
    else LCD->RAM[LCD_RAMRegister_11]|=0x00<<((digit-2)*2); //
    break;
    case 5:
    LCD->RAM[LCD_RAMRegister_0]|=0x01<<(digit*2); //digit 1  ok
    if(digit<2) LCD->RAM[LCD_RAMRegister_3]|=0x30<<(digit*2); //
    else LCD->RAM[LCD_RAMRegister_4]|=0x03<<((digit-2)*2); //
    LCD->RAM[LCD_RAMRegister_7]|=0x02<<(digit*2);//
    if(digit<2) LCD->RAM[LCD_RAMRegister_10]|=0x20<<(digit*2);//
    else LCD->RAM[LCD_RAMRegister_11]|=0x02<<((digit-2)*2); //
    break;
    case 6:
    LCD->RAM[LCD_RAMRegister_0]|=0x01<<(digit*2); //digit 1  ok
    if(digit<2) LCD->RAM[LCD_RAMRegister_3]|=0x30<<(digit*2); //
    else LCD->RAM[LCD_RAMRegister_4]|=0x03<<((digit-2)*2); //
    LCD->RAM[LCD_RAMRegister_7]|=0x03<<(digit*2);//
    if(digit<2) LCD->RAM[LCD_RAMRegister_10]|=0x20<<(digit*2);//
    else LCD->RAM[LCD_RAMRegister_11]|=0x02<<((digit-2)*2); //
    break;
    case 7:
    LCD->RAM[LCD_RAMRegister_0]|=0x03<<(digit*2); //digit 1 ok
    if(digit<2) LCD->RAM[LCD_RAMRegister_3]|=0x00<<(digit*2); //
    else LCD->RAM[LCD_RAMRegister_4]|=0x00<<((digit-2)*2); //
    LCD->RAM[LCD_RAMRegister_7]|=0x02<<(digit*2);//
    if(digit<2) LCD->RAM[LCD_RAMRegister_10]|=0x00<<(digit*2);//
    else LCD->RAM[LCD_RAMRegister_11]|=0x00<<((digit-2)*2); //
    break;
    case 8:
    LCD->RAM[LCD_RAMRegister_0]|=0x03<<(digit*2); //digit 1 ok
    if(digit<2) LCD->RAM[LCD_RAMRegister_3]|=0x30<<(digit*2); //
    else LCD->RAM[LCD_RAMRegister_4]|=0x03<<((digit-2)*2); //
    LCD->RAM[LCD_RAMRegister_7]|=0x03<<(digit*2);//
    if(digit<2) LCD->RAM[LCD_RAMRegister_10]|=0x20<<(digit*2);//
    else LCD->RAM[LCD_RAMRegister_11]|=0x02<<((digit-2)*2); //
    break;
    case 9:
    LCD->RAM[LCD_RAMRegister_0]|=0x03<<(digit*2); //digit 1 ok
    if(digit<2) LCD->RAM[LCD_RAMRegister_3]|=0x30<<(digit*2); //
    else LCD->RAM[LCD_RAMRegister_4]|=0x03<<((digit-2)*2); //
    LCD->RAM[LCD_RAMRegister_7]|=0x02<<(digit*2);//
    if(digit<2) LCD->RAM[LCD_RAMRegister_10]|=0x20<<(digit*2);//
    else LCD->RAM[LCD_RAMRegister_11]|=0x02<<((digit-2)*2); //
    break;
    case 10:
    LCD->RAM[LCD_RAMRegister_0]|=0x00<<(digit*2); //digit 1 ok
    if(digit<2) LCD->RAM[LCD_RAMRegister_3]|=0x00<<(digit*2); //
    else LCD->RAM[LCD_RAMRegister_4]|=0x00<<((digit-2)*2); //
    LCD->RAM[LCD_RAMRegister_7]|=0x00<<(digit*2);//
    if(digit<2) LCD->RAM[LCD_RAMRegister_10]|=0x00<<(digit*2);//
    else LCD->RAM[LCD_RAMRegister_11]|=0x00<<((digit-2)*2); //
    break;
    case 11:
    
  }
}
/**
  * @brief Delay.
  * @param[in] nCount
  * @retval None
  */
void Delay(uint32_t nCount)
{
  /* Decrement nCount value */
  while (nCount != 0)
  {
    nCount--;
  }
}
void ADC_Stop()
{
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, DISABLE);
  ADC_DeInit(ADC1);
}
void ADC_Config()
{
    GPIO_Init(GPIOC, GPIO_Pin_7, GPIO_Mode_In_FL_No_IT);
    CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);
    ADC_DeInit(ADC1);
    ADC_Init(ADC1,ADC_ConversionMode_Continuous,ADC_Resolution_12Bit,ADC_Prescaler_1);
    ADC_SamplingTimeConfig(ADC1, ADC_Group_SlowChannels, ADC_SamplingTime_24Cycles);
   ADC_Cmd(ADC1,ENABLE);
   ADC_SchmittTriggerConfig(ADC1,ADC_Channel_3,DISABLE);
   ADC_ChannelCmd(ADC1,ADC_Channel_3, ENABLE);
   ADC_SoftwareStartConv(ADC1); 
}
float Res_Value()
{
  float res,adc;
  uint8_t i;
  ADC_Config();
    while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==0);
    for(i=0; i<10;i++)
    {
    adc+=ADC_GetConversionValue(ADC1);
    }
    adc=adc/10;
    res=(15000*adc)/(4096-adc);
    asm("nop");
    return res;
}
float temperature()
{
    uint8_t i=0;
    float min, max,temp;
    for(i=0;i<72;i++)
      {
        if(Res_Value()>=table_temp[i]){
            min=table_temp[i];
            max=table_temp[i-1];
            temp=(i-1)+((max-Res_Value())/(max-min));
            break;
          }
      }
    return temp;
}
void display_temp()
{
    int nhietdo;
    nhietdo=temperature()*10;
    Display(0,nhietdo/100);
    Display(1,(nhietdo%100)/10);
    Display(2,nhietdo%10);
    LCD->RAM[LCD_RAMRegister_11]&=0xfe;
    LCD->RAM[LCD_RAMRegister_11]|=0x01;
    
    LCD->RAM[LCD_RAMRegister_0]&=0x3f;
    LCD->RAM[LCD_RAMRegister_4]&=0xf3;
    LCD->RAM[LCD_RAMRegister_7]&=0x3f;
    LCD->RAM[LCD_RAMRegister_11]&=0xf3;
    LCD->RAM[LCD_RAMRegister_0]|=0xc0;
    LCD->RAM[LCD_RAMRegister_4]|=0x0c;
    
}
///////////////////////
void LCD_Config(void)
{
   /*
    The LCD is configured as follow:
     - clock source = LSE (32.768 KHz)
     - Voltage source = Internal
     - Prescaler = 2
     - Divider = 18 (16 + 2)  
     - Mode = 1/4 Duty, 1/3 Bias
     - LCD frequency = (clock source * Duty) / (Prescaler * Divider)
                     = 228 Hz ==> Frame frequency = 57 Hz */
  /* Enable LCD clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_LCD, ENABLE);
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSE, CLK_RTCCLKDiv_1);

  ///////
  /* Initialize the LCD */
  LCD_Init(LCD_Prescaler_2, LCD_Divider_18, LCD_Duty_1_4,
           LCD_Bias_1_3, LCD_VoltageSource_Internal);

  /* Mask register*/
  LCD_PortMaskConfig(LCD_PortMaskRegister_0, 0xFF);//SEG0 -> SEG7
  LCD_PortMaskConfig(LCD_PortMaskRegister_1, 0x00);
  LCD_PortMaskConfig(LCD_PortMaskRegister_2, 0x00);
  LCD_PortMaskConfig(LCD_PortMaskRegister_3, 0x00);
  LCD_PortMaskConfig(LCD_PortMaskRegister_4, 0x00);
  LCD_PortMaskConfig(LCD_PortMaskRegister_5, 0x00);

  LCD_PageSelect(LCD_PageSelection_FirstPage);//COM0 -> COM3
  
  LCD_ContrastConfig(LCD_Contrast_Level_5);
  LCD_DeadTimeConfig(LCD_DeadTime_2);
  LCD_PulseOnDurationConfig(LCD_PulseOnDuration_1);

  LCD_Cmd(ENABLE); /*!< Enable LCD peripheral */
  ////////////////////////////////////////////////////
}
////////////////////////
static void RTC_Config(void)
{
  /* Enable RTC clock */
  CLK_LSEConfig(CLK_LSE_ON);
  /* Wait for LSE clock to be ready */
  while (CLK_GetFlagStatus(CLK_FLAG_LSERDY) == RESET);
  /* wait for 1 second for the LSE Stabilisation */
  LSE_StabTime();

  /* Select LSE (32.768 KHz) as RTC clock source */
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSE, CLK_RTCCLKDiv_1);

  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);
  ////////
  /* Configures the RTC wakeup timer_step = RTCCLK/16 = LSE/16 = 488.28125 us */
  RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);

  /* Enable wake up unit Interrupt */
  RTC_ITConfig(RTC_IT_WUT, ENABLE);

  /* Enable general Interrupt*/
}
unsigned char hour=0, minute=0, second=0, date=0, month=0, year=0, weekend=0;
char status_dot_time=0;
extern char mode;
void Time_Show()
{
  /* Wait until the calendar is synchronized */
  while (RTC_WaitForSynchro() != SUCCESS);

  /* Get the current Time*/
  RTC_GetTime(RTC_Format_BIN, &RTC_TimeStr);
  RTC_GetDate(RTC_Format_BIN, &RTC_DateStr);

  /* Fill the LCDString fields with the current Time*/
  hour=RTC_TimeStr.RTC_Hours;
  minute=RTC_TimeStr.RTC_Minutes;
  second=RTC_TimeStr.RTC_Seconds;
  //
  char dv,ch;
  ch=hour/10;
  dv=hour%10;
  if((mode!=1)|(status_dot_time==0))
  {
    Display(0,ch);
    Display(1,dv);
  }
  else if(mode==1)
  {
    Display(0,10);
    Display(1,10);
  }
  ///
  ch=minute/10;
  dv=minute%10;
  ///
  if((mode!=2)|(status_dot_time==0))
  {
    Display(2,ch);
    Display(3,dv);
  }
  else if(mode==2)
  {
    Display(2,10);
    Display(3,10);
  }
  //
  status_dot_time=~status_dot_time;
  Dot_time(status_dot_time);

}

/**
  * @brief  Wait 1 sec for LSE stabilisation .
  * @param  None.
  * @retval None.
  * Note : TIM4 is configured for a system clock = 2MHz
  */
void LSE_StabTime(void)
{

  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);

  /* Configure TIM4 to generate an update event each 1 s */
  TIM4_TimeBaseInit(TIM4_Prescaler_16384, 123);
  /* Clear update flag */
  TIM4_ClearFlag(TIM4_FLAG_Update);

  /* Enable TIM4 */
  TIM4_Cmd(ENABLE);

  /* Wait 1 sec */
  while ( TIM4_GetFlagStatus(TIM4_FLAG_Update) == RESET );

  TIM4_ClearFlag(TIM4_FLAG_Update);

  /* Disable TIM4 */
  TIM4_Cmd(DISABLE);

  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, DISABLE);
}

void Calendar_Init(void)
{
  RTC_InitStr.RTC_HourFormat = RTC_HourFormat_24;
  RTC_InitStr.RTC_AsynchPrediv = 0x7f;
  RTC_InitStr.RTC_SynchPrediv = 0x00ff;
  RTC_Init(&RTC_InitStr);

//  RTC_DateStructInit(&RTC_DateStr);
//  RTC_DateStr.RTC_WeekDay = RTC_Weekday_Monday;
//  RTC_DateStr.RTC_Date = 13;
//  RTC_DateStr.RTC_Month = RTC_Month_October;
//  RTC_DateStr.RTC_Year = 17;
//  RTC_SetDate(RTC_Format_BIN, &RTC_DateStr);

  RTC_TimeStructInit(&RTC_TimeStr);
  RTC_TimeStr.RTC_Hours   = 12;
  RTC_TimeStr.RTC_Minutes = 40;
  RTC_TimeStr.RTC_Seconds = 00;
  RTC_SetTime(RTC_Format_BIN, &RTC_TimeStr);

}
//////////////////////////////
void Set_time(void)
{
  RTC_TimeStructInit(&RTC_TimeStr);
  RTC_TimeStr.RTC_Hours   = count_hour_set;
  RTC_TimeStr.RTC_Minutes = count_minute_set;
  RTC_TimeStr.RTC_Seconds = 00;
  RTC_SetTime(RTC_Format_BIN, &RTC_TimeStr);
}
#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif

/**
  * @}
  */

/**
  * @}
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
