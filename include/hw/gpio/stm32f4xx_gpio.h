/*
 * Copyright (c) 2020 liang yan <yanl1229@163.com>
 *
 * STM32F4XX GPIO
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
#ifndef STM32F4XX_GPIO_H
#define STM32F4XX_GPIO_H

#include "hw/sysbus.h"

#define STM_GPIO_MODE           0x00
#define STM_GPIO_OTYPER         0x04
#define STM_GPIO_OSPEEDR        0x08
#define STM_GPIO_PUPDR          0x0C
#define STM_GPIO_IDR            0x10
#define STM_GPIO_ODR            0x14
#define STM_GPIO_BSRR           0x18
#define STM_GPIO_LCKR           0x1c
#define STM_GPIO_AFRL           0x20
#define STM_GPIO_AFRH           0x24

#define TYPE_STM32F4XX_GPIO "stm32f4xx-gpio"
#define STM32F4XX_GPIO(obj) \
    OBJECT_CHECK(STM32F4XXGPIOState, (obj), TYPE_STM32F4XX_GPIO)

typedef struct {
    /* <private> */
    SysBusDevice parent_obj;

    /* <public> */
    MemoryRegion mmio;

    uint32_t gpio_mode;
    uint32_t gpio_otyper;
    uint32_t gpio_ospeedr;
    uint32_t gpio_pupdr;
    uint32_t gpio_idr;
    uint32_t gpio_odr;
    uint32_t gpio_bsrr;
    uint32_t gpio_lckr;
    uint32_t gpio_afrl;
    uint32_t gpio_afrh;

    qemu_irq irq;
} STM32F4XXGPIOState;

#endif
