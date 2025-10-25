/*
 * Copyright (c) 2025 liang yan <yanl1229@163.com>
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
#include "qemu/osdep.h"
#include "qapi/error.h"
#include "qemu/log.h"
#include "hw/gpio/stm32f1xx_gpio.h"
#include "hw/irq.h"
#include "hw/qdev-properties.h"
#include "migration/vmstate.h"
#include "qemu/module.h"

#ifndef STM_GPIO_ERR_DEBUG
#define STM_GPIO_ERR_DEBUG 0
#endif

#define DB_PRINT_L(lvl, fmt, args...) do { \
    if (STM_GPIO_ERR_DEBUG >= lvl) { \
        qemu_log("%s: " fmt, __func__, ## args); \
    } \
} while (0);

#define DB_PRINT(fmt, args...) DB_PRINT_L(1, fmt, ## args)

static void stm32f1xx_gpio_reset(DeviceState *dev)
{
    STM32F1XXGPIOState *s = STM32F1XX_GPIO(dev);
    s->gpio_crl     = 0x44444444;
    s->gpio_crh     = 0x44444444;
    s->gpio_idr     = 0x00000000;
    s->gpio_odr     = 0x00000000;
    s->gpio_bsrr    = 0x00000000;
    s->gpio_brr    = 0x00000000;
    s->gpio_lckr    = 0x00000000;
}

static uint64_t stm32f1xx_gpio_read(void *opaque, hwaddr addr,
                                     unsigned int size)
{
    STM32F1XXGPIOState *s = opaque;
    uint64_t retvalue = 0;

    DB_PRINT("Address: 0x%" HWADDR_PRIx "\n", addr);
    switch(addr) {
    case STM_GPIO_CRL:
        retvalue = s->gpio_crl;
        break;
    case STM_GPIO_CRH:
        retvalue = s->gpio_crh;
        break;
    case STM_GPIO_IDR:
        retvalue = s->gpio_idr;
        break;
    case STM_GPIO_ODR:
        retvalue = s->gpio_odr;
        break;
    case STM_GPIO_BSRR:
        retvalue = s->gpio_bsrr;
        break;
    case STM_GPIO_BRR:
        retvalue = s->gpio_brr;
        break;
    case STM_GPIO_LCKR:
        retvalue = s->gpio_lckr;
        break;
    default:
        qemu_log_mask(LOG_GUEST_ERROR, "%s: Bad offset 0x%" HWADDR_PRIx "\n",
            __func__, addr);
        retvalue = 0;
        break;
    }
    return retvalue;
}

static void stm32f1xx_gpio_write(void *opaque, hwaddr addr,
                                uint64_t val64, unsigned int size)
{
    STM32F1XXGPIOState *s = opaque;
    uint32_t value = val64;

    DB_PRINT("Address: 0x%" HWADDR_PRIx ", Value: 0x%x\n", addr, value);
    switch(addr) {
    case STM_GPIO_CRL:
        s->gpio_crl = value;
        break;
    case STM_GPIO_CRH:
        s->gpio_crh = value;
        break;
    case STM_GPIO_IDR:
        s->gpio_idr = value;
        break;
    case STM_GPIO_ODR:
        s->gpio_odr = value;
        break;
    case STM_GPIO_BSRR:
        s->gpio_bsrr = value;
        break;
    case STM_GPIO_BRR:
        s->gpio_brr = value;
        break;
    case STM_GPIO_LCKR:
        s->gpio_lckr = value;
        break;
    default:
        qemu_log_mask(LOG_GUEST_ERROR, "%s: Bad offset 0x%" HWADDR_PRIx "\n",
            __func__, addr);
    }
}

static const MemoryRegionOps stm32f1xx_gpio_ops = {
    .read = stm32f1xx_gpio_read,
    .write = stm32f1xx_gpio_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
};

static const VMStateDescription vmstate_stm32f1xx_gpio = {
    .name = TYPE_STM32F1XX_GPIO,
    .version_id = 1,
    .minimum_version_id = 1,
    .fields = (VMStateField[]) {
        VMSTATE_UINT32(gpio_crl, STM32F1XXGPIOState),
        VMSTATE_UINT32(gpio_crh, STM32F1XXGPIOState),
        VMSTATE_UINT32(gpio_idr, STM32F1XXGPIOState),
        VMSTATE_UINT32(gpio_odr, STM32F1XXGPIOState),
        VMSTATE_UINT32(gpio_bsrr, STM32F1XXGPIOState),
        VMSTATE_UINT32(gpio_brr, STM32F1XXGPIOState),
        VMSTATE_UINT32(gpio_lckr, STM32F1XXGPIOState),
        VMSTATE_END_OF_LIST()
    }
};

static void stm32f1xx_gpio_init(Object *obj)
{
    STM32F1XXGPIOState *s = STM32F1XX_GPIO(obj);

    memory_region_init_io(&s->mmio, obj, &stm32f1xx_gpio_ops, s,
                          TYPE_STM32F1XX_GPIO, 0x400);
    sysbus_init_mmio(SYS_BUS_DEVICE(obj), &s->mmio);

    sysbus_init_irq(SYS_BUS_DEVICE(obj), &s->irq);
}

static void stm32f1xx_gpio_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);

    dc->reset = stm32f1xx_gpio_reset;
    dc->vmsd = &vmstate_stm32f1xx_gpio;
}

static const TypeInfo stm32f1xx_gpio_info = {
    .name          = TYPE_STM32F1XX_GPIO,
    .parent        = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(STM32F1XXGPIOState),
    .instance_init = stm32f1xx_gpio_init,
    .class_init    = stm32f1xx_gpio_class_init,
};

static void stm32f1xx_gpio_register_types(void)
{
    type_register_static(&stm32f1xx_gpio_info);
}

type_init(stm32f1xx_gpio_register_types)
