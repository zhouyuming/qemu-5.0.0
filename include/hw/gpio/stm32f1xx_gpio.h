/*
 * Copyright (c) 2025- liang yan <yanl1229@163.com>
 *
 * STM32F1XX GPIO
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
#ifndef STM32F1XX_GPIO_H
#define STM32F1XX_GPIO_H

#include "hw/sysbus.h"

#define STM_GPIO_CRL            0x00
#define STM_GPIO_CRH            0x04
#define STM_GPIO_IDR            0x08
#define STM_GPIO_ODR            0x0c
#define STM_GPIO_BSRR           0x10
#define STM_GPIO_BRR            0x14
#define STM_GPIO_LCKR           0x18

#define TYPE_STM32F1XX_GPIO "stm32f1xx-gpio"
#define STM32F1XX_GPIO(obj) \
    OBJECT_CHECK(STM32F1XXGPIOState, (obj), TYPE_STM32F1XX_GPIO)

typedef struct {
    /* <private> */
    SysBusDevice parent_obj;

    /* <public> */
    MemoryRegion mmio;

    uint32_t gpio_crl;
    uint32_t gpio_crh;
    uint32_t gpio_idr;
    uint32_t gpio_odr;
    uint32_t gpio_bsrr;
    uint32_t gpio_brr;
    uint32_t gpio_lckr;

    qemu_irq irq;

} STM32F1XXGPIOState;

#endif
