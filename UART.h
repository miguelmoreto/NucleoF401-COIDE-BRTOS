/*
 * BRTOS USART driver.
 *
 * */
#ifndef __UART__H
#define __UART__H

#include "BRTOS.h"
#include "stm32f4xx_usart.h"

/* Declares a pointer to the semaphore that synchronizes
 * the serial data transmission. */
BRTOS_Sem   *SerialTX2;
/* Declares a Queue structure to serial data reception. */
OS_QUEUE SerialPortBuffer2;
/* Declares a pointer to a Queue control block for serial
 * port reception. */
BRTOS_Queue  *Serial2;
/* Declares a pointer to a mutex for controlling the access
 * of the serial port. */
BRTOS_Mutex *SerialMutex;

void UART2_Init(uint8_t priority);
unsigned char UARTPutString(USART_TypeDef* USARTx, char *string);
unsigned char UARTPutChar(USART_TypeDef* USARTx, char data);
unsigned char UARTGetChar(USART_TypeDef* USARTx, unsigned char *data, int timeout);

#endif
