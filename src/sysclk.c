

#include "stm32f1xx_ll_system.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_cortex.h"

#include <stdint.h>

static uint32_t tick_count = 0;

void sysclk_init(void)
{
  const uint32_t desired_sys_clk = 36000000;

  //set up to run off the 36MHz PLL
  LL_SetFlashLatency(desired_sys_clk);

  LL_RCC_HSE_Enable();
  LL_RCC_HSE_EnableCSS();     // enable clock security to detect HSE failure

  // delay until HSE is ready
  while (LL_RCC_HSE_IsReady() == 0U)
  {
  }

  // divide the 8MHz HSE clock by 2 and multiply by 9 to get 36MHz
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_2, LL_RCC_PLL_MUL_9);
  LL_RCC_PLL_Enable();

   // delay until PLL is ready
  while (LL_RCC_PLL_IsReady() == 0U)
  {
  }

  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

  // delay until HSE is system clock
  while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  }

  // update the global variable SystemCoreClock to reflect the new core clock
  SystemCoreClockUpdate();


  LL_RCC_ClocksTypeDef clk_struct;

  LL_RCC_GetSystemClocksFreq(&clk_struct);
  LL_Init1msTick(clk_struct.HCLK_Frequency);

  LL_SYSTICK_EnableIT();
}

uint32_t get_tick(void)
{
  return tick_count;
}

void SysTick_Handler(void)
{
  tick_count++;
}