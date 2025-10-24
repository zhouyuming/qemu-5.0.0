/*
 * Copyright (c) 2020 liang yan <yanl1229@163.com>
 *
 * STM32F4XX Power
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
#ifndef STM32F4XX_PWR_H
#define STM32F4XX_PWR_H
#include "hw/sysbus.h"

#define STM_PWR_CR      0x00
#define STM_PWR_CSR     0x04

#define PWR_CR_DBP      (1 << 8)
#define PWR_CR_ODEN     (1 << 16)
#define PWR_CR_ODSWEN   (1 << 17)

#define PWR_CSR_ODRDY   (1 << 16)
#define PWR_CSR_ODSWRDY (1 << 17)

#define TYPE_STM32F4XX_POWER "stm32f4xx-pwr"
#define STM32F4XX_POWER(obj) \
    OBJECT_CHECK(STM32F4XXPowerState, (obj), TYPE_STM32F4XX_POWER)

typedef struct {
    /* <private> */
    SysBusDevice parent_obj;

    /* <public> */
    MemoryRegion mmio;

    uint32_t pwr_cr;
    uint32_t pwr_csr;
} STM32F4XXPowerState;

#endif
