#ifndef LVGL_TASK_H
#define LVGL_TASK_H

#include <Arduino.h>
#include "TFT_eSPI.h"
#include "SPI.h"
#include "lvgl.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>

#include "AI.h"
#include "ui.h"
#include "My_GetTime.h"
#include "My_WiFi.h"
#include "My_XPT2046.h"
#include "Weather.h"
#include "lvgl_event.h"

extern lv_obj_t * ui_Screen1;
extern lv_obj_t * ui_Panel1;
extern lv_obj_t * ui_Label1;
extern lv_obj_t * ui_Label2;

void Init(void);

void my_disp_flush( lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p );
void my_touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
void lvgl_Task(void);


#endif /* LVGL_TASK_H */
