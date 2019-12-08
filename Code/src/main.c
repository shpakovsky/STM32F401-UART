#include "stm32f4xx.h"

void UART_Init(void);
void UART_Send_Message(char* message);

int main(void)
{
	
	UART_Init();
	
	while(1){}
	
}

void UART_Init(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	
	/* PA9 - transmitter, PA10 - reciver: */
	GPIOA->MODER |= GPIO_MODER_MODER9_1;
	GPIOA->MODER |= GPIO_MODER_MODER10_1;
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR9_1;
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR10_1;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR9_0;
	
	/* Number of these Alternative Functions is 7 */
	GPIOA->AFR[1] |= (0x07 << 4);
	GPIOA->AFR[1] |= (0x07 << (4*2));
	
	/* Baud rate 9600: */
	USART1->BRR = 0x1117;
	
	/* Transmitter, Receiver and USART enable: */
	USART1->CR1 |= USART_CR1_TE;
	USART1->CR1 |= USART_CR1_RE;
	USART1->CR1 |= USART_CR1_UE;
	
	/* Enable Interrupts: */
	USART1->CR1 |= USART_CR1_IDLEIE;
	NVIC_EnableIRQ(USART1_IRQn);
}

void UART_Send_Message(char* message)
{
	uint8_t i = 0;
	
	while(message[i])
	{
		/* Waiting for data transfer complete: */
		while(!(USART1->SR & USART_SR_TC));
		
		USART1->DR = message[i++];
	}
}
/* RXNEIE bit to null */
void USART1_IRQHandler(void)
{
	if(USART1->SR & USART_CR1_RXNEIE)
	{
		USART1->SR &= ~USART_CR1_RXNEIE;
		
		if(USART1->DR == '1')
			UART_Send_Message("1");
		else
			UART_Send_Message("0");
	}
}

