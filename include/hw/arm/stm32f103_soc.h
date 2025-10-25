/*
 * Copyright (c) 2025 liang yan <yanl1229@163.com>
 *
 * STM32F103 SoC
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 * Contributions after 2012-01-13 are licensed under the terms of the
 * GNU GPL, version 2 or (at your option) any later version.
 */
#ifndef STM32F103_SOC_H
#define STM32F103_SOC_H

#include "hw/or-irq.h"
#include "hw/arm/armv7m.h"
#include "hw/misc/stm32f1xx_rcc.h"
#include "hw/misc/stm32f1xx_exti.h"
#include "hw/misc/stm32f1xx_pwr.h"
#include "hw/char/stm32f1xx_usart.h"
#include "hw/misc/stm32f1xx_flash.h"
#include "hw/gpio/stm32f1xx_gpio.h"
#include "hw/misc/stm32f1xx_afio.h"

#define FLASH_BASE_ADDRESS  0x8000000
#define FLASH_SIZE          (128*1024)
#define SRAM_BASE_ADDRESS   0x20000000
#define SRAM_SIZE           (20 * 1024)

#define RCC_BASE_ADDR    0x40021000
#define EXTI_BASE_ADDR   0x40010400
#define PWR_BASE_ADDR    0x40007000
#define FLASH_BASE_ADDR  0x40022000

#define STM_NUM_USARTS 3
#define UART1_BASE_ADDR  0x40013800
#define UART2_BASE_ADDR  0x40004400
#define UART3_BASE_ADDR  0x40004800

#define STM_NUM_GPIOS       7
#define STM_GPIO_PORTA      0x40010800
#define STM_GPIO_PORTB      0x40010c00
#define STM_GPIO_PORTC      0x40010000
#define STM_GPIO_PORTD      0x40011400
#define STM_GPIO_PORTE      0x40011800
#define STM_GPIO_PORTF      0x40011c00
#define STM_GPIO_PORTG      0x40012000

#define STM_AFIO_BASE   0x40010000

#define TYPE_STM32F103_SOC "stm32f103-soc"
#define STM32F103_SOC(obj) \
    OBJECT_CHECK(STM32F103State, (obj), TYPE_STM32F103_SOC)

typedef struct STM32F103State {
    /*< private >*/
    SysBusDevice parent_obj;
    /*< public >*/

    char *kernel_filename;
    char *cpu_type;
    ARMv7MState armv7m;

    STM32F1XXRccState rcc;
    STM32F1xxExtiState exti;
    STM32F1XXPowerState pwr;
    STM32F1XXFlashState flash;
    STM32F1XXUsartState usart[STM_NUM_USARTS];
    STM32F1XXGPIOState gpio[STM_NUM_GPIOS];
    STM32F1XXAFIOState afio;

} STM32F103State;

#endif
