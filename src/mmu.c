#include "mmu.h"

struct table_descriptor_stage1 L1table[512] __attribute__((aligned(4096)));
struct page_descriptor_stage1 L2table[512] __attribute__((aligned(4096)));

/*
   38             30  29                 21  20                           0
  |------------------|----------------------|------------------------------|
  | L1 table entry   |  L2 table entry      | Offset                       |
  |------------------|----------------------|------------------------------|
*/

void mapPages(void *vaddr, void *paddr) {
    unsigned int L2tableIndex = ((unsigned long)vaddr >> 21) & 0x1ff;
    unsigned int L1tableIndex = ((unsigned long)vaddr >> 30) & 0x1ff;

    L1table[L1tableIndex].type = 3;
    L1table[L1tableIndex].next_lvl_table = ((unsigned long)&L2table[0]) >> 12;

    L2table[L2tableIndex].attrindx = 0; // Normal memory, not memory-mapped IO (1 for IO)
    L2table[L2tableIndex].type = 1;     // Pointing to memory page
    L2table[L2tableIndex].sh = 3;       // Set inner sharable
    L2table[L2tableIndex].ap = 0;       // Access permission, kernel RW
    L2table[L2tableIndex].af = 1;       // Access flag
    L2table[L2tableIndex].output_addr = (unsigned long)paddr >> 12;  // Shift by 12 bits
}

int loadPageTable(struct table_descriptor_stage1 *L1table) {
    unsigned long b, r;
    asm volatile ("mrs %0, id_aa64mmfr0_el1" : "=r" (r));
    b = r & 0xF;
    if (r & (0xF << 28) || b < 1) {
        return -1;
    }

    r = (0xFF << 0) |    // AttrIdx=0: normal, IWBWA, OWBWA, NTR
        (0x04 << 8) |    // AttrIdx=1: device, nGnRE (must be OSH too)
        (0x44 << 16);    // AttrIdx=2: non-cacheable
    asm volatile ("msr mair_el1, %0" : : "r" (r));

    r = (0b00LL << 37) | (b << 32) | (0b10LL << 30) | (0b11LL << 28) |
        (0b01LL << 26) | (0b01LL << 24) | (0b0LL << 23) | (25LL << 16) |
        (0b00LL << 14) | (0b11LL << 12) | (0b01LL << 10) | (0b01LL << 8) |
        (0b0LL << 7) | (25LL << 0);
    asm volatile ("msr tcr_el1, %0; isb" : : "r" (r));

    asm volatile ("msr ttbr0_el1, %0" : : "r" ((unsigned long)L1table));

    asm volatile ("dsb ish; isb; mrs %0, sctlr_el1" : "=r" (r));
    r |= 0xC00800;
    r &= ~((1 << 25) | (1 << 24) | (1 << 19) | (1 << 12) | (1 << 4) | (1 << 3) | (1 << 2) | (1 << 1));
    r |= (1 << 0);
    asm volatile ("msr sctlr_el1, %0; isb" : : "r" (r));
    return 0;
}
