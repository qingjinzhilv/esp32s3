#include "head.h"

// 屏幕参数配置
static const uint16_t screenWidth = 128;
static const uint16_t screenHeight = 160;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * screenHeight / 10];
TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight);

lv_group_t *group1;

lv_indev_t *indev_button;
extern lv_obj_t * ui_Panel1;
extern lv_obj_t * ui_hour;
extern lv_obj_t * ui_minute;
extern lv_obj_t * ui_second;
extern lv_obj_t * ui_Label6;
extern lv_obj_t * ui_YearMonDa;
extern lv_obj_t * ui_week;
extern lv_obj_t * ui_setup;
extern lv_obj_t * ui_menu;


//实体按键初始化
void Key_Init(void)
{
    pinMode(1,INPUT_PULLUP);
    pinMode(2,INPUT_PULLUP);
    pinMode(3,INPUT_PULLUP);
    pinMode(4,INPUT_PULLUP);
}
//获取实体按键值
uint32_t key_Get(void)
{
    if(digitalRead(1) == 0)
    {
        delay(20);
        if(digitalRead(1) == 0)
        {
            Serial.println("111");
            return 1;
        }
    }
    if(digitalRead(2) == 0)
    {
        delay(20);
        if(digitalRead(2) == 0)
        {
            Serial.println("222");
            return 2;
        }
    }
    if(digitalRead(3) == 0)
    {
        delay(20);
        if(digitalRead(3) == 0)
        {
            Serial.println("333");
            return 3;
        }
    }
    if(digitalRead(4) == 0)
    {
        delay(20);
        if(digitalRead(4) == 0)
        {
            return 4;
        }
    }
    return 0;
}


/*将被库调用读取鼠标*/
static void keypad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    static uint32_t last_key = 0;

    /*获取是否按下了a键并保存了按下的键*/
    uint32_t act_key = key_Get();
    if(act_key != 0) 
    {
        data->state = LV_INDEV_STATE_PRESSED;
        /*根据您的键定义将键转换为 LVGL 控制字符*/
        switch(act_key) 
        {
            case 5:
                //减小值或向左移动
                act_key = LV_KEY_LEFT;
                break;
            case 4:
                //聚焦到上一个点
                act_key = LV_KEY_PREV;
                break;
            case 3:
                //聚焦到下一个点
                act_key = LV_KEY_NEXT;
                break;
            case 2:
                //增加值或向右移动
                act_key = LV_KEY_RIGHT;
                break;
            case 1:
                //确认键
                act_key = LV_KEY_ENTER;
                break;
        }
        last_key = act_key;
    }
    else 
     {
        data->state = LV_INDEV_STATE_RELEASED;
    }
    data->key = last_key;
}

//输入设备初始化函数
void lv_port_indev_init(void)
{
    /*初始化您的小键盘或键盘（如果有）*/
    Key_Init();

    /*注册键盘输入设备*/
    static lv_indev_drv_t indev_drv;
    //初始化输入设备
    lv_indev_drv_init(&indev_drv);
    //注册的按键类型
    indev_drv.type = LV_INDEV_TYPE_KEYPAD;
    //按键控制回调函数
    indev_drv.read_cb = keypad_read;
    //注册输入设备
    indev_button = lv_indev_drv_register(&indev_drv);

    /*稍后您应该使用 'lv_group_t * group = lv_group_create（）' 创建组，
     *使用“lv_group_add_obj（group， obj）”将对象添加到组中
     *并将此输入设备分配给组以在其中导航：
     *'lv_indev_set_group（indev_keypad， 组）;`*/
}

void Group_Init(void)
{
    //创建组
    group1 = lv_group_create();
    // 添加控件到组1里
    // lv_group_add_obj(group1,ui_second);
    lv_group_add_obj(group1,ui_setup);
    lv_group_add_obj(group1,ui_menu);
    // 默认与组1绑定
    lv_indev_set_group(indev_button,group1);
    //初始化聚焦位置
    lv_group_focus_obj(ui_menu);
}

// 刷新显示回调函数
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

// lvgl 初始化 初始化长宽高等基本信息
void lvgl_Init(void)
{
    lv_init();  
    tft.begin();        
    tft.setRotation(0); 
    tft.fillScreen(TFT_BLACK); // 清屏 把屏幕设置为黑色
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * screenHeight / 10);
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);
    lv_port_indev_init();
}


// 创建UI进程
void lvgl_Task(void)
{
    xTaskCreatePinnedToCore(Guitask, "Guitask",8 * 1024,  NULL,12, NULL, 1);
}

// 运行ui进程
void Guitask(void *param)
{
  lvgl_Init();
  ui_init();
  Group_Init();
  while(1)
  {
    lv_timer_handler();
    delay(5); 
  }
}


