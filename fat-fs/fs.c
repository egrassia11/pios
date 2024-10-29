#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include "fat.h"

// File descriptor
int fd = 0;

char sector_buf[512];

void init() {
    fd = open("disk.img", O_RDONLY);

    if (fd < 0) {
        perror("open");
        fprintf(stderr, "open() failed\n");
        exit(-1);
    }
}

void readSector(uint64_t sector, char * buf) {
    off_t o = lseek(fd, 512 * sector, SEEK_SET);
    size_t n = read(fd, buf, 512);

}

int main() {
    struct boot_sector *bs = (struct boot_sector*)sector_buf;
    struct root_directory_entry *rde = (struct root_directory_entry*) rde_region;
    init();
    readSector(0, sector_buf);

    for (int k = 0; k < 16; k++) {
        printf("%0x ", sector_buf[k]);
    }

    printf("\n");
    printf("Bytes per sector: %d\n", bs->bytes_per_sector);
    printf("Sectors per cluster: %d\n", bs->num_sectors_per_cluster);
    printf("Reserved sectors: %d\n", bs->num_reserved_sectors);
    printf("Number of FATs: %d\n", bs->num_fat_tables);
    printf("Number of RDEs: %d\n", bs->num_root_dir_entries);

    readSector(bs->num_reserved_sectors + bs->num_fat_tables * bs->num_sectors_per_cluster, rde_region);

    for (int k = 0; k< 8; k++) {
        printf("Name of file: %d is \ "%s\ "\n", k, rde[k].file_name);
    }

    for (int k = 0; k< 16; k++) {
        printf("%02x ", rde_region[k]);
    }

    return 0;
}

