/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __INIT_PERIPH_H
#define __INIT_PERIPH_H

#define LED1_PORT 				GPIOA
#define LED1 					GPIO_Pin_5
#define LED1_RCC				RCC_AHB1Periph_GPIOA
#define USER_BTN_PORT			GPIOC
#define USER_BTN				GPIO_Pin_13
#define USER_BTN_RCC 			RCC_AHB1Periph_GPIOC
#define USER_BTN_EXTI_LINE		EXTI_Line13
#define USER_BTN_PortSource		EXTI_PortSourceGPIOC
#define USER_BTN_PinSource		EXTI_PinSource13
#define USER_BTN_IRQchannel		EXTI15_10_IRQn
#define USER_BTN_IRQ_handler	EXTI15_10_IRQHandler


void MyConfigTimers(void);
void MyConfigGPIO(void);
void MyConfigUSART(void);

#endif /* __INIT_PERIPH_H */
