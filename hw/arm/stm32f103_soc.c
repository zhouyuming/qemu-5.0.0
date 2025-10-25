/*
 * Copyright (c) 2025 liang yan <yanl1229@163.com>
 *
 * STM32F103 SoC
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
#include "qemu-common.h"
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/arm/boot.h"
#include "hw/qdev-properties.h"
#include "sysemu/sysemu.h"
#include "migration/vmstate.h"
#include "exec/address-spaces.h"
#include "hw/arm/stm32f103_soc.h"
#include "sysemu/sysemu.h"

static const uint32_t usart_addr[STM_NUM_USARTS] = {
    UART1_BASE_ADDR, UART2_BASE_ADDR, UART3_BASE_ADDR,
};

static const uint32_t gpio_addr[STM_NUM_GPIOS] = {
    STM_GPIO_PORTA, STM_GPIO_PORTB, STM_GPIO_PORTC, STM_GPIO_PORTD,
    STM_GPIO_PORTE, STM_GPIO_PORTF, STM_GPIO_PORTG
};

static const int usart_irq[STM_NUM_USARTS] = {
    37, 38, 39
};

static void stm32f103_soc_initfn(Object *obj)
{
    int i;

    STM32F103State *s = STM32F103_SOC(obj);

    sysbus_init_child_obj(obj, "armv7m", &s->armv7m, sizeof(s->armv7m),
                          TYPE_ARMV7M);

    object_initialize(&s->rcc, sizeof(s->rcc), TYPE_STM32F1XX_RCC);
    qdev_set_parent_bus(DEVICE(&s->rcc), sysbus_get_default());

    sysbus_init_child_obj(obj, "afio", &s->afio, sizeof(s->afio),
                        TYPE_STM32F1XX_AFIO);

    for (i = 0; i < STM_NUM_USARTS; i++) {
        object_initialize(&s->usart[i], sizeof(s->usart[i]), TYPE_STM32F1XX_USART);
        qdev_set_parent_bus(DEVICE(&s->usart[i]), sysbus_get_default());
    }

    sysbus_init_child_obj(obj, "pwr", &s->pwr, sizeof(s->pwr),
                         TYPE_STM32F1XX_POWER);

    sysbus_init_child_obj(obj, "flash", &s->flash, sizeof(s->flash),
                        TYPE_STM32F1XX_FLASH);

    for (i = 0; i < STM_NUM_GPIOS; i++) {
        object_initialize(&s->gpio[i], sizeof(s->gpio[i]),
                          TYPE_STM32F1XX_GPIO);
        qdev_set_parent_bus(DEVICE(&s->gpio[i]), sysbus_get_default());
    }
}

static void stm32f103_soc_realize(DeviceState *dev_soc, Error **errp)
{
    STM32F103State *s = STM32F103_SOC(dev_soc);
    DeviceState *dev, *armv7m;
    SysBusDevice *busdev;
    Error *err = NULL;
    int i;

    armv7m = DEVICE(&s->armv7m);
    qdev_prop_set_uint32(armv7m, "num-irq", 68);
    qdev_prop_set_string(armv7m, "cpu-type", s->cpu_type);
    qdev_prop_set_bit(armv7m, "enable-bitband", true);
    object_property_set_link(OBJECT(&s->armv7m), OBJECT(get_system_memory()),
                                     "memory", &error_abort);
    object_property_set_bool(OBJECT(&s->armv7m), true, "realized", &err);
    if (err != NULL) {
        error_propagate(errp, err);
        return;
    }

    /* System rcc controller */
    dev = DEVICE(&s->rcc);
    object_property_set_bool(OBJECT(&s->rcc), true, "realized", &err);
    if (err != NULL) {
        error_propagate(errp, err);
        return;
    }
    busdev = SYS_BUS_DEVICE(dev);
    sysbus_mmio_map(busdev, 0, RCC_BASE_ADDR);

    /* USART controllers */
    for (i = 0; i < STM_NUM_USARTS; i++) {
        dev = DEVICE(&(s->usart[i]));
        qdev_prop_set_chr(dev, "chardev", serial_hd(i));
        object_property_set_bool(OBJECT(&s->usart[i]), true, "realized", &err);
        if (err != NULL) {
            error_propagate(errp, err);
            return;
        }
        busdev = SYS_BUS_DEVICE(dev);
        sysbus_mmio_map(busdev, 0, usart_addr[i]);
        sysbus_connect_irq(busdev, 0, qdev_get_gpio_in(armv7m, usart_irq[i]));
    }

    /* Flash Controller */
    dev = DEVICE(&s->flash);
    object_property_set_bool(OBJECT(&s->flash), true, "realized", &err);
    if (err != NULL) {
        error_propagate(errp, err);
        return;
    }
    busdev = SYS_BUS_DEVICE(dev);
    sysbus_mmio_map(busdev, 0, FLASH_BASE_ADDR);

    /* GPIO A to G */
    for (i = 0; i < STM_NUM_GPIOS; i++) {
        dev = DEVICE(&(s->gpio[i]));
        object_property_set_bool(OBJECT(&s->gpio[i]), true, "realized", &err);
        if (err != NULL) {
            error_propagate(errp, err);
            return;
        }
        busdev = SYS_BUS_DEVICE(dev);
        sysbus_mmio_map(busdev, 0, gpio_addr[i]);
    }

    /* AFIO Controller */
    dev = DEVICE(&s->afio);
    object_property_set_bool(OBJECT(&s->afio), true, "realized", &err);
    if (err != NULL) {
        error_propagate(errp, err);
        return;
    }
    busdev = SYS_BUS_DEVICE(dev);
    sysbus_mmio_map(busdev, 0, STM_AFIO_MAPR2);
}

static Property stm32f103_soc_properties[] = {
    DEFINE_PROP_STRING("kernel-filename", STM32F103State, kernel_filename),
    DEFINE_PROP_STRING("cpu-type", STM32F103State, cpu_type),
    DEFINE_PROP_END_OF_LIST(),
};

static void stm32f103_soc_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);

    dc->realize = stm32f103_soc_realize;
    dc->props = stm32f103_soc_properties;
}

static const TypeInfo stm32f103_soc_info = {
    .name          = TYPE_STM32F103_SOC,
    .parent        = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(STM32F103State),
    .instance_init = stm32f103_soc_initfn,
    .class_init    = stm32f103_soc_class_init,
};

static void stm32f103_soc_types(void)
{
    type_register_static(&stm32f103_soc_info);
}

type_init(stm32f103_soc_types)
