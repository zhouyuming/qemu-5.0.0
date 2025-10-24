/*
 * Copyright (c) 2020 liang yan <yanl1229@163.com>
 *
 * STM32F4XX RCC
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
#ifndef STM32F4XX_RCC_H
#define STM32F4XX_RCC_H

#include "hw/sysbus.h"
#include "hw/hw.h"

#define RCC_CR              0x00
#define RCC_PLLCFGR         0x04
#define RCC_CFGR            0x08
#define RCC_CIR             0x0c
#define RCC_AHB1RSTR        0x10
#define RCC_AHB2RSTR        0x14
#define RCC_AHB3RSTR        0x18

#define RCC_APB1RSTR        0x20
#define RCC_APB2RSTR        0x24
#define RCC_AHB1ENR         0x30
#define RCC_AHB2ENR         0x34
#define RCC_AHB3ENR         0x38

#define RCC_APB1ENR         0x40
#define RCC_APB2ENR         0x44
#define RCC_AHB1LPENR       0x50
#define RCC_AHB2LPENR       0x54
#define RCC_AHB3LPENR       0x58
#define RCC_APB1LPENR       0x60
#define RCC_APB2LPENR       0x64
#define RCC_BDCR            0x70
#define RCC_CSR             0x74
#define RCC_SSCGR           0x80
#define RCC_PLLI2SCFGR      0x84
#define RCC_PLLSAICFGR      0x88
#define RCC_DCKCFGR         0x8c

#define RCC_CR_HSION        (1<<0)
#define RCC_CR_HSIRDY       (1<<1)
#define RCC_CR_HSENON       (1<<16)
#define RCC_CR_HSERDY       (1<<17)
#define RCC_CR_PLLON        (1<<24)
#define RCC_CR_PLLRDY       (1<<25)
#define RCC_CR_PLLI2SON     (1<<26)
#define RCC_CR_PLLI2SRDY    (1<<27)
#define RCC_CR_PLLSAION     (1<<28)
#define RCC_CR_PLLSAIRDY    (1<<29)

#define RCC_PLLCFGR_PLLDRC       (1<<22)
#define RCC_PLLCFGR_PLLM_SHIFT   0
#define RCC_PLLCFGR_PLLM_MASK    (0x3f)
#define RCC_PLLCFGR_PLLN_SHIFT   6
#define RCC_PLLCFGR_PLLN_MASK    (0x1ff)
#define RCC_PLLCFGR_PLLP_SHIFT   16
#define RCC_PLLCFGR_PLLP_MASK    (0x03)
#define RCC_PLLCFGR_PLLQ_SHIFT   24
#define RCC_PLLCFGR_PLLQ_MASK    (0xf)
#define RCC_CSR_LSION            (1<<0)
#define RCC_CSR_LSIRDY           (1<<1)

#define RCC_BDCR_LSEON           (1<<0)
#define RCC_BDCR_LSERDY          (1<<1)
#define RCC_CFGR_SW_HSI          (0x00)
#define RCC_CFGR_SW_HSE          (0x01)
#define RCC_CFGR_SW_PLL          (0x02)
#define RCC_CFGR_SW_MASK         (0x03)
#define PCC_CFGR_PLLMUL_SHIFT   18
#define RCC_CFGR_PLLMUL_MASK (0x0F)

#define RCC_CFGR_SWS_HSI (0x00 << 2)
#define RCC_CFGR_SWS_HSE (0x01 << 2)
#define RCC_CFGR_SWS_PLL (0x02 << 2)

#define RCC_CFGR_PLL_SRC (1 << 16)

#define RCC_CFGR2_PREDIV1_MASK (0x0F)

#define TYPE_STM32F4XX_RCC "stm32f4xx-rcc"
#define STM32F4XX_RCC(obj) \
    OBJECT_CHECK(STM32F4XXRccState, (obj), TYPE_STM32F4XX_RCC)

typedef struct {
    /* <private> */
    SysBusDevice parent_obj;

    /* <public> */
    MemoryRegion mmio;

    uint32_t rcc_cr;
    uint32_t rcc_pllcfgr;
    uint32_t rcc_cfgr;
    uint32_t rcc_cir;
    uint32_t rcc_ahb1rstr;
    uint32_t rcc_ahb2rstr;
    uint32_t rcc_ahb3rstr;
    uint32_t rcc_apb1rstr;
    uint32_t rcc_apb2rstr;
    uint32_t rcc_ahb1enr;
    uint32_t rcc_ahb2enr;
    uint32_t rcc_ahb3enr;
    uint32_t rcc_apb1enr;
    uint32_t rcc_apb2enr;
    uint32_t rcc_ahb1lpenr;
    uint32_t rcc_ahb2lpenr;
    uint32_t rcc_ahb3lpenr;
    uint32_t rcc_apb1lpenr;
    uint32_t rcc_apb2lpenr;
    uint32_t rcc_bdcr;
    uint32_t rcc_csr;
    uint32_t rcc_sscgr;
    uint32_t rcc_plli2scfgr;
    uint32_t rcc_pllsaicfgr;
    uint32_t rcc_dckcfgr;
} STM32F4XXRccState;

#endif
