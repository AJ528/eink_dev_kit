
#include "mprintf.h"
#include "sysclk.h"
#include "gpio.h"
#include "uart.h"
#include "mcli.h"
#include "display_epd.h"

#include "demo_image1.h"
#include "demo_image2.h"
#include "demo_image3.h"
#include "demo_image4.h"
#include "demo_image5.h"
#include "demo_image6.h"

#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_usart.h"

// explicitly include stm32wlxx.h for access to DWT registers
#include "stm32f1xx.h"

#include <stdint.h>
#include <stdbool.h>

static inline void enable_cycle_count(void);

extern uint32_t _vector_table_offset;

int main(void)
{
  SCB->VTOR = (uint32_t)(&_vector_table_offset);  // set the vector table offset
  enable_cycle_count();

  // initialize GPIO pins
  GPIO_init();
  /* Configure the system clock to run off HSE */
  sysclk_init();
  // initialize UART
  UART_init();

  EPD_init();
  EPD_whitescreen_white();
  EPD_deepsleep();
  LL_mDelay(4000);

  EPD_init();
  EPD_whitescreen_all(gImage_3);
  EPD_deepsleep();
  LL_mDelay(4000);

  EPD_init_fast();
  EPD_whitescreen_all(gImage_4);
  EPD_deepsleep();
  LL_mDelay(4000);



  EPD_init();
  EPD_whitescreen_all(gImage_5);
  EPD_deepsleep();
  LL_mDelay(4000);

  EPD_init();
  EPD_whitescreen_all(gImage_6);
  EPD_deepsleep();
  LL_mDelay(4000);

  // EPD_init();
  // EPD_whitescreen_white();
  // EPD_deepsleep();


// infinite loop
  while(1){

    // if UART data is present, receive it
    // TODO: trigger this off an interrupt?
    if(LL_USART_IsActiveFlag_RXNE(USART1)){
      char c = (char)LL_USART_ReceiveData8(USART1);
      cli_input(c);
    }
    cli_process();

  }
}

int32_t putchar_(char c)
{
  // loop while the LPUART_TDR register is full
  while(LL_USART_IsActiveFlag_TXE(USART1) != 1);
  // once the LPUART_TDR register is empty, fill it with char c
  LL_USART_TransmitData8(USART1, (uint8_t)c);
  return (c);
}

static inline void enable_cycle_count(void)
{
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CYCCNT = 0;
  DWT->LSUCNT = 0;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk | DWT_CTRL_LSUEVTENA_Msk;
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
}
#endif /* USE_FULL_ASSERT */
