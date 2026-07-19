#include "uart.h"

#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_usart.h"

void UART_init(void)
{
  // set the LPUART clock source to the peripheral clock
  // LL_RCC_SetLPUARTClockSource(LL_RCC_LPUART1_CLKSOURCE_PCLK1);

  // enable clocks for LPUART
  // LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_LPUART1);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);

  LL_USART_InitTypeDef USART_InitStruct = {
    .BaudRate = 115200,
    .DataWidth = LL_USART_DATAWIDTH_8B,
    .StopBits = LL_USART_STOPBITS_1,
    .Parity = LL_USART_PARITY_NONE,
    .TransferDirection = LL_USART_DIRECTION_TX_RX,
    .HardwareFlowControl = LL_USART_HWCONTROL_NONE,
    .OverSampling = LL_USART_OVERSAMPLING_16
  };
  LL_USART_Init(USART1, &USART_InitStruct);
  LL_USART_Enable(USART1);

  

  // // wait for the LPUART module to send an idle frame and finish initialization
  // while(!(LL_LPUART_IsActiveFlag_TEACK(LPUART1)) || !(LL_LPUART_IsActiveFlag_REACK(LPUART1)));
}