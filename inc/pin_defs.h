#ifndef __PIN_DEFS_H
#define __PIN_DEFS_H

#define UART_TX_PIN                     LL_GPIO_PIN_9
#define UART_RX_PIN                     LL_GPIO_PIN_10
#define UART_PORT                       GPIOA

#define SPI_CS_PIN                      LL_GPIO_PIN_8
#define SPI_CS_PORT                     GPIOD
#define SPI_CLK_PIN                     LL_GPIO_PIN_9
#define SPI_CLK_PORT                    GPIOD
#define SPI_MOSI_PIN                    LL_GPIO_PIN_10
#define SPI_MOSI_PORT                   GPIOD
#define SPI_DC_PIN                      LL_GPIO_PIN_15
#define SPI_DC_PORT                     GPIOE

#define EPD_RST_PIN                     LL_GPIO_PIN_14
#define EPD_RST_PORT                    GPIOE
#define EPD_BUSY_PIN                    LL_GPIO_PIN_13
#define EPD_BUSY_PORT                   GPIOE

#define LED_PIN                         LL_GPIO_PIN_12
#define LED_PORT                        GPIOE


#endif /* __PIN_DEFS_H */
