
#include "gpio.h"
#include "pin_defs.h"

#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_utils.h"

#include <stdint.h>
#include <stdbool.h>


static void SPI_Write(uint8_t value);

void GPIO_init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOD);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOE);
  

  // configure GPIO pins for USART1 TX communication
  GPIO_InitStruct.Pin = UART_TX_PIN;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_MEDIUM;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(UART_PORT, &GPIO_InitStruct);


  // configure GPIO pins for USART1 RX communication
  GPIO_InitStruct.Pin = UART_RX_PIN;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(UART_PORT, &GPIO_InitStruct);

  // configure output pins on port D
  GPIO_InitStruct.Pin = SPI_CS_PIN | SPI_CLK_PIN | SPI_MOSI_PIN;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_MEDIUM;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  // first initial pin outputs low
  LL_GPIO_ResetOutputPin(GPIOD, GPIO_InitStruct.Pin);
  // then set pins as output
  LL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  // configure output pins on port E
  GPIO_InitStruct.Pin = SPI_DC_PIN | EPD_RST_PIN | LED_PIN;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_MEDIUM;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  // first initial pin outputs low
  LL_GPIO_ResetOutputPin(GPIOD, GPIO_InitStruct.Pin);
  // then set pins as output
  LL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  // configure input pin on port E
  GPIO_InitStruct.Pin = EPD_BUSY_PIN;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(GPIOE, &GPIO_InitStruct);

}

void EPD_reset(void)
{
  LL_GPIO_ResetOutputPin(EPD_RST_PORT, EPD_RST_PIN); 
  LL_mDelay(10);
  LL_GPIO_SetOutputPin(EPD_RST_PORT, EPD_RST_PIN); 
  LL_mDelay(10);
}

bool EPD_is_busy(void)
{
  // 1 = not busy
  if(LL_GPIO_IsOutputPinSet(EPD_BUSY_PORT, EPD_BUSY_PIN)){
    return false;
  }else{  // 0 = busy
    return true;
  }
}

void EPD_WriteCMD(uint8_t command)
{
  // activate chip select (CS = 0)
  LL_GPIO_ResetOutputPin(SPI_CS_PORT, SPI_CS_PIN);
  // set to command mode (DC = 0)
  LL_GPIO_ResetOutputPin(SPI_DC_PORT, SPI_DC_PIN);
  // write data
  SPI_Write(command);
  // deactivate chip select (CS = 1)
  LL_GPIO_SetOutputPin(SPI_CS_PORT, SPI_CS_PIN);
}

void EPD_WriteData(uint8_t data)
{
  // activate chip select (CS = 0)
  LL_GPIO_ResetOutputPin(SPI_CS_PORT, SPI_CS_PIN);
  // set to command mode (DC = 0)
  LL_GPIO_ResetOutputPin(SPI_DC_PORT, SPI_DC_PIN);
  // write data
  SPI_Write(data);
  // deactivate chip select (CS = 1)
  LL_GPIO_SetOutputPin(SPI_CS_PORT, SPI_CS_PIN);
}

//bit-banging SPI write byte
static void SPI_Write(uint8_t value)
{
  uint32_t i;
  LL_GPIO_ResetOutputPin(SPI_CLK_PORT, SPI_CLK_PIN);  

  for(i=8; i>0; i--){
    if(value&0x80)
      LL_GPIO_SetOutputPin(SPI_MOSI_PORT, SPI_MOSI_PIN);
    else
      LL_GPIO_ResetOutputPin(SPI_MOSI_PORT, SPI_MOSI_PIN);

    __NOP();
    __NOP();
    __NOP();
    __NOP();

    LL_GPIO_SetOutputPin(SPI_CLK_PORT, SPI_CLK_PIN);
    // max SPI speed is 10MHz and sysclk is 36MHz
    // so add some NOPs to slow below 10MHz
    __NOP();
    __NOP();
    __NOP();
    __NOP();

    LL_GPIO_ResetOutputPin(SPI_CLK_PORT, SPI_CLK_PIN);
    __NOP();
    __NOP();
    __NOP();
    __NOP();

    value=value<<1;
  }
}
