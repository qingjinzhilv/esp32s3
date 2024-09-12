#ifndef _LVGL_TASK_H
#define _LVGL_TASK_H

#include "head.h"

// 刷新显示回调函数
void my_disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);
// lvgl 初始化 初始化长宽高等基本信息
void lvgl_Init(void);
//按键初始化
void Key_Init(void);
//按键值获取
uint32_t key_Get(void);

void Group_Init(lv_group_t *group);

void keypad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);

void lv_port_indev_init(void);

void lvgl_Task(void);
void Guitask(void *param);

#endif
