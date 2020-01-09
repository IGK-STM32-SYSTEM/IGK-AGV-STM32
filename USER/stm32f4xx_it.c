/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    04-August-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
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
#include "sys.h"
#include "communication.h"
/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}
void  DMA1_Stream6_IRQHandler(void)//串口2发送DMA中断
{
#if SYSTEM_SUPPORT_OS 	
	OSIntEnter();    
#endif	
    if(DMA_GetITStatus(DMA1_Stream6, DMA_IT_TCIF6))
    {
        USART_ITConfig(USART2, USART_IT_TC, ENABLE);//发送数据时，进入串口2DMA中断，打开串口3发送中断
				DMA_ClearITPendingBit(DMA1_Stream6, DMA_IT_TCIF6);
				DMA_Cmd(DMA1_Stream6, DISABLE);
    }
#if SYSTEM_SUPPORT_OS 	
	OSIntExit();  											 
#endif	
}
void  DMA1_Stream3_IRQHandler(void)//串口3发送DMA中断
{
#if SYSTEM_SUPPORT_OS 	
	OSIntEnter();    
#endif	
    if(DMA_GetITStatus(DMA1_Stream3, DMA_IT_TCIF3))
    {
      USART_ITConfig(USART3, USART_IT_TC, ENABLE);//发送数据时，进入串口3DMA中断，打开串口3发送中断
			DMA_ClearITPendingBit(DMA1_Stream3, DMA_IT_TCIF3);
			DMA_Cmd(DMA1_Stream3, DISABLE);
    }
#if SYSTEM_SUPPORT_OS 	
	OSIntExit();  											 
#endif	
}
void  DMA1_Stream4_IRQHandler(void)//串口4DMA发送中断
{
#if SYSTEM_SUPPORT_OS 	
	OSIntEnter();    
#endif	
    if(DMA_GetITStatus(DMA1_Stream4, DMA_IT_TCIF4))
    {
				USART_ITConfig(UART4, USART_IT_TC, ENABLE);//发送数据时，进入串口4DMA中断，打开串口4发送中断
				DMA_ClearITPendingBit(DMA1_Stream4, DMA_IT_TCIF4);
				DMA_Cmd(DMA1_Stream4, DISABLE);
    }
#if SYSTEM_SUPPORT_OS 	
	OSIntExit();  											 
#endif	
}
void  DMA1_Stream7_IRQHandler(void)//串口5DMA发送中断
{
#if SYSTEM_SUPPORT_OS 	
	OSIntEnter();    
#endif	
    if(DMA_GetITStatus(DMA1_Stream7, DMA_IT_TCIF7))
    {
				DMA_ClearITPendingBit(DMA1_Stream7, DMA_IT_TCIF7);
				DMA_Cmd(DMA1_Stream7, DISABLE);
    }
#if SYSTEM_SUPPORT_OS 	
	OSIntExit();  											 
#endif	
}
void  DMA2_Stream7_IRQHandler(void)//串口6发送DMA中断
{
    if(DMA_GetITStatus(DMA2_Stream7, DMA_IT_TCIF7))
    {
		DMA_ClearITPendingBit(DMA2_Stream7, DMA_IT_TCIF7);
		DMA_Cmd(DMA2_Stream7, DISABLE);
    }
}
void  DMA2_Stream0_IRQHandler(void)//AD DMA中断
{
}
void USART2_IRQHandler(void)             
{

} 
void USART3_IRQHandler(void)             
{


}
void UART4_IRQHandler(void)                
{

} 
void UART5_IRQHandler(void)             
{

}
void USART6_IRQHandler(void)             
{

} 
 
/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
//void PendSV_Handler(void)
//{
//}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
//void SysTick_Handler(void)
//{
// 
//}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
