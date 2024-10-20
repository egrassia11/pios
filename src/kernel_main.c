#include "rprintf.h"  // Include the correct header for esp_printf

extern void putc(int c);
unsigned int getEL();
void mmu_on();

unsigned long get_timer_count() {
    unsigned long *timer_count_register = (unsigned long *)0x3f003004;
    return *timer_count_register;
}

void wait_1ms() {
    unsigned long start_time = get_timer_count();
    unsigned long end_time = start_time + 1000;  // 1000 microseconds = 1 millisecond

    while (get_timer_count() < end_time) {
    }
}

void mmu_on();

void kernel_main() {
    esp_printf(putc, "Current Execution Level is %d\r\n", getEL());

    extern int __bss_start;
    extern int __bss_end;

    char *bss_start;
    char *bss_end;

    bss_start = (char *)&__bss_start;
    bss_end = (char *)&__bss_end;

    // Loop to clear the BSS segment
    while (bss_start < bss_end) {
        *bss_start = 0;
        bss_start++;
    }

    mmu_on();  // Enable MMU

    wait_1ms();  // Delay for 1 millisecond

    while (1) {
    }
}

unsigned int getEL() {
    unsigned int el;
    asm volatile ("mrs %0, CurrentEL" : "=r" (el));  // Read CurrentEL register
    return (el >> 2) & 3;  // Extract the execution level bits
}
