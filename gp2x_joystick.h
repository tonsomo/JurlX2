//
//
//

#ifndef _gp2x_joystick_h_
#define _gp2x_joystick_h_

#define GPIOAPINLVL 0x1180
#define GPIOBPINLVL 0x1182
#define GPIOCPINLVL 0x1184
#define GPIODPINLVL 0x1186
#define GPIOEPINLVL 0x1188
#define GPIOFPINLVL 0x118A
#define GPIOGPINLVL 0x118C
#define GPIOHPINLVL 0x118E
#define GPIOIPINLVL 0x1190
#define GPIOJPINLVL 0x1192
#define GPIOKPINLVL 0x1194
#define GPIOLPINLVL 0x1196
#define GPIOMPINLVL 0x1198
#define GPIONPINLVL 0x119A
#define GPIOOPINLVL 0x119C

#define GPIOAOUT 0x1060
#define GPIOBOUT 0x1062
#define GPIOCOUT 0x1064
#define GPIODOUT 0x1066
#define GPIOEOUT 0x1068
#define GPIOFOUT 0x106A
#define GPIOGOUT 0x106C
#define GPIOHOUT 0x106E
#define GPIOIOUT 0x1060
#define GPIOJOUT 0x1062
#define GPIOKOUT 0x1064
#define GPIOLOUT 0x1066
#define GPIOMOUT 0x1068
#define GPIONOUT 0x106A
#define GPIOOOUT 0x106C

typedef enum {
	false = 0,
	true  = 1
} bool;

// Initialisiere joystick
unsigned char initphys (void);
void closephys (void);

// query button state
#define gp2x_A         1
#define gp2x_B         2
#define gp2x_Y         3
#define gp2x_X         4

#define gp2x_Select    5
#define gp2x_Start     6

#define gp2x_VolMinus  7
#define gp2x_VolPlus   8

#define gp2x_LButton   9
#define gp2x_RButton  10

#define gp2x_JoyLeft  20
#define gp2x_JoyUp    21
#define gp2x_JoyRight 22
#define gp2x_JoyDown  23
#define gp2x_Center   24

bool gp2x_JoystickPressed(int aButton);
bool gp2x_ButtonPressed(int aButton);
bool gp2x_VolumePressed(int aButton);

#endif
