#ifndef _CONSTANTS_H
#define _CONSTANTS_H

#define REFRESH_TIME 500

/*
#if defined(CONFIG_ARCH_POSIX)
#define USER_BUTTON 8
const uint8_t leds[4] = {1, 2, 4, 8};
#else
*/
#include <dk_buttons_and_leds.h>
#define USER_BUTTON DK_BTN4_MSK
const uint8_t leds[4] = {DK_LED1, DK_LED2, DK_LED3, DK_LED4};
//#endif

#endif //_CONSTANTS_H