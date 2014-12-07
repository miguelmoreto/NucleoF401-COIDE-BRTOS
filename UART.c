/*
 * BRTOS USART driver.
 *
 * */
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"

#include "UART.h"

/* Init the USART 2 */
void UART2_Init(uint8_t priority)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* enable peripheral clock for USART2 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	/* GPIOA clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	/* GPIOA Configuration:  USART1 Tx (PA2) and Rx (PA3) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Connect USART1 pins to AF7 */
	/* Tx = PA2
	 * Rx = PA3 */
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);

	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART2, &USART_InitStructure);

	// Interrupt.
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	if (OSSemCreate(0, &SerialTX2) != ALLOC_EVENT_OK){
		// Oh Oh
    	// It should not enter here!!!
		while(1){};
	};

	if (OSQueueCreate(&SerialPortBuffer2,128, &Serial2) != ALLOC_EVENT_OK){
		// Oh Oh
		// It should not enter here!!!
		while(1){};
	};

    /* Creates a mutex with priority equal to 10.
     * The mutex priority have to be greater than
     * the highest priority of the task the access
     * the shared resource. */
    if (OSMutexCreate(&SerialMutex,priority) != ALLOC_EVENT_OK){
    	// Mutex allocation failed
    	// Treat this error here !!!
    	while(1){};
    }

	USART_Cmd(USART2, ENABLE); // enable USART2
}

unsigned char UARTPutString(USART_TypeDef* USARTx, char *string)
{
	INT8U result = 0;

	/* Using mutex to avoid more than one task
	 * using the serial at the same time */
	(void)OSMutexAcquire(SerialMutex);

	while(*string)
	{
		/* Transmit Data */
		USARTx->DR = *string;
		USART_ITConfig(USARTx, USART_IT_TC, ENABLE);
		if (USARTx == USART2)
		{
			result = OSSemPend(SerialTX2,5);
		}
	    string++;
	}

	/* Release the shared resource */
	(void)OSMutexRelease(SerialMutex);

	return result;
}

unsigned char UARTPutChar(USART_TypeDef* USARTx, char data)
{
	INT8U result = 0;

	/* Using mutex to avoid more than one task
	 * using the serial at the same time */
	(void)OSMutexAcquire(SerialMutex);

	USARTx->DR = data;
	USART_ITConfig(USARTx, USART_IT_TC, ENABLE);
	if (USARTx == USART2)
	{
		result = OSSemPend(SerialTX2,5);
	}

	// Release the shared resource
	(void)OSMutexRelease(SerialMutex);

	return result;
}


unsigned char UARTGetChar(USART_TypeDef* USARTx, unsigned char *data, int timeout)
{
	unsigned char ret=0;

	if (USARTx == USART2)
	{
		ret = OSQueuePend(Serial2, data, timeout);
	}

	return ret;
}


void USART2_IRQHandler(void)
{
	  /*******************************
	   * OS-specific Interrupt Enter.
	   *******************************/
	  if (USART_GetFlagStatus(USART2, USART_FLAG_TC) == SET)
	  {
		  // Clear transmission flag
		  USART_ITConfig(USART2, USART_IT_TC, DISABLE);
		  USART_ClearFlag(USART2, USART_FLAG_TC);

		  (void)OSSemPost(SerialTX2);
	  }

	  if (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET)
	  {
			char receive_byte = USART2->DR;

			if (OSQueuePost(Serial2, receive_byte) == BUFFER_UNDERRUN)
			{
				// Problem: buffer underrun
				// OSCleanQueue(Serial1);
			}

			// Clear reception flag.
			USART_ClearFlag(USART2, USART_FLAG_RXNE);
	  }

	  // ************************
	  // Interrupt Exit
	  // ************************
	  OS_INT_EXIT_EXT();
	  // ************************
}
