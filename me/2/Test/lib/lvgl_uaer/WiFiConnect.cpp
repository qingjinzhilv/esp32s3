#include "head.h"
#include "InformationGet.h"

extern lv_obj_t * ui_Dropdown1;;

//5s内为连接成功判断为连接失败
unsigned long timeout = 5000; // 5sec

void WiFi_Connect(void *pv);
void WiFi_Find_Task(void *pv);

String ssid = "";
String password = "";

TaskHandle_t WiFi_Connect_handle;
TaskHandle_t WiFi_Find_handle;

extern lv_obj_t *ui_password;
extern lv_obj_t *ui_w_c;

//连接WiFi任务创建
void WiFi_Creat(void)
{
    Serial.println("WiFi_Creat");
    lv_img_set_src(ui_Image2, &ui_img_disconnect_png);
    xTaskCreate((TaskFunction_t)WiFi_Connect,(char *)"WiFi_Connect",(uint32_t)8 * 1024,NULL,(UBaseType_t)10,&WiFi_Connect_handle);
    Serial.println("WiFi_Creat Success!");
}

//连接WiFi
void WiFi_Connect(void *pv)
{
    unsigned long startingTime = millis();
    if(ssid == NULL || password.length() < 8 || password.length() == 0 || password == NULL)
    {
        Serial.println("WiFi ssid/passworld NULL!");
        lv_textarea_set_text(ui_password,"");
    }
    else
    {
        WiFi.begin(ssid.c_str(),password.c_str());
        Serial.println("WiFi_Connect......");
        while((WiFi.status() != WL_CONNECTED) && ((millis() - startingTime) < timeout))
        {
            Serial.println("WiFi_Connect while....");
            delay(250);
        }
        Serial.println("WiFi pdge finish!");
        if (WiFi.status() == WL_CONNECTED)
        {
            lv_textarea_set_text(ui_password,"");
            lv_img_set_src(ui_Image2, &ui_img_connect_png);
            lv_textarea_set_placeholder_text(ui_password, "Connect Success!");
            lv_obj_set_style_bg_img_src(ui_w_c, &ui_img_w_c_s_png, LV_PART_MAIN | LV_STATE_DEFAULT);
            delay(5000);
            lv_obj_set_style_bg_img_src(ui_w_c, &ui_img_w_c_png, LV_PART_MAIN | LV_STATE_DEFAULT);
            delay(1000);
            configTime(8 * 3600,0,NTP1,NTP2,NTP3);
            GetTime_Create();
            GetWeather_Create();
        }
        else
        {
            lv_textarea_set_text(ui_password,"");
            lv_textarea_set_placeholder_text(ui_password,"Wrong password!");
            lv_img_set_src(ui_Image2, &ui_img_disconnect_png);
            lv_obj_set_style_bg_img_src(ui_w_c, &ui_img_w_c_f_png, LV_PART_MAIN | LV_STATE_DEFAULT);
            delay(5000);
            lv_obj_set_style_bg_img_src(ui_w_c, &ui_img_w_c_png, LV_PART_MAIN | LV_STATE_DEFAULT);
            vTaskDelay(1000); 
        }
    }
    vTaskDelete(WiFi_Connect_handle);
}

// 断开WiFi连接
void Cancel_WiFi(void)
{
    if(ssid == NULL || password.length() < 8 || password.length() == 0 || password == NULL)
    {
        Serial.println("WiFi ssid/passworld NULL!");
        lv_textarea_set_text(ui_password,"");
        WiFi.disconnect();                           // 断开WiFi
        delay(300);
    }
    // 如果连接到WiFi了 则断开WiFi 并重新搜索WiFi
    if (WiFi.status() == WL_CONNECTED)
    {
        WiFi.disconnect();                           // 断开WiFi
        delay(300);
        lv_textarea_set_text(ui_password,"");        //清空文本
        vTaskDelay(300);                             // 延时 300ms
        vTaskDelete(WiFi_Connect_handle);           //结束WiFi连接函数
        vTaskDelay(300);
        WiFi_Find_Create();
    }
    // 如果没有连接到WiFi 就开始搜索WiFi
    else if (WiFi.status() != WL_CONNECTED)
    {
        lv_textarea_set_text(ui_password,"");        //清空文本
    }
}

// 创建搜索WiFi任务
void WiFi_Find_Create(void)
{
    xTaskCreate((TaskFunction_t)WiFi_Find_Task,(char *)"WiFi_Find_Task",(uint32_t)8 * 1024,NULL,(UBaseType_t)8,&WiFi_Find_handle);
    delay(500);
}

// 查找WiFi
void WiFi_Find_Task(void *pv)
{
    int n = WiFi.scanNetworks();    //开始同步扫描，将返回值WiFi数量
    if (n <= 0)
    {
        Serial.println("WiFi not found");
        lv_dropdown_add_option(ui_Dropdown1, "Not Found WIFI", LV_DROPDOWN_POS_LAST);
    }
    else // 显示搜索到的WiFi
    {
        vTaskDelay(10);
        for (int i = 0; i < n; ++i)
        {
            //                        WiFi名                 信号强度                       是否加密
            String item = (i + 1) + WiFi.SSID(i) + " (" + WiFi.RSSI(i) + ") " + ((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
            // String item = (i + 1) + WiFi.SSID(i) + " (" + WiFi.RSSI(i) + ") " + ((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
            String item1 = WiFi.SSID(i);
            Serial.println(item);
            // 把搜索到的WiFi追加到下拉列表
            lv_dropdown_add_option(ui_Dropdown1,item1.c_str(), LV_DROPDOWN_POS_LAST);
            delay(10);
        }
    }
    vTaskDelete(NULL);
}
