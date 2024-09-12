#include "lvgl_task.h"

/*更改屏幕分辨率*/
static const uint16_t screenWidth = 320;
static const uint16_t screenHeight = 240;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * screenHeight / 10];

TFT_eSPI tft = TFT_eSPI();

WIFI My_WiFi;
uint16_t x, y;
String inputText = "你好，文心一言";
String test = "";

void Init(void)
{
  //初始化串口
  Serial.begin(115200);
  //初始化触摸屏
  XPT2046_Init();                          
  // 连接WIFI
  My_WiFi.WIFI_Connect("倾尽","00000001"); 
  //验证与文星一言的连接
  // test = getGPTAnswer(inputText);
  // Serial.println("Answer: " + test);
  //获取本地时间
  My_GetTime.GetTime_Set(2024, 9, 8, 9, 45, 0);
  My_GetTime.GetTime_Init();
  My_GetTime.GetTime_Data_Analyze();
  //获取天气
  Buil_Request();
  //lvgl初始化
  lv_init();
  /*初始化显示屏*/
  tft.begin();
  tft.setRotation(3);        // 显示方向
  tft.fillScreen(TFT_BLACK); // 用黑色填充屏幕
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * screenHeight / 10);
  /*初始化显示*/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  /*将以下行更改为您的显示分辨率*/
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);
  /*初始化（虚拟）输入设备驱动程序*/
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);
}

/* 显示刷新 */
void my_disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t *)&color_p->full, w * h, true);
  tft.endWrite();

  lv_disp_flush_ready(disp_drv);
}

/*阅读触摸板*/
void my_touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
  uint16_t touchX, touchY;

  bool touched = digitalRead(21);          // 读取触摸屏状态
  XPT2046_GetCoordinate(&touchX, &touchY); // 读取触摸坐标

  if (touched)
  {
    data->state = LV_INDEV_STATE_REL;
  }
  else
  {
    data->state = LV_INDEV_STATE_PR;

    /*设置坐标*/
    data->point.x = touchX;
    data->point.y = touchY;

    Serial.print("Data x ");
    Serial.println(touchX);

    Serial.print("Data y ");
    Serial.println(touchY);
  }
}

// 运行ui进程
void Guitask(void *param)
{
  // 初始化UI界面
  ui_init();
  event_Init();
  while (1)
  {
    // 刷新UI界面
    lv_timer_handler();
    delay(5);
  }
}
// UI界面修改进程
void Interface_modification_task(void *param)
{
  while (1)
  {
    char time_str[7] = {0};
    sprintf(time_str, "%.2d : %.2d", My_GetTime.My_Time_Data.hour, My_GetTime.My_Time_Data.minute);
    lv_label_set_text(ui_Label1, time_str);
    memset(time_str, 0, sizeof(time_str));
    sprintf(time_str, "%.2d", My_GetTime.My_Time_Data.second);
    lv_label_set_text(ui_Label2, time_str);
    delay(100);
  }
}

//AI进程 
void AI_Task(void *param)
{
  while (1)
  {
    if(Serial.available())
    {
      inputText = Serial.readStringUntil('\n');
      if(inputText.length() > 0)
      {
        Serial.println("\r\nme:" + inputText);
        lv_textarea_set_text(ui_TextArea1, (char *)"");
        lv_textarea_set_text(ui_TextArea2, (char *)"");
        lv_textarea_set_text(ui_TextArea1, inputText.c_str());
      }
    }
    delay(100);
  }
}

// 创建UI进程
void lvgl_Task(void)
{
  // 创建UI进程
  xTaskCreatePinnedToCore(Guitask, "Guitask", 8 * 1024, NULL, 11, NULL, 1);
  // 创建UI界面修改进程
  xTaskCreate(Interface_modification_task, "Interface_modification_task", 4 * 1024, NULL, 10, NULL);
  // AI进程
  xTaskCreate(AI_Task, "AI_Task", 5 * 1024, NULL, 9, NULL);
}
