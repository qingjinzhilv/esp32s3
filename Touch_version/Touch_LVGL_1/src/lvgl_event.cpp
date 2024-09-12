#include "lvgl_event.h"

// extern String AI_Data_Str_child,AI_Data_Str;
String answer;

void AIData_Task(void *pvParameters)
{
  while(1)
  {
    if(AI_Data_Str_child.length() > 0)
    {
    //   Serial.println("AI: " + AI_Data_Str);
    //   Serial.println("AI: " + AI_Data_Str_child);
    //   Serial.println("AI: " + answer);
      //追加数据到文本框内
    //   lv_textarea_add_text(ui_TextArea2,AI_Data_Str_child.c_str());
        // lv_textarea_set_text(ui_TextArea2, AI_Data_Str.c_str());
      AI_Data_Str_child = "";
    }
    //接受完后删除任务
    if(answer.length() > 0)
    {
        Serial.println("删除AIData_Task任务！");
        vTaskDelete(NULL); //删除AI任务
    }
    delay(100);
  }
}

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
            lv_textarea_set_text(ui_TextArea2, (char *)"");
            // xTaskCreate(AIData_Task, "AIData_Task", 1024*2, NULL, 13, NULL); //创建AI任务
            answer = getGPTAnswer(inputText);
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


