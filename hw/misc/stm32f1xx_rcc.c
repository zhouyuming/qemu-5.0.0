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
#include "qemu/osdep.h"
#include "hw/misc/stm32f1xx_rcc.h"
#include "qemu/log.h"
#include "hw/irq.h"
#include "hw/qdev-properties.h"
#include "qemu/module.h"
#include "migration/vmstate.h"
#include "hw/timer/armv7m_systick.h"
#include "qemu/timer.h"

#ifndef STM_RCC_ERR_DEBUG
#define STM_RCC_ERR_DEBUG 0
#endif

#define DB_PRINT_L(lvl, fmt, args...) do { \
    if (STM_RCC_ERR_DEBUG >= lvl) { \
        qemu_log("%s: " fmt, __func__, ## args); \
    } \
} while (0);

#define DB_PRINT(fmt, args...) DB_PRINT_L(1, fmt, ## args)
static void stm32f1xx_rcc_reset(DeviceState *dev)
{
    STM32F1XXRccState *s = STM32F1XX_RCC(dev);

    s->rcc_cr           = 0x00000083;
    s->rcc_cfgr         = 0x00000000;
    s->rcc_cir          = 0x00000000;
    s->rcc_apb2rstr     = 0x00000000;
    s->rcc_apb1rstr     = 0x00000000;
    s->rcc_ahbenr       = 0x00000014;
    s->rcc_apb2enr      = 0x00000000;
    s->rcc_apb1enr      = 0x00000000;
    s->rcc_bdcr         = 0x00000000;
    s->rcc_csr          = 0x0c000000;
}

static uint64_t stm32f1xx_rcc_read(void *opaque, hwaddr addr,
                                    unsigned int size)
{
    STM32F1XXRccState *s = opaque;
    uint64_t reg = 0;

    DB_PRINT("Address: 0x%" HWADDR_PRIx "\n", addr);
    switch (addr) {
    case RCC_CR:
        reg = s->rcc_cr;
        break;
    case RCC_CFGR:
        reg = s->rcc_cfgr;
        break;
    case RCC_CIR:
        reg = s->rcc_cir;
        break;
    case RCC_APB2RSTR:
        reg = s->rcc_apb2rstr;
        break;
    case RCC_APB1RSTR:
        reg = s->rcc_apb1rstr;
        break;
    case RCC_AHBENR:
        reg = s->rcc_ahbenr;
        break;
    case RCC_APB2ENR:
        reg = s->rcc_apb2enr;
        break;
    case RCC_APB1ENR:
        reg = s->rcc_apb1enr;
        break;
    case RCC_BDCR:
        reg = s->rcc_bdcr;
        break;
    case RCC_CSR:
        reg = s->rcc_csr;
        break;
    default:
        qemu_log_mask(LOG_GUEST_ERROR,
              "%s: Bad offset 0x%"HWADDR_PRIx"\n", __func__, addr);
        reg = 0;
    }
    return reg;
}

static void stm32f1xx_rcc_write(void *opaque, hwaddr addr,
                                uint64_t val64, unsigned int size)
{
    STM32F1XXRccState *s = opaque;
    uint32_t value = val64;
    uint32_t pll_mutil;
    uint32_t cpu_freq_hz = 0;

    DB_PRINT("Address: 0x%" HWADDR_PRIx ", Value: 0x%x\n", addr, value);
    switch (addr) {
    case RCC_CR:
        s->rcc_cr = value;
        if (value & RCC_CR_HSION)
            s->rcc_cr |= RCC_CR_HSIRDY;
        else
            s->rcc_cr &= ~RCC_CR_HSIRDY;
        if (value & RCC_CR_HSENON)
            s->rcc_cr |= RCC_CR_HSERDY;
        else
            s->rcc_cr &= ~RCC_CR_HSIRDY;
        if (value & RCC_CR_PLLON)
            s->rcc_cr |= RCC_CR_PLLRDY;
        else
            s->rcc_cr &= ~RCC_CR_PLLRDY;
        break;
    case RCC_CFGR:
        s->rcc_cfgr = value;
        /* setup system clock source */
        if (value & RCC_CFGR_SW_PLL)
            s->rcc_cfgr |= RCC_CFGR_SWS_PLL;
        else if (value & RCC_CFGR_SW_HSE)
            s->rcc_cfgr |= RCC_CFGR_SWS_HSE;
        else
            s->rcc_cfgr |= RCC_CFGR_SWS_HSI;
        /* setup system clock */
        if ((value & RCC_CFGR_SW_PLL) || (value & RCC_CFGR_SW_HSE))
            cpu_freq_hz = 8000000;
        else {
            pll_mutil = ((value >> 18) & 0xf) + 2;
            if ((s->rcc_cfgr & RCC_CFGR_SWS_HSI)) {
                cpu_freq_hz = (8 / 2) * pll_mutil * 1000000;
            }
            else
            {
                if (value & RCC_CFGR_PLLXTPRE)
                    cpu_freq_hz = (8 / 2) * pll_mutil * 1000000;
                else
                    cpu_freq_hz = 8 * pll_mutil * 1000000;
            }
        }

        if (cpu_freq_hz) {
                system_clock_scale = NANOSECONDS_PER_SECOND / cpu_freq_hz;
                if (system_clock_scale == 0)
                        system_clock_scale = 1;
        }
        break;
    case RCC_CIR:
        s->rcc_cir = value;
        break;
    case RCC_APB2RSTR:
        s->rcc_apb2rstr = value;
        break;
    case RCC_APB1RSTR:
        s->rcc_apb1rstr = value;
        break;
    case RCC_AHBENR:
        s->rcc_ahbenr = value;
        break;
    case RCC_APB2ENR:
        s->rcc_apb2enr = value;
        break;
    case RCC_APB1ENR:
        s->rcc_apb1enr = value;
        break;
    case RCC_BDCR:
        s->rcc_bdcr = value;
        break;
    case RCC_CSR:
        s->rcc_csr = value;
        break;
    default:
        qemu_log_mask(LOG_GUEST_ERROR,
                  "%s: Bad offset 0x%"HWADDR_PRIx"\n", __func__, addr);
    }
}

static const MemoryRegionOps stm32f1xx_rcc_ops = {
        .read = stm32f1xx_rcc_read,
        .write = stm32f1xx_rcc_write,
        .endianness = DEVICE_NATIVE_ENDIAN,
};

static const VMStateDescription vmstate_stm32f1xx_rcc = {
        .name = TYPE_STM32F1XX_RCC,
        .version_id = 1,
        .minimum_version_id = 1,
        .fields = (VMStateField[]) {
                VMSTATE_UINT32(rcc_cr, STM32F1XXRccState),
                VMSTATE_UINT32(rcc_cfgr, STM32F1XXRccState),
                VMSTATE_UINT32(rcc_cir, STM32F1XXRccState),
                VMSTATE_UINT32(rcc_apb2rstr, STM32F1XXRccState),
                VMSTATE_UINT32(rcc_apb1rstr, STM32F1XXRccState),
                VMSTATE_UINT32(rcc_ahbenr, STM32F1XXRccState),
                VMSTATE_UINT32(rcc_apb2enr, STM32F1XXRccState),
                VMSTATE_UINT32(rcc_apb1enr, STM32F1XXRccState),
                VMSTATE_UINT32(rcc_bdcr, STM32F1XXRccState),
                VMSTATE_UINT32(rcc_csr, STM32F1XXRccState),
                VMSTATE_END_OF_LIST()
        }
};

static void stm32f1xx_rcc_init(Object *obj)
{
        STM32F1XXRccState *s = STM32F1XX_RCC(obj);

        memory_region_init_io(&s->mmio, obj, &stm32f1xx_rcc_ops, s,
                          TYPE_STM32F1XX_RCC, 0x400);
        sysbus_init_mmio(SYS_BUS_DEVICE(obj), &s->mmio);
}

static void stm32f1xx_rcc_class_init(ObjectClass *klass, void *data)
{
        DeviceClass *dc = DEVICE_CLASS(klass);

        dc->reset = stm32f1xx_rcc_reset;
        dc->vmsd = &vmstate_stm32f1xx_rcc;
}

static const TypeInfo stm32f1xx_rcc_info = {
        .name          = TYPE_STM32F1XX_RCC,
        .parent        = TYPE_SYS_BUS_DEVICE,
        .instance_size = sizeof(STM32F1XXRccState),
        .instance_init = stm32f1xx_rcc_init,
        .class_init    = stm32f1xx_rcc_class_init,
};

static void stm32f1xx_rcc_register_types(void)
{
        type_register_static(&stm32f1xx_rcc_info);
}

type_init(stm32f1xx_rcc_register_types)
