char global[128];

void kernel_main() {
    extern int __bss_start;
    extern int __bss_end;

    char *bss_start;
    char *bss_end;

    bss_start = &__bss_start;
    bss_end = &__bss_end;

    // Loop to clear the BSS segment
    while (bss_start < bss_end) {
        *bss_start = 0;
        bss_start++;
    }

    while(1){
    }
}
