/*
 * Copyright (c) 2020 liang yan <yanl1229@163.com>
 *
 * STM32F4XX Timer
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
#ifndef HW_STM32F4XX_TIMER_H
#define HW_STM32F4XX_TIMER_H

#include "hw/sysbus.h"
#include "qemu/timer.h"
#include "sysemu/sysemu.h"

#define TIM_CR1      0x00
#define TIM_CR2      0x04
#define TIM_SMCR     0x08
#define TIM_DIER     0x0C
#define TIM_SR       0x10
#define TIM_EGR      0x14
#define TIM_CCMR1    0x18
#define TIM_CCMR2    0x1C
#define TIM_CCER     0x20
#define TIM_CNT      0x24
#define TIM_PSC      0x28
#define TIM_ARR      0x2C
#define TIM_CCR1     0x34
#define TIM_CCR2     0x38
#define TIM_CCR3     0x3C
#define TIM_CCR4     0x40
#define TIM_DCR      0x48
#define TIM_DMAR     0x4C
#define TIM_OR       0x50

#define TIM_CR1_CEN   (1 << 0)
#define TIM_CR1_URS   (1 << 2)
#define TIM_CR1_CMS   (3 << 6)
#define TIM_CR1_DIR   (1 << 4)

#define TIM_EGR_UG 1

#define TIM_CCER_CC2E   (1 << 4)
#define TIM_CCMR1_OC2M2 (1 << 14)
#define TIM_CCMR1_OC2M1 (1 << 13)
#define TIM_CCMR1_OC2M0 (1 << 12)
#define TIM_CCMR1_OC2PE (1 << 11)

#define TIM_DIER_UIE    (1 << 0)
#define TIM_DIER_CC1IE  (1 << 1)
#define TIM_DIER_CC2IE  (1 << 2)
#define TIM_DIER_CC3IE  (1 << 3)
#define TIM_DIER_CC4IE  (1 << 4)

#define TIM_SR_UIF      (1 << 0)

#define TYPE_STM32F4XX_TIMER "stm32f4xx-timer"
#define STM32F4XXTIMER(obj) OBJECT_CHECK(STM32F4XXTimerState, \
                            (obj), TYPE_STM32F4XX_TIMER)

typedef struct STM32F4XXTimerState {
    /* <private> */
    SysBusDevice parent_obj;

    /* <public> */
    MemoryRegion iomem;
    QEMUTimer *timer;
    qemu_irq irq;

    int64_t tick_offset;
    uint64_t hit_time;
    uint64_t freq_hz;

    uint32_t tim_cr1;
    uint32_t tim_cr2;
    uint32_t tim_smcr;
    uint32_t tim_dier;
    uint32_t tim_sr;
    uint32_t tim_egr;
    uint32_t tim_ccmr1;
    uint32_t tim_ccmr2;
    uint32_t tim_ccer;
    uint32_t tim_cnt;
    uint32_t tim_psc;
    uint32_t tim_arr;
    uint32_t tim_ccr1;
    uint32_t tim_ccr2;
    uint32_t tim_ccr3;
    uint32_t tim_ccr4;
    uint32_t tim_dcr;
    uint32_t tim_dmar;
    uint32_t tim_or;
} STM32F4XXTimerState;

#endif
