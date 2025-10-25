/*
 * Copyright (c) 2025 liang yan <yanl1229@163.com>
 *
 * STM32F103 OpenMCU Board
 *
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
#include "hw/boards.h"
#include "qemu/error-report.h"
#include "exec/address-spaces.h"
#include "hw/arm/stm32f103_soc.h"
#include "hw/qdev-properties.h"
#include "hw/arm/boot.h"

static void openmcu_init(MachineState *machine)
{
    DeviceState *dev;
    MemoryRegion *system_memory = get_system_memory();
    MemoryRegion *sram = g_new(MemoryRegion, 1);
    MemoryRegion *flash = g_new(MemoryRegion, 1);
    MemoryRegion *flash_alias = g_new(MemoryRegion, 1);

    memory_region_init_ram(flash, NULL, "STM32F103.flash", FLASH_SIZE,
                           &error_fatal);
    memory_region_init_alias(flash_alias, NULL, "STM32F103.flash.alias",
                             flash, 0, FLASH_SIZE);

    memory_region_add_subregion(system_memory, FLASH_BASE_ADDRESS, flash);
    memory_region_add_subregion(system_memory, 0, flash_alias);

    memory_region_init_ram(sram, NULL, "STM32F103.sram", SRAM_SIZE,
                           &error_fatal);
    memory_region_add_subregion(system_memory, SRAM_BASE_ADDRESS, sram);

    dev = qdev_create(NULL, TYPE_STM32F103_SOC);

    qdev_prop_set_string(dev, "cpu-type", ARM_CPU_TYPE_NAME("cortex-m3"));
    object_property_set_bool(OBJECT(dev), true, "realized", &error_fatal);

    armv7m_load_kernel(ARM_CPU(first_cpu), machine->kernel_filename,
                       FLASH_SIZE);
}

static void openmcu_machine_init(MachineClass *mc)
{
    mc->desc = "Open MCU EVK Board (STM32F103 Soc)";
    mc->init = openmcu_init;
}

DEFINE_MACHINE("stm32f103-openmcu", openmcu_machine_init)
