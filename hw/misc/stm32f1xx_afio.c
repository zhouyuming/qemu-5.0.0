/*
 * Copyright (c) 2025 liang yan <yanl1229@163.com>
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
#include "qemu/osdep.h"
#include "qapi/error.h"
#include "qemu/log.h"
#include "hw/misc/stm32f1xx_afio.h"
#include "hw/irq.h"
#include "hw/qdev-properties.h"
#include "migration/vmstate.h"
#include "qemu/module.h"

#ifndef STM_AFIO_ERR_DEBUG
#define STM_AFIO_ERR_DEBUG 0
#endif

#define DB_PRINT_L(lvl, fmt, args...) do { \
    if (STM_AFIO_ERR_DEBUG >= lvl) { \
        qemu_log("%s: " fmt, __func__, ## args); \
    } \
} while (0);

#define DB_PRINT(fmt, args...) DB_PRINT_L(1, fmt, ## args)

static void stm32f1xx_afio_reset(DeviceState *dev)
{
    STM32F1XXAFIOState *s = STM32F1XX_AFIO(dev);
    s->afio_evcr     = 0x00000000;
    s->afio_mapr     = 0x00000000;
    s->afio_exticr1  = 0x00000000;
    s->afio_exticr2  = 0x00000000;
    s->afio_exticr3  = 0x00000000;
    s->afio_exticr4  = 0x00000000;
    s->afio_mapr2    = 0x00000000;
}

static uint64_t stm32f1xx_afio_read(void *opaque, hwaddr addr,
                                     unsigned int size)
{
    STM32F1XXAFIOState *s = opaque;
    uint64_t retvalue = 0;

    DB_PRINT("Address: 0x%" HWADDR_PRIx "\n", addr);
    switch(addr) {
    case STM_AFIO_EVCR:
        retvalue = s->afio_evcr;
        break;
    case STM_AFIO_MAPR:
        retvalue = s->afio_mapr;
        break;
    case STM_AFIO_EXTICR1:
        retvalue = s->afio_exticr1;
        break;
    case STM_AFIO_EXTICR2:
        retvalue = s->afio_exticr2;
        break;
    case STM_AFIO_EXTICR3:
        retvalue = s->afio_exticr3;
        break;
    case STM_AFIO_EXTICR4:
        retvalue = s->afio_exticr4;
        break;
    case STM_AFIO_MAPR2:
        retvalue = s->afio_mapr2;
        break;
    default:
        qemu_log_mask(LOG_GUEST_ERROR, "%s: Bad offset 0x%" HWADDR_PRIx "\n",
            __func__, addr);
        retvalue = 0;
        break;
    }
    return retvalue;
}

static void stm32f1xx_afio_write(void *opaque, hwaddr addr,
                                uint64_t val64, unsigned int size)
{
    STM32F1XXAFIOState *s = opaque;
    uint32_t value = val64;

    DB_PRINT("Address: 0x%" HWADDR_PRIx ", Value: 0x%x\n", addr, value);
    switch(addr) {
    case STM_AFIO_EVCR:
        s->afio_evcr = value;
        break;
    case STM_AFIO_MAPR:
        s->afio_mapr = value;
        break;
    case STM_AFIO_EXTICR1:
        s->afio_exticr1 = value;
        break;
    case STM_AFIO_EXTICR2:
        s->afio_exticr2 = value;
        break;
    case STM_AFIO_EXTICR3:
        s->afio_exticr3 = value;
        break;
    case STM_AFIO_EXTICR4:
        s->afio_exticr4 = value;
        break;
    case STM_AFIO_MAPR2:
        s->afio_mapr2 = value;
        break;
    default:
        qemu_log_mask(LOG_GUEST_ERROR, "%s: Bad offset 0x%" HWADDR_PRIx "\n",
            __func__, addr);
    }
}

static const MemoryRegionOps stm32f1xx_afio_ops = {
    .read = stm32f1xx_afio_read,
    .write = stm32f1xx_afio_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
};

static const VMStateDescription vmstate_stm32f1xx_afio = {
    .name = TYPE_STM32F1XX_AFIO,
    .version_id = 1,
    .minimum_version_id = 1,
    .fields = (VMStateField[]) {
        VMSTATE_UINT32(afio_evcr, STM32F1XXAFIOState),
        VMSTATE_UINT32(afio_mapr, STM32F1XXAFIOState),
        VMSTATE_UINT32(afio_exticr1, STM32F1XXAFIOState),
        VMSTATE_UINT32(afio_exticr2, STM32F1XXAFIOState),
        VMSTATE_UINT32(afio_exticr3, STM32F1XXAFIOState),
        VMSTATE_UINT32(afio_exticr4, STM32F1XXAFIOState),
        VMSTATE_UINT32(afio_mapr2, STM32F1XXAFIOState),
        VMSTATE_END_OF_LIST()
    }
};

static void stm32f1xx_afio_init(Object *obj)
{
    STM32F1XXAFIOState *s = STM32F1XX_AFIO(obj);

    memory_region_init_io(&s->mmio, obj, &stm32f1xx_afio_ops, s,
                          TYPE_STM32F1XX_AFIO, 0x400);
    sysbus_init_mmio(SYS_BUS_DEVICE(obj), &s->mmio);

    sysbus_init_irq(SYS_BUS_DEVICE(obj), &s->irq);
}

static void stm32f1xx_afio_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);

    dc->reset = stm32f1xx_afio_reset;
    dc->vmsd = &vmstate_stm32f1xx_afio;
}

static const TypeInfo stm32f1xx_afio_info = {
    .name          = TYPE_STM32F1XX_AFIO,
    .parent        = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(STM32F1XXAFIOState),
    .instance_init = stm32f1xx_afio_init,
    .class_init    = stm32f1xx_afio_class_init,
};

static void stm32f1xx_afio_register_types(void)
{
    type_register_static(&stm32f1xx_afio_info);
}

type_init(stm32f1xx_afio_register_types)
