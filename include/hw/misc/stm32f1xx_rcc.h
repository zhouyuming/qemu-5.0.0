/*
 * Copyright (c) 2025 liang yan <yanl1229@163.com>
 *
 * STM32F1XX RCC
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
#ifndef STM32F1XX_RCC_H
#define STM32F1XX_RCC_H

#include "hw/sysbus.h"
#include "hw/hw.h"

#define RCC_CR              0x00
#define RCC_CFGR            0x04
#define RCC_CIR             0x08
#define RCC_APB2RSTR        0x0c
#define RCC_APB1RSTR        0x10
#define RCC_AHBENR          0x14
#define RCC_APB2ENR         0x18
#define RCC_APB1ENR         0x1c
#define RCC_BDCR            0x20
#define RCC_CSR             0x24

#define RCC_CR_HSION        (1<<0)
#define RCC_CR_HSIRDY       (1<<1)
#define RCC_CR_HSENON       (1<<16)
#define RCC_CR_HSERDY       (1<<17)
#define RCC_CR_PLLON        (1<<24)
#define RCC_CR_PLLRDY       (1<<25)

#define RCC_CSR_LSION            (1<<0)
#define RCC_CSR_LSIRDY           (1<<1)

#define RCC_BDCR_LSEON           (1<<0)
#define RCC_BDCR_LSERDY          (1<<1)
#define RCC_CFGR_SW_HSI          (0x00)
#define RCC_CFGR_SW_HSE          (0x01)
#define RCC_CFGR_SW_PLL          (0x02)
#define RCC_CFGR_SW_MASK         (0x03)
#define RCC_CFGR_PLLXTPRE        (1<<17)
#define RCC_CFGR_PLL_SRC         (1 << 16)

#define RCC_CFGR_SWS_HSI (0x00 << 2)
#define RCC_CFGR_SWS_HSE (0x01 << 2)
#define RCC_CFGR_SWS_PLL (0x02 << 2)



#define RCC_CFGR2_PREDIV1_MASK (0x0F)

#define TYPE_STM32F1XX_RCC "stm32f1xx-rcc"
#define STM32F1XX_RCC(obj) \
    OBJECT_CHECK(STM32F1XXRccState, (obj), TYPE_STM32F1XX_RCC)

typedef struct {
    /* <private> */
    SysBusDevice parent_obj;

    /* <public> */
    MemoryRegion mmio;

    uint32_t rcc_cr;
    uint32_t rcc_cfgr;
    uint32_t rcc_cir;
    uint32_t rcc_apb2rstr;
    uint32_t rcc_apb1rstr;
    uint32_t rcc_ahbenr;
    uint32_t rcc_apb2enr;
    uint32_t rcc_apb1enr;
    uint32_t rcc_bdcr;
    uint32_t rcc_csr;

} STM32F1XXRccState;

#endif
