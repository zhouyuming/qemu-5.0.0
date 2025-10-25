/*
 * Copyright (c) 2025 liang yan <yanl1229@163.com>
 *
 * STM32F1xx Flash interface
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
#ifndef STM32F1XX_FLASH_H
#define STM32F1XX_FLASH_H

#include "hw/sysbus.h"

#define STM_FLASH_ACR      0x00
#define STM_FLASH_KEYR     0x04
#define STM_FLASH_OPTKEYR  0x08
#define STM_FLASH_SR       0x0c
#define STM_FLASH_CR       0x10
#define STM_FLASH_AR       0x14
#define STM_FLASH_OBR      0x18
#define STM_FLASH_WRPR     0x20

#define PWR_CR_DBP      (1 << 8)

#define TYPE_STM32F1XX_FLASH "stm32f1xx-flash"
#define STM32F1XX_FLASH(obj) \
    OBJECT_CHECK(STM32F1XXFlashState, (obj), TYPE_STM32F1XX_FLASH)

typedef struct {
    /* <private> */
    SysBusDevice parent_obj;

    /* <public> */
    MemoryRegion mmio;

    uint32_t flash_acr;
    uint32_t flash_keyr;
    uint32_t flash_optkeyr;
    uint32_t flash_sr;
    uint32_t flash_cr;
    uint32_t flash_ar;
    uint32_t flash_obr;
    uint32_t flash_wrpr;

} STM32F1XXFlashState;

#endif
