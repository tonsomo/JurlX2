#ifdef GP2X
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <unistd.h>
#include <stropts.h>

#include "gp2x_joystick.h"

extern int errno;
unsigned long  *memregs32;
unsigned short *memregs16;
int memfd;

void *trymmap (void *start, size_t length, int prot, int flags, int fd, off_t offset)
{
	char *p;
	int aa;

	printf ("mmap(%X, %X, %X, %X, %X, %X) ... ", (unsigned int)start, length, prot, flags, fd, (unsigned int)offset);
	p = (char *)mmap (start, length, prot, flags, fd, offset);
	if (p == (char *)0xFFFFFFFF)
	{
		aa = errno;
		printf ("failed. errno = %d\n", aa);
	}
	else
	{
		printf ("OK! (%X)\n", (unsigned int)p);
	}

	return p;
}

unsigned char initphys (void)
{
	memfd = open("/dev/mem", O_RDWR);
	if (memfd == -1)
	{
		printf ("Open failed\n");
		return 0;
	}

	printf ("/dev/mem opened successfully - fd = %d\n", memfd);

	memregs32 = (unsigned long *)trymmap(0, 0x10000, PROT_READ, MAP_SHARED, memfd, 0xc0000000);
	if (memregs32 == (unsigned long *)0xFFFFFFFF) return 0;

	memregs16 = (unsigned short *)memregs32;

	return 1;
}

void closephys (void)
{
	close (memfd);
}

unsigned short lastjoy = 0;
unsigned short lastbutton = 0;
unsigned short lastvolume = 0;

bool gp2x_JoystickPressed(int aButton) {
    unsigned short joy;

    joy = ~memregs16[GPIOMPINLVL >> 1];
    if (joy != lastjoy) {
      printf("joystick: %04X, aButton: %04X\n", joy, aButton);
      lastjoy = joy;
    }
    
    if (aButton == gp2x_JoyLeft) { 
        if (joy == 0xFE02) return true;
        if (joy == 0xFE04) return true;
        if (joy == 0xFE06) return true;
        if (joy == 0xFE08) return true;
        if (joy == 0xFE0C) return true;
    }

    if (aButton == gp2x_JoyUp) { 
        if (joy == 0xFE01) return true;
        if (joy == 0xFE02) return true;
        if (joy == 0xFE03) return true;
        if (joy == 0xFE80) return true;
        if (joy == 0xFE81) return true;
    }
            
    if (aButton == gp2x_JoyRight) { 
      if (joy == 0xFE20) return true;
      if (joy == 0xFE40) return true;
      if (joy == 0xFE60) return true;
      if (joy == 0xFE80) return true;
      if (joy == 0xFEC0) return true;
    }
             
    if (aButton == gp2x_JoyDown) { 
        if (joy == 0xFE10) return true;
        if (joy == 0xFE20) return true;
        if (joy == 0xFE30) return true;
        if (joy == 0xFE08) return true;
        if (joy == 0xFE18) return true;
    }
    
    return false;
}

bool gp2x_ButtonPressed(int aButton) {
    unsigned short button;
    
    button = ~memregs16[GPIOCPINLVL >> 1];
    if (button != lastbutton) {
      printf("Button: %04X, aButton: %04X\n", button, aButton);
      lastbutton = button;
    }

    switch (aButton) {
        case gp2x_Start   : return (button == 0x0100); break;
        case gp2x_Select  : return (button == 0x0200); break;
        case gp2x_LButton : return (button == 0x0400); break;
        case gp2x_RButton : return (button == 0x0800); break;
        case gp2x_A       : return (button == 0x1000); break;
        case gp2x_B       : return (button == 0x2000); break;
        case gp2x_X       : return (button == 0x4000); break;
        case gp2x_Y       : return (button == 0x8000); break;
        default: return 0;
    }
}

bool gp2x_VolumePressed(int aButton) {
    unsigned short button;
    
    button = ~memregs16[GPIODPINLVL >> 1];
    if (button != lastvolume) {
      printf("Button: %04X, aButton: %04X\n", button, aButton);
      lastvolume = button;
    }

    switch (aButton) {
        case gp2x_VolMinus: return (button == 0xC040); break;
        case gp2x_VolPlus : return (button == 0xC080); break;
        default: return 0;
    }
}
#else
unsigned char initphys(void) { return 1; }

void closephys() { }

int gp2x_JoystickPressed(int aButton) { return 0;}
int gp2x_ButtonPressed(int aButton) { return 0; }
int gp2x_VolumePressed(int aButton) { return 0; }
#endif
