#ifndef __EVENT_H_
#define __EVENT_H_

#include "ui.h"

extern uint8_t screen_n;
extern void (*Screen_Init[128])(void);

void lv_disp_Screen(uint8_t n);

void ui_screen_event_add(void);
void ui_event_menu(lv_event_t * e);
void ui_event_setup(lv_event_t * e);

void lv_Screen2_Init(void);
void lv_Screen3_Init(void);
void lv_Screen4_Init(void);
void lv_Screen5_Init(void);
void lv_Screen6_Init(void);
void lv_Screen7_Init(void);

#endif
