#ifndef CONSTANTS_H__
#define CONSTANTS_H__

#define REFRESH_TIME 100

#ifdef CONFIG_BOARD_NATIVE_POSIX

#define USER_BUTTON 4
#define LED_1       1
#define LED_2       2
#define LED_3       3
#define LED_4       4
#define LED_1_MSK   1
#define LED_2_MSK   2
#define LED_3_MSK   4
#define LED_4_MSK   8

#else

#include <dk_buttons_and_leds.h>

#define USER_BUTTON DK_BTN4_MSK
#define LED_1       DK_LED1
#define LED_2       DK_LED2
#define LED_3       DK_LED3
#define LED_4       DK_LED4
#define LED_1_MSK   DK_LED1_MSK
#define LED_2_MSK   DK_LED2_MSK
#define LED_3_MSK   DK_LED3_MSK
#define LED_4_MSK   DK_LED4_MSK

#endif


#endif //CONSTANTS_H__