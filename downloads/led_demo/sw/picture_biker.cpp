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

#include "gammalut.h"

//PRE R1.0 MAPPING
//#define FPGA_PANEL_ADDR_REG 0x0010
//#define FPGA_PANEL_DATA_REG 0x0012
//#define FPGA_PANEL_BUFFER_REG 0x0014

//R1.0 MAPPING
#define FPGA_PANEL_ADDR_REG 0x0008
#define FPGA_PANEL_DATA_REG 0x0009
#define FPGA_PANEL_BUFFER_REG 0x000A

//char [] pictureFile = "beagleboard_logo.raw"

void write16 (uint16_t addr, uint16_t data);

int fd = 0;

int main (int argc, char *argv[])
{
    // open fpga memory device
    fd = open ("/dev/logibone_mem", O_RDWR | O_SYNC);

    // open raw image data
    //FILE *fin = fopen ("beagleboard_logo.raw", "rb");
	//FILE *fin = fopen ("~/glen/sw/beagleboard_logo.raw", "rb");
	FILE *fin = fopen ("/home/ubuntu/led_demo/sw/biker-icon.raw", "rb");

    // set address to buffer 0
    write16 (FPGA_PANEL_ADDR_REG, 0x0000);

    // display buffer 0
    write16 (FPGA_PANEL_BUFFER_REG, 0x0000);

    // read image data from file and write to display
    for (int row = 0; row < 32; row++) {
        for (int col = 0; col < 32; col++) {
            uint8_t r = fgetc (fin);
            uint8_t g = fgetc (fin);
            uint8_t b = fgetc (fin);
            r = gammaLut[r];
            g = gammaLut[g];
            b = gammaLut[b];
            uint16_t data = (r<<8) | (g<<4) | b;
            write16 (FPGA_PANEL_DATA_REG, data);
        }
    }
    
    // close image file
    fclose (fin);

    // close fpga device
    close (fd);

    return 0;
}


void write16 (uint16_t addr, uint16_t data)
{
    pwrite (fd, &data, 2, addr);
}
