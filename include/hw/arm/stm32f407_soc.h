#ifndef STM32F407_SOC_H
#define STM32F407_SOC_H
 
#include "hw/or-irq.h"
#include "hw/arm/armv7m.h"
 
#define TYPE_STM32F407_SOC "stm32f407-soc"
#define STM32F407_SOC(obj) \
    OBJECT_CHECK(STM32F407State, (obj), TYPE_STM32F407_SOC)
 
 
typedef struct STM32F407State {
    /*< private >*/
    SysBusDevice parent_obj;
    /*< public >*/
 
    char *kernel_filename;
    char *cpu_type;
    ARMv7MState armv7m;
 
} STM32F407State;
 
#endif