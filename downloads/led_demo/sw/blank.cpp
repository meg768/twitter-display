#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <memory.h>

void write16 (uint16_t addr, uint16_t data);

int fd = 0;

//PRE R1.0 MAPPING
//#define FPGA_PANEL_ADDR_REG 0x0010
//#define FPGA_PANEL_DATA_REG 0x0012
//#define FPGA_PANEL_BUFFER_REG 0x0014

//R1.0 MAPPING
#define FPGA_PANEL_ADDR_REG 0x0008
#define FPGA_PANEL_DATA_REG 0x0009
#define FPGA_PANEL_BUFFER_REG 0x000A


int main (int argc, char *argv[])
{
    // open fpga memory device
    fd = open ("/dev/logibone_mem", O_RDWR | O_SYNC);

    // set address to buffer 0
    write16 (FPGA_PANEL_ADDR_REG, 0x0000);

    // display buffer 0
    write16 (FPGA_PANEL_BUFFER_REG, 0x0000);

    // fill buffer 0 with black
    for (int row = 0; row < 32; row++) {
        for (int col = 0; col < 32; col++) {
            write16 (FPGA_PANEL_DATA_REG, 0x0000);
        }
    }
    
    // close fpga device
    close (fd);

    return 0;
}


void write16 (uint16_t addr, uint16_t data)
{
    pwrite (fd, &data, 2, addr);
}
