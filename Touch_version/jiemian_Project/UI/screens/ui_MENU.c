// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.0
// LVGL version: 8.3.11
// Project name: SquareLine_Project

#include "../ui.h"

void ui_MENU_screen_init(void)
{
    ui_MENU = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_MENU, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_MENU, lv_color_hex(0xFFF5EE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_MENU, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label3 = lv_label_create(ui_MENU);
    lv_obj_set_width(ui_Label3, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label3, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label3, -2);
    lv_obj_set_y(ui_Label3, -102);
    lv_obj_set_align(ui_Label3, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label3, "MENU");

    ui_Label5 = lv_label_create(ui_MENU);
    lv_obj_set_width(ui_Label5, 279);
    lv_obj_set_height(ui_Label5, 29);
    lv_obj_set_x(ui_Label5, -1);
    lv_obj_set_y(ui_Label5, -31);
    lv_obj_set_align(ui_Label5, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label5, "AI-UART");
    lv_obj_add_state(ui_Label5, LV_STATE_CHECKED);       /// States
    lv_obj_add_flag(ui_Label5, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_set_style_text_align(ui_Label5, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label5, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label7 = lv_label_create(ui_MENU);
    lv_obj_set_width(ui_Label7, 291);
    lv_obj_set_height(ui_Label7, 29);
    lv_obj_set_x(ui_Label7, 0);
    lv_obj_set_y(ui_Label7, 8);
    lv_obj_set_align(ui_Label7, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label7, "Language chat");
    lv_obj_add_flag(ui_Label7, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_set_style_text_align(ui_Label7, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label7, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_Label5, ui_event_Label5, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Label7, ui_event_Label7, LV_EVENT_ALL, NULL);

}
