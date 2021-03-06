/* See COPYRIGHT for copyright information. */

#include <inc/x86.h>
#include <kern/kclock.h>
#include <kern/trap.h>
#include <kern/picirq.h>

/* HINT: Note that selected CMOS
 * register is reset to the first one
 * after first access, i.e. it needs to be selected
 * on every access.
 *
 * Don't forget to disable NMI for the time of
 * operation (look up for the appropriate constant in kern/kclock.h)
 *
 * Why it is necessary?
 */

uint8_t
cmos_read8(uint8_t reg) {
    /* MC146818A controller */
    // LAB 4: Your code here

    uint8_t res = 0;
    nmi_disable();

    outb(CMOS_CMD, CMOS_NMI_LOCK | reg);
    res = inb(CMOS_DATA);

    nmi_enable();
    return res;
}

void
cmos_write8(uint8_t reg, uint8_t value) {
    nmi_disable();

    outb(CMOS_CMD, CMOS_NMI_LOCK | reg);
    outb(CMOS_DATA, CMOS_NMI_LOCK | value);

    nmi_enable();
}

uint16_t
cmos_read16(uint8_t reg) {
    return cmos_read8(reg) | (cmos_read8(reg + 1) << 8);
}

void
rtc_timer_pic_interrupt(void) {
    // LAB 4: Your code here
    // Enable PIC interrupts.

    pic_irq_mask(IRQ_CLOCK);
}

void
rtc_timer_pic_handle(void) {
    rtc_check_status();
    pic_send_eoi(IRQ_CLOCK);
}

void
rtc_timer_init(void) {
    // LAB 4: Your code here
    // (use cmos_read8/cmos_write8)

    outb(CMOS_CMD, RTC_BREG);
    uint8_t reg = cmos_read8(RTC_BREG);
    reg = reg | RTC_PIE;
    cmos_write8(RTC_BREG, reg);

    outb(CMOS_CMD, RTC_AREG);
    reg = cmos_read8(RTC_AREG);
    reg = reg | 0xD;
    cmos_write8(RTC_AREG, reg);
}

uint8_t
rtc_check_status(void) {
    // LAB 4: Your code here
    // (use cmos_read8)
    return cmos_read8(RTC_CREG);
}
