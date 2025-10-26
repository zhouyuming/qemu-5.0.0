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
#include "hw/arm/stm32f407_soc.h"
#include "sysemu/sysemu.h"

static const uint32_t timer_addr[STM_NUM_TIMERS] = {
    STM32F407_TIM2, STM32F407_TIM3, STM32F407_TIM4,
    STM32F407_TIM5
};

static const uint32_t usart_addr[STM_NUM_USARTS] = {
    STM32F407_USART1, STM32F407_USART2, STM32F407_USART3,
    STM32F407_USART6
};

static const uint32_t gpio_addr[STM_NUM_GPIOS] = {
    STM_GPIO_PORTA, STM_GPIO_PORTB, STM_GPIO_PORTC, STM_GPIO_PORTD,
    STM_GPIO_PORTE, STM_GPIO_PORTF, STM_GPIO_PORTG, STM_GPIO_PORTH,
    STM_GPIO_PORTI, STM_GPIO_PORTJ, STM_GPIO_PORTK
};

static const int usart_irq[STM_NUM_USARTS] = {
    37, 38, 39, 71
};

static const int exti_irq[] =
{
    6, 7, 8, 9, 10, 23, 23, 23, 23, 23, 40,
    40, 40, 40, 40, 40
};

static const int timer_irq[STM_NUM_TIMERS] = {
    28, 29, 30, 50
};
 
static void stm32f407_soc_initfn(Object *obj)
{
 
    STM32F407State *s = STM32F407_SOC(obj);
    int i;
 
    sysbus_init_child_obj(obj, "armv7m", &s->armv7m, sizeof(s->armv7m),
                          TYPE_ARMV7M);
    sysbus_init_child_obj(obj, "syscfg", &s->syscfg, sizeof(s->syscfg),
                          TYPE_STM32F4XX_SYSCFG);
    sysbus_init_child_obj(obj, "exti", &s->exti, sizeof(s->exti),
                           TYPE_STM32F4XX_EXTI);
    sysbus_init_child_obj(obj, "flash", &s->flash, sizeof(s->flash),
                        TYPE_STM32F4XX_FLASH);

    object_initialize(&s->rcc, sizeof(s->rcc), TYPE_STM32F4XX_RCC);
    qdev_set_parent_bus(DEVICE(&s->rcc), sysbus_get_default());

    object_initialize(&s->power, sizeof(s->power), TYPE_STM32F4XX_POWER);
    qdev_set_parent_bus(DEVICE(&s->power), sysbus_get_default());

    for (i = 0; i < STM_NUM_USARTS; i++) {
        object_initialize(&s->usart[i], sizeof(s->usart[i]), TYPE_STM32F4XX_USART);
        qdev_set_parent_bus(DEVICE(&s->usart[i]), sysbus_get_default());
    }

    for (i = 0; i < STM_NUM_GPIOS; i++) {
        object_initialize(&s->gpio[i], sizeof(s->gpio[i]),
                          TYPE_STM32F4XX_GPIO);
        qdev_set_parent_bus(DEVICE(&s->gpio[i]), sysbus_get_default());
    }

    for (i = 0; i < STM_NUM_TIMERS; i++) {
        object_initialize(&s->timer[i], sizeof(s->timer[i]), TYPE_STM32F4XX_TIMER);
        qdev_set_parent_bus(DEVICE(&s->timer[i]), sysbus_get_default());
    }
}
 
static void stm32f407_soc_realize(DeviceState *dev_soc, Error **errp)
{
    STM32F407State *s = STM32F407_SOC(dev_soc);
    DeviceState *dev, *armv7m/*, *touch*/;
    SysBusDevice *busdev;
    Error *err = NULL;
    int i/*, j*/;
    // I2CBus *i2c;
    // SSIBus *spi_bus;
 
    armv7m = DEVICE(&s->armv7m);
    qdev_prop_set_uint32(armv7m, "num-irq", 98);
    qdev_prop_set_string(armv7m, "cpu-type", s->cpu_type);
    qdev_prop_set_bit(armv7m, "enable-bitband", true);
    object_property_set_link(OBJECT(&s->armv7m), OBJECT(get_system_memory()),
                                     "memory", &error_abort);
    object_property_set_bool(OBJECT(&s->armv7m), true, "realized", &err);
    if (err != NULL) {
        error_propagate(errp, err);
        return;
    }

    /* System configuration controller */
    dev = DEVICE(&s->syscfg);
    object_property_set_bool(OBJECT(&s->syscfg), true, "realized", &err);
    if (err != NULL) {
        error_propagate(errp, err);
        return;
    }
    busdev = SYS_BUS_DEVICE(dev);
    sysbus_mmio_map(busdev, 0, SYSCFG_BASE_ADDRESS);
    sysbus_connect_irq(busdev, 0, qdev_get_gpio_in(armv7m, SYSCFG_IRQ));

    /* EXTI Controller */
    dev = DEVICE(&s->exti);
    object_property_set_bool(OBJECT(&s->exti), true, "realized", &err);
    if (err != NULL) {
        error_propagate(errp, err);
        return;
    }
    busdev = SYS_BUS_DEVICE(dev);
    sysbus_mmio_map(busdev, 0, EXIT_BASE_ADDRESS);
    for (i = 0; i < 16; i++) {
        sysbus_connect_irq(busdev, i, qdev_get_gpio_in(armv7m, exti_irq[i]));
    }
    for (i = 0; i < 16; i++) {
        qdev_connect_gpio_out(DEVICE(&s->syscfg), i, qdev_get_gpio_in(dev, i));
    }

    /* System Power */
    dev = DEVICE(&s->power);
    object_property_set_bool(OBJECT(&s->power), true, "realized", &err);
    if (err != NULL) {
        error_propagate(errp, err);
        return;
    }
    busdev = SYS_BUS_DEVICE(dev);
    sysbus_mmio_map(busdev, 0, POWER_BASE_ADDR);

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

    /* GPIO A to K */
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

    /* Timer 2 to 5 contoller */
    for (i = 0; i < STM_NUM_TIMERS; i++) {
        dev = DEVICE(&(s->timer[i]));
        qdev_prop_set_uint64(dev, "clock-frequency", 1000000000);
        object_property_set_bool(OBJECT(&s->timer[i]), true, "realized", &err);
        if (err != NULL) {
            error_propagate(errp, err);
            return;
        }
        busdev = SYS_BUS_DEVICE(dev);
        sysbus_mmio_map(busdev, 0, timer_addr[i]);
        sysbus_connect_irq(busdev, 0, qdev_get_gpio_in(armv7m, timer_irq[i]));
    }
}
 
static Property stm32f407_soc_properties[] = {
    DEFINE_PROP_STRING("kernel-filename", STM32F407State, kernel_filename),
    DEFINE_PROP_STRING("cpu-type", STM32F407State, cpu_type),
    DEFINE_PROP_END_OF_LIST(),
};
 
static void stm32f407_soc_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
 
    dc->realize = stm32f407_soc_realize;
    // dc->props = stm32f407_soc_properties;
    device_class_set_props(dc, stm32f407_soc_properties);
}
 
static const TypeInfo stm32f407_soc_info = {
    .name          = TYPE_STM32F407_SOC,
    .parent        = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(STM32F407State),
    .instance_init = stm32f407_soc_initfn,
    .class_init    = stm32f407_soc_class_init,
};
 
static void stm32f407_soc_types(void)
{
    type_register_static(&stm32f407_soc_info);
}
 
type_init(stm32f407_soc_types)
