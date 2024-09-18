#include "MAX98357_MiniMax.h"

// 将宏定义的字符串放到一个数组中
const char* voiceTypeStrings[46] = {
    MALE_QN_QINGSE, // 青涩青年音色
    MALE_QN_JINGYING, // 精英青年音色
    MALE_QN_BADAO, // 霸道青年音色
    MALE_QN_DAXUESHENG, // 青年大学生音色
    FEMALE_SHAONV, // 少女音色
    FEMALE_YUJIE, // 御姐音色
    FEMALE_CHENGSHU, // 成熟女性音色
    FEMALE_TIANMEI, // 甜美女性音色
    PRESENTER_MALE, // 男性主持人
    PRESENTER_FEMALE, // 女性主持人
    AUDIOBOOK_MALE_1, // 男性有声书1
    AUDIOBOOK_MALE_2, // 男性有声书2
    AUDIOBOOK_FEMALE_1, // 女性有声书1
    AUDIOBOOK_FEMALE_2, // 女性有声书2
    MALE_QN_QINGSE_JINGPIN, // 青涩青年音色-beta
    MALE_QN_JINGYING_JINGPIN, // 精英青年音色-beta
    MALE_QN_BADAO_JINGPIN, // 霸道青年音色-beta
    MALE_QN_DAXUESHENG_JINGPIN, // 青年大学生音色-beta
    FEMALE_SHAONV_JINGPIN, // 少女音色-beta
    FEMALE_YUJIE_JINGPIN, // 御姐音色-beta
    FEMALE_CHENGSHU_JINGPIN, // 成熟女性音色-beta
    FEMALE_TIANMEI_JINGPIN, // 甜美女性音色-beta
    CLEVER_BOY, // 聪明男童
    CUTE_BOY, // 可爱男童
    LOVELY_GIRL, // 萌萌女童
    CARTOON_PIG, // 卡通猪小琪
    BINGJIAO_DIDI, // 病娇弟弟
    JUNLANG_NANYOU, // 俊朗男友
    CHUNZHEN_XUEDI, // 纯真学弟
    LENGDAN_XIONGZHANG, // 冷淡学长
    BADAO_SHAOYE, // 霸道少爷
    TIANXIN_XIAOLING, // 甜心小玲
    QIAOPI_MENGMEI, // 俏皮萌妹
    WUMEI_YUJIE, // 妩媚御姐
    DIADIA_XUEMEI, // 嗲嗲学妹
    DANYA_XUEJIE, // 淡雅学姐
    SANTA_CLAUS, // Santa Claus
    GRINCH, // Grinch
    RUDOLPH, // Rudolph
    ARNOLD, // Arnold
    CHARMING_SANTA, // Charming Santa
    CHARMING_LADY, // Charming Lady
    SWEET_GIRL, // Sweet Girl
    CUTE_ELF, // Cute Elf
    ATTRACTIVE_GIRL, // Attractive Girl
    SERENE_WOMAN // Serene Woman
};

String voice_id = "female-tianmei-jingpin"; 
// 2. 替换为您的 OpenAI API 密钥
const char* apiKey = "eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCJ9.eyJHcm91cE5hbWUiOiLlgL7lsL0iLCJVc2VyTmFtZSI6IuWAvuWwvSIsIkFjY291bnQiOiIiLCJTdWJqZWN0SUQiOiIxODI5MTMwNzc1NDg4OTA1NjM3IiwiUGhvbmUiOiIxNTg0Mjk2MjMyNyIsIkdyb3VwSUQiOiIxODI5MTMwNzc1NDgwNTE2NjIwIiwiUGFnZU5hbWUiOiIiLCJNYWlsIjoiIiwiQ3JlYXRlVGltZSI6IjIwMjQtMDktMTIgMTk6MzY6MzkiLCJpc3MiOiJtaW5pbWF4In0.sw1jqcY4mcFCn_0pzZjokILMye_r1f_zem738REoYAjthTelAM2nN9XiA2n_F6JwL4v9RlCy0it-AiwPON6wvqxXabFqFr5bCMapH_ANa-IhRsndlAOA7UrEPlXtZ3J5bkbILivXsd_ISQJlrrX3EPZDoxn3M_HiXZXTbm7D09cfdkBxIiT1wVYoytujbIPDaJDJskaNkCgAYJN21H4RMRXAqxPWh3pCR76-1KAiT8I12oJu5fckKT708TuKdSWdKVYHcDtGzpBrwPBINQOkUuh7XWGG898ybT8SA8v8VPp9RW_sGRt514QFzMZ72Mg999hh_App9DDXxYPcmK8Rzg";
const char* group_id = "1829130775480516620";
const char* url = "https://api.minimax.chat/v1/t2a_pro?GroupId=1829130775480516620";
char myCharPointer;
//扬声器引脚
#define I2S_DOUT 5  // DIN connection
#define I2S_BCLK 6  // Bit clock
#define I2S_LRC 7   // Left Right Clock

Audio audio;
String answerv;

float speed_value = 1.0;

//Text_conversion
String Text_conversion(String inputText) 
{
  HTTPClient http1;
  http1.setTimeout(60000); // 设置超时时间为10秒
  if(!http1.begin(url)) 
  {
    Serial.println("HTTPClient begin failed");
    return "Error";
  }
  http1.addHeader("Content-Type", "application/json");
  String token_key = String("Bearer ") + apiKey;
  http1.addHeader("Authorization", token_key);

  // 创建一个StaticJsonDocument对象，足够大以存储JSON数据
  StaticJsonDocument<2048> doc;

  // 填充数据
  doc["text"] = inputText;
  doc["model"] = "speech-01";
  doc["audio_sample_rate"] = 32000;
  doc["bitrate"] = 128000;
  doc["voice_id"] = voice_id;
  doc["speed"] = speed_value;
  doc["stream"] = "True";
  


  // 创建一个String对象来存储序列化后的JSON字符串
  String jsonString;

  // 序列化JSON到String对象
  serializeJson(doc, jsonString);

  // 发送POST请求
  int httpResponseCode1 = http1.POST(jsonString);
  Serial.printf("HTTP Response code: %s\r\n",jsonString.c_str());

  if(httpResponseCode1 != 200) 
  {
    Serial.print("HTTP Request Failed, error code: ");
    Serial.println(httpResponseCode1);
    http1.end();
    return "Error"; // 返回一个默认的错误信息
  }

  String reason = http1.getString();
  Serial.println("Received response:");
  Serial.println(reason);
  http1.end();
  DynamicJsonDocument jsonDoc1(2048);
  deserializeJson(jsonDoc1, reason);
  String outputText = jsonDoc1["audio_file"];
  return outputText; // 返回响应数据
}

//设置语音
void MAX98357_SetText(String inputText) 
{
  answerv = Text_conversion(inputText);
  Serial.print(answerv);
  char myCharPointer[answerv.length() + 1];  // 分配足够的空间来存储字符串
  strcpy(myCharPointer, answerv.c_str());    // 复制字符串到 myCharPointer
  audio.connecttohost(myCharPointer);        //  128k mp3
}

void MAX98357_Init(void) 
{
    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    audio.setVolume(20);  // 0...21
}

void MAX98357_loop(void) 
{
    audio.loop();
}
//设置音色
void MAX98357_Settimbre(String inputText) 
{
    voice_id = inputText;
}
//设置声音大小
void MAX98357_SetSound_volume(uint8_t volume) 
{
    if(volume > 21) volume = 21;
    audio.setVolume(volume);
}

//设置语速
void MAX98357_SetSpeed(float speed) 
{
    if(speed >= 2.0) speed = 2.0;
    else if(speed <= 0.5) speed = 0.5;
    speed_value = speed;
}
