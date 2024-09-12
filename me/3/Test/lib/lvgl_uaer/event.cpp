#include "Arduino.h"
#include <TFT_eSPI.h>
#include "event.h"
#include "lvgl_task.h"
#include "WiFiConnect.h"

uint8_t screen_flag = 1;

lv_obj_t * ui_Screen2;
lv_obj_t * ui_Screen3;
lv_obj_t * ui_Screen4;
lv_obj_t * ui_Screen5;
lv_obj_t * ui_Screen6;
lv_obj_t * ui_Screen7;


lv_obj_t * ui_Label2;
lv_obj_t * list_btn;
lv_obj_t * list1;

extern lv_group_t *group1;
lv_indev_t *lv_btn;

//WiFi界面
lv_obj_t * ui_wifilabel;
lv_obj_t * ui_Exit;
lv_obj_t * ui_Dropdown1;
lv_obj_t * ui_w_c;
lv_obj_t * ui_password;
lv_obj_t * ui_Keyboard1;

/*向列表添加按钮*/
lv_obj_t *btn[128];
uint8_t list_btn1 = 6;

static void event_handler(lv_event_t * e);

//所以屏幕相关参数和函数
uint8_t screen_n = 7;
void (*Screen_Init[128])(void) = {ui_Screen1_screen_init,lv_Screen2_Init,lv_Screen3_Init,lv_Screen4_Init,lv_Screen5_Init,lv_Screen6_Init,lv_Screen7_Init};


void lv_disp_Screen(uint8_t n)
{
    switch (n)
    {
        case 1:
            lv_disp_load_scr(ui_Screen1);
        break;
        case 2:
            lv_disp_load_scr(ui_Screen2);
        break;
        case 3:
            lv_disp_load_scr(ui_Screen3);
        break;
        case 4:
            lv_disp_load_scr(ui_Screen4);
        break;
        case 5:
            lv_disp_load_scr(ui_Screen5);
        break;
        case 6:
            lv_disp_load_scr(ui_Screen6);
        break;
        case 7:
            lv_disp_load_scr(ui_Screen7);
        break;
    }
    if(screen_flag != 3)
    {
        //重新复位连接WiFi按键显示
        lv_obj_set_style_bg_img_src(ui_w_c, &ui_img_w_c_png, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_textarea_set_text(ui_password,"");
        lv_textarea_set_placeholder_text(ui_password, "password.......");
    }
}
//界面1的事件触发
void ui_event_setup(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    if(code == LV_EVENT_PRESSED && screen_flag == 1)
    {
        lv_obj_clear_state(obj, LV_STATE_ANY);     // 清除状态
        //显示界面2
        lv_disp_Screen(2);
        screen_flag = 2;
    }
}

void ui_event_menu(lv_event_t * e)
{
    
}

//屏幕1的事件触发回调函数设置
void ui_screen_event_add(void)
{
    //按键的事件触发 按下时
    lv_obj_add_event_cb(ui_setup, ui_event_setup, LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(ui_menu, ui_event_menu, LV_EVENT_PRESSED, NULL);
}
//界面2的事件触发
static void event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_LONG_PRESSED && screen_flag == 2) 
    {
        if(obj == btn[0])
        {
            WiFi_Find_Create();                             // 查找WiFi
            lv_obj_clear_state(obj, LV_STATE_PRESSED);      // 清除状态
            lv_disp_Screen(3);                              //加载界面3
            lv_group_focus_obj(ui_Exit);                    //聚焦在退出按键
            screen_flag = 3;                                //界面3标志位
        }
        else if(obj == btn[1])
        {
            lv_disp_Screen(4);
            screen_flag = 4;
        }
        else if(obj == btn[2])
        {
            lv_disp_Screen(5);
            screen_flag = 5;
        }
        else if(obj == btn[3])
        {
            lv_disp_Screen(6);
            screen_flag = 6;
        }
        else if(obj == btn[4])
        {
            lv_disp_Screen(7);
            screen_flag = 7;
        }
        else if(obj == btn[5])
        {
            //显示界面1
            lv_disp_Screen(1);
            screen_flag = 1;
        }
        lv_group_focus_obj(btn[1]);
        lv_obj_clear_state(obj, LV_STATE_PRESSED);     // 清除状态
        Serial.printf("Clicked: %s\r\n", lv_list_get_btn_text(list1, obj));
    }
}
//界面3的事件触发
static void WiFi_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_PRESSED && screen_flag == 3) 
    {
        if(obj == ui_Dropdown1)
        {
            //1为编辑模式 0为导航模式
            uint8_t value = lv_group_get_editing(group1) ? 0 : 1;
            lv_group_set_editing(group1,value);
        }
        else if(obj == ui_password)
        {
            uint8_t value = lv_group_get_editing(group1) ? 0 : 1;
            lv_group_set_editing(group1,value);
            Serial.printf("ui_password->%d\r\n",value);
            //显示键盘并聚焦在键盘上
            _ui_flag_modify(ui_Keyboard1,LV_OBJ_FLAG_HIDDEN,_UI_MODIFY_FLAG_TOGGLE);
            lv_group_focus_obj(ui_Keyboard1);
            lv_obj_clear_state(ui_password, LV_STATE_ANY);
        }
        else if(obj == ui_Keyboard1)
        {
            //获取键盘上按键按下的值
            const char * txt = lv_btnmatrix_get_btn_text(obj, lv_btnmatrix_get_selected_btn(obj));
            if(strcmp(txt, LV_SYMBOL_OK) == 0) 
            {
                //1为编辑模式 0为导航模式
                uint8_t value = lv_group_get_editing(group1) ? 0 : 1;
                lv_group_set_editing(group1,value);
                Serial.printf("ui_Keyboard1->%d\r\n",value);
                //如果是导航模式，则隐藏键盘并显示按键
                if(value == 0)
                {
                    //隐藏键盘
                    _ui_flag_modify(ui_Keyboard1,LV_OBJ_FLAG_HIDDEN,_UI_MODIFY_FLAG_TOGGLE);  
                }
            }
            else if(strcmp(txt, LV_SYMBOL_KEYBOARD) == 0) 
            {
                lv_keyboard_set_mode(ui_Keyboard1, LV_KEYBOARD_MODE_TEXT_LOWER);
            }
            else if(strcmp(txt, "ABC") == 0) 
            {
                lv_keyboard_set_mode(ui_Keyboard1, LV_KEYBOARD_MODE_TEXT_UPPER);
            }
            else if(strcmp(txt, "1#") == 0) 
            {
                lv_keyboard_set_mode(ui_Keyboard1, LV_KEYBOARD_MODE_SPECIAL);
            }
            else if(strcmp(txt, "abc") == 0) 
            {
                lv_keyboard_set_mode(ui_Keyboard1, LV_KEYBOARD_MODE_TEXT_LOWER);
            }
            // 清除按钮 所有状态
            lv_obj_clear_state(ui_Exit, LV_STATE_ANY);      
            lv_obj_clear_state(ui_w_c, LV_STATE_ANY);  
            lv_obj_clear_state(ui_Dropdown1, LV_STATE_ANY);
            lv_obj_clear_state(ui_Keyboard1, LV_STATE_ANY);
            lv_obj_clear_state(ui_password, LV_STATE_ANY);
            lv_group_focus_obj(ui_Keyboard1);
        }
        else if(obj == ui_Exit)
        {
            lv_disp_Screen(2);
            screen_flag = 2;
        }
    }
    else if(code == LV_EVENT_LONG_PRESSED && screen_flag == 3) 
    {
        if(obj == ui_w_c)
        {
            //获取WiFi名称
            char ssid_1[32];
            lv_dropdown_get_selected_str(ui_Dropdown1,ssid_1,sizeof(ssid_1));
            ssid = String((const char *)ssid_1);
            // 获取输入的WiFi密码
            char password_1[17];
            sprintf(password_1,"%s",lv_textarea_get_text(ui_password));
            password = String(password_1);
            //自动清除字符串中最前面和最后面的空白数据
            password.trim();
            // 打印WiFi名称和密码
            Serial.println(ssid);
            Serial.println(password);
            Serial.println(password.length());
            //如果WiFi连接了，取消WiFi的连接
            Cancel_WiFi();
            //创建WiFi连接任务
            WiFi_Creat();
        }
    }
}

/**
 * 界面2 菜单
 */
void lv_Screen2_Init(void)
{
    static lv_anim_t animation_template;
    static lv_style_t label_style;

    //创建第二个屏幕
    ui_Screen2 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Screen2, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Screen2, lv_color_hex(0x0CDC9C9), LV_PART_MAIN);

    lv_anim_init(&animation_template);
    /*等待 1 秒钟以开始第一次滚动*/
    lv_anim_set_delay(&animation_template, 1000); 
    /*在标签滚动回初始位置 3 秒后重复滚动*/          
    lv_anim_set_repeat_delay(&animation_template,3000);    
    /*使用动画模板初始化标签样式*/
    lv_style_init(&label_style);
    lv_style_set_anim(&label_style, &animation_template);
    //建立标签
    lv_obj_t *setup_label = lv_label_create(ui_Screen2);
    lv_label_set_long_mode(setup_label, LV_LABEL_LONG_SCROLL_CIRCULAR);      /*圆形卷轴*/
    //设置宽
    lv_obj_set_width(setup_label, 30);
    //设置位置
    lv_obj_set_x(setup_label, 0);
    lv_obj_set_y(setup_label, -70);
    lv_obj_set_align(setup_label, LV_ALIGN_CENTER);
    //设置内容
    lv_label_set_text(setup_label, "Setup");
    //设置字体颜色
    lv_obj_set_style_text_color(setup_label, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    //设置字体
    lv_obj_set_style_text_opa(setup_label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    /*创建列表*/
    list1 = lv_list_create(ui_Screen2);
    lv_obj_set_x(list1, 0);
    lv_obj_set_y(list1, -20);
    lv_obj_set_size(list1, 110, 130);
    lv_obj_center(list1);;
    
    /*注册键盘输入设备*/
    static lv_indev_drv_t indev_drv;
    //初始化输入设备
    lv_indev_drv_init(&indev_drv);
    //注册的按键类型
    indev_drv.type = LV_INDEV_TYPE_KEYPAD;
    //按键控制回调函数
    indev_drv.read_cb = keypad_read;
    //注册输入设备
    lv_btn = lv_indev_drv_register(&indev_drv);
    //创建组
    group1 = lv_group_create();
    //WiFi连接
    btn[0] = lv_list_add_btn(list1, LV_SYMBOL_WIFI, "WiFi");
    //蓝牙
    btn[1] = lv_list_add_btn(list1, LV_SYMBOL_BLUETOOTH, "Bluetooth");
    //时间设置
    btn[2] = lv_list_add_btn(list1, LV_SYMBOL_SETTINGS, "Set Timer");
    //数据查看
    btn[3] = lv_list_add_btn(list1, LV_SYMBOL_EYE_OPEN, "Data View");
    //闹钟
    btn[4] = lv_list_add_btn(list1, LV_SYMBOL_BELL, "Alarm Clock");
    //退出
    btn[5] = lv_list_add_btn(list1, LV_SYMBOL_CLOSE, "Exit");
    
    lv_obj_add_event_cb(btn[0], event_handler, LV_EVENT_LONG_PRESSED, NULL);
    lv_obj_add_event_cb(btn[1], event_handler, LV_EVENT_LONG_PRESSED, NULL);
    lv_obj_add_event_cb(btn[2], event_handler, LV_EVENT_LONG_PRESSED, NULL);
    lv_obj_add_event_cb(btn[3], event_handler, LV_EVENT_LONG_PRESSED, NULL);
    lv_obj_add_event_cb(btn[4], event_handler, LV_EVENT_LONG_PRESSED, NULL);
    lv_obj_add_event_cb(btn[5], event_handler, LV_EVENT_LONG_PRESSED, NULL);

    lv_group_add_obj(group1,btn[0]);
    lv_group_add_obj(group1,btn[1]);
    lv_group_add_obj(group1,btn[2]);
    lv_group_add_obj(group1,btn[3]);
    lv_group_add_obj(group1,btn[4]);
    lv_group_add_obj(group1,btn[5]);
    lv_indev_set_group(lv_btn,group1);
    //初始化聚焦位置
    lv_group_focus_obj(btn[1]);
    
}

/**
 * 界面3 WiFi
*/
void lv_Screen3_Init(void)
{
    ui_Screen3 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Screen3, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Screen3, lv_color_hex(0xFFDAB9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Screen3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_wifilabel = lv_label_create(ui_Screen3);
    lv_obj_set_width(ui_wifilabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_wifilabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_wifilabel, 0);
    lv_obj_set_y(ui_wifilabel, -66);
    lv_obj_set_align(ui_wifilabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_wifilabel, "WiFI");
    lv_obj_set_style_text_font(ui_wifilabel, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Exit = lv_btn_create(ui_Screen3);
    lv_obj_set_width(ui_Exit, 24);
    lv_obj_set_height(ui_Exit, 24);
    lv_obj_set_x(ui_Exit, -49);
    lv_obj_set_y(ui_Exit, 66);
    lv_obj_set_align(ui_Exit, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Exit, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_Exit, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Exit, lv_color_hex(0xE6E6FA), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Exit, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(ui_Exit, &ui_img_exit_png, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Dropdown1 = lv_dropdown_create(ui_Screen3);
    lv_dropdown_set_options(ui_Dropdown1, "");
    lv_obj_set_width(ui_Dropdown1, 118);
    lv_obj_set_height(ui_Dropdown1, 30);
    lv_obj_set_x(ui_Dropdown1, 0);
    lv_obj_set_y(ui_Dropdown1, -32);
    lv_obj_set_align(ui_Dropdown1, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Dropdown1, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags

    ui_w_c = lv_btn_create(ui_Screen3);
    lv_obj_set_width(ui_w_c, 24);
    lv_obj_set_height(ui_w_c, 24);
    lv_obj_set_x(ui_w_c, 49);
    lv_obj_set_y(ui_w_c, 66);
    lv_obj_set_align(ui_w_c, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_w_c, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_w_c, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_w_c, lv_color_hex(0xE6E6FA), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_w_c, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(ui_w_c, &ui_img_w_c_png, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_password = lv_textarea_create(ui_Screen3);
    lv_obj_set_width(ui_password, 118);
    lv_obj_set_height(ui_password, 30);
    lv_obj_set_x(ui_password, 0);
    lv_obj_set_y(ui_password, 0);
    lv_obj_set_align(ui_password, LV_ALIGN_CENTER);
    lv_textarea_set_placeholder_text(ui_password, "password.......");

    ui_Keyboard1 = lv_keyboard_create(ui_Screen3);
    lv_keyboard_set_mode(ui_Keyboard1, LV_KEYBOARD_MODE_SPECIAL);
    lv_obj_set_width(ui_Keyboard1, 128);
    lv_obj_set_height(ui_Keyboard1, 65);
    lv_obj_set_x(ui_Keyboard1, -1);
    lv_obj_set_y(ui_Keyboard1, 46);
    lv_obj_set_align(ui_Keyboard1, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Keyboard1, LV_OBJ_FLAG_HIDDEN);     /// Flags
    

    lv_keyboard_set_textarea(ui_Keyboard1, ui_password);

    /*注册键盘输入设备*/
    static lv_indev_drv_t indev_drv;
    //初始化输入设备
    lv_indev_drv_init(&indev_drv);
    //注册的按键类型
    indev_drv.type = LV_INDEV_TYPE_KEYPAD;
    //按键控制回调函数
    indev_drv.read_cb = keypad_read;
    //注册输入设备
    lv_btn = lv_indev_drv_register(&indev_drv);
    //创建组
    group1 = lv_group_create();
    
    lv_group_add_obj(group1,ui_Dropdown1);
    lv_group_add_obj(group1,ui_password);
    lv_group_add_obj(group1,ui_Exit);
    lv_group_add_obj(group1,ui_w_c);
    lv_group_add_obj(group1,ui_Keyboard1);

    lv_indev_set_group(lv_btn,group1);
    //初始化聚焦位置
    lv_group_focus_obj(ui_Dropdown1);

    lv_obj_add_event_cb(ui_Exit, WiFi_event_handler, LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(ui_w_c, WiFi_event_handler, LV_EVENT_LONG_PRESSED, NULL);
    lv_obj_add_event_cb(ui_Dropdown1, WiFi_event_handler, LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(ui_password, WiFi_event_handler, LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(ui_Keyboard1, WiFi_event_handler, LV_EVENT_PRESSED, NULL);

    // // 当下拉框的值改变时 获取下拉框的文本值
    // lv_obj_add_event_cb(ui_Dropdown1, WiFi_event_handler, LV_EVENT_VALUE_CHANGED, NULL);

    // lv_obj_add_event_cb(ui_Dropdown1, Dropdown_event_handler, LV_EVENT_PRESSED, NULL);

}

/**
 * 界面4
*/
void lv_Screen4_Init(void)
{
    ui_Screen4 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Screen4, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Screen4, lv_color_hex(0x0CDC9C9), LV_PART_MAIN);

    lv_obj_t *ui_wifi_label = lv_label_create(ui_Screen4);
    lv_obj_set_width(ui_wifi_label, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_wifi_label, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_wifi_label, 0);
    lv_obj_set_y(ui_wifi_label, -66);
    lv_obj_set_align(ui_wifi_label, LV_ALIGN_CENTER);
    lv_label_set_text(ui_wifi_label, "Bluetooth");
    lv_obj_set_style_text_font(ui_wifi_label, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
}
/**
 * 界面5
*/
void lv_Screen5_Init(void)
{
    ui_Screen5 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Screen5, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Screen5, lv_color_hex(0x0CDC9C9), LV_PART_MAIN);

    lv_obj_t *ui_wifi_label = lv_label_create(ui_Screen5);
    lv_obj_set_width(ui_wifi_label, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_wifi_label, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_wifi_label, 0);
    lv_obj_set_y(ui_wifi_label, -66);
    lv_obj_set_align(ui_wifi_label, LV_ALIGN_CENTER);
    lv_label_set_text(ui_wifi_label, "Set Timer");
    lv_obj_set_style_text_font(ui_wifi_label, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
}

/**
 * 界面6
*/
void lv_Screen6_Init(void)
{
    ui_Screen6 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Screen6, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Screen6, lv_color_hex(0x0CDC9C9), LV_PART_MAIN);

    lv_obj_t *ui_wifi_label = lv_label_create(ui_Screen6);
    lv_obj_set_width(ui_wifi_label, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_wifi_label, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_wifi_label, 0);
    lv_obj_set_y(ui_wifi_label, -66);
    lv_obj_set_align(ui_wifi_label, LV_ALIGN_CENTER);
    lv_label_set_text(ui_wifi_label, "Data View");
    lv_obj_set_style_text_font(ui_wifi_label, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
}

/**
 * 界面7
*/
void lv_Screen7_Init(void)
{
    ui_Screen7 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Screen7, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Screen7, lv_color_hex(0x0CDC9C9), LV_PART_MAIN);

    lv_obj_t *ui_wifi_label = lv_label_create(ui_Screen7);
    lv_obj_set_width(ui_wifi_label, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_wifi_label, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_wifi_label, 0);
    lv_obj_set_y(ui_wifi_label, -66);
    lv_obj_set_align(ui_wifi_label, LV_ALIGN_CENTER);
    lv_label_set_text(ui_wifi_label, "Alarm Clock");
    lv_obj_set_style_text_font(ui_wifi_label, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
}



