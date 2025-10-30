#include "qemu/osdep.h"
#include "qapi/error.h"
#include "hw/boards.h"
#include "qemu/error-report.h"
#include "exec/address-spaces.h"
#include "hw/arm/stm32f407_soc.h"
#include "hw/qdev-properties.h"
#include "hw/arm/boot.h"
 
static void disco_init(MachineState *machine)
{
    DeviceState *dev;
    MemoryRegion *system_memory = get_system_memory();
    MemoryRegion *sram = g_new(MemoryRegion, 1);
    MemoryRegion *flash = g_new(MemoryRegion, 1);
    MemoryRegion *flash_alias = g_new(MemoryRegion, 1);
    MemoryRegion *ccm = g_new(MemoryRegion, 1);
 
    memory_region_init_ram(flash, NULL, "STM32F407.flash", FLASH_SIZE,
                           &error_fatal);
    memory_region_init_alias(flash_alias, NULL, "STM32F407.flash.alias",
                             flash, 0, FLASH_SIZE);
 
    memory_region_add_subregion(system_memory, FLASH_BASE_ADDRESS, flash);
    memory_region_add_subregion(system_memory, 0, flash_alias);
 
    memory_region_init_ram(sram, NULL, "STM32F407.sram", SRAM_SIZE,
                           &error_fatal);
    memory_region_add_subregion(system_memory, SRAM_BASE_ADDRESS, sram);
 
    memory_region_init_ram(ccm, NULL, "STM32F407.ccm", CCM_SIZE,
                          &error_fatal);
    memory_region_add_subregion(system_memory, CCM_BASE_ADDR, ccm);
 
    dev = qdev_create(NULL, TYPE_STM32F407_SOC);
 
    qdev_prop_set_string(dev, "cpu-type", ARM_CPU_TYPE_NAME("cortex-m4"));
    object_property_set_bool(OBJECT(dev), true, "realized", &error_fatal);
 
    armv7m_load_kernel(ARM_CPU(first_cpu), machine->kernel_filename,
                       FLASH_SIZE);
}
 
static void disco_machine_init(MachineClass *mc)
{
    static const char * const valid_cpu_types[] = {
        ARM_CPU_TYPE_NAME("cortex-m4"),
        NULL
    };
 
    mc->desc = "Discovery EVK Machine (STM32F407 Soc)";
    mc->init = disco_init;
    mc->valid_cpu_types = valid_cpu_types;
}
 
DEFINE_MACHINE("stm32f407-st-disco", disco_machine_init)
