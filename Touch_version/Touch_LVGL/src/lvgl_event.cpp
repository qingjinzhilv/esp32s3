#include "lvgl_event.h"

String answer;

void ui_event_Button1(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) 
    {
        String inputText = lv_textarea_get_text(ui_TextArea1);
        Serial.println("Input Text: " + inputText);
        if(inputText.length() > 0)
        {
            answer = getGPTAnswer(inputText);
            Serial.println("AI: " + answer);
            lv_textarea_set_text(ui_TextArea2, answer.c_str());
            Serial.println("Enter a prompt:");
        }
    }
}

void ui_event_Control_interface(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) 
    {
        _ui_screen_change(&ui_MENU, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_MENU_screen_init);
    }
}

void ui_event_MENU(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) 
    {
        _ui_screen_change(&ui_Screen1, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_Screen1_screen_init);
    }
}

void event_Init(void)
{
    lv_obj_add_event_cb(ui_Button1, ui_event_Button1, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Control_interface, ui_event_Control_interface, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_MENU, ui_event_MENU, LV_EVENT_ALL, NULL);
}


