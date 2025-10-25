/*
 * Copyright (c) 2025- liang yan <yanl1229@163.com>
 *
 * STM32F1XX AFIO
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
#ifndef STM32F1XX_AFIO_H
#define STM32F1XX_AFIO_H

#include "hw/sysbus.h"

#define STM_AFIO_EVCR               0x00
#define STM_AFIO_MAPR               0x04
#define STM_AFIO_EXTICR1            0x08
#define STM_AFIO_EXTICR2            0x0c
#define STM_AFIO_EXTICR3            0x10
#define STM_AFIO_EXTICR4            0x14
#define STM_AFIO_MAPR2              0x1c

#define TYPE_STM32F1XX_AFIO "stm32f1xx-afio"
#define STM32F1XX_AFIO(obj) \
    OBJECT_CHECK(STM32F1XXAFIOState, (obj), TYPE_STM32F1XX_AFIO)

typedef struct {
    /* <private> */
    SysBusDevice parent_obj;

    /* <public> */
    MemoryRegion mmio;

    uint32_t afio_evcr;
    uint32_t afio_mapr;
    uint32_t afio_exticr1;
    uint32_t afio_exticr2;
    uint32_t afio_exticr3;
    uint32_t afio_exticr4;
    uint32_t afio_mapr2;


    qemu_irq irq;

} STM32F1XXAFIOState;

#endif
