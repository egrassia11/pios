#define MU_IO_REG ((unsigned int*)0x3F215040)

void putc(int data) {
    *MU_IO_REG = (unsigned int)data;
}

