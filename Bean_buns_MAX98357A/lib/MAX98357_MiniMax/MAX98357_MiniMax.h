#ifndef MAX98357_MINIMAX_H_
#define MAX98357_MINIMAX_H_

#include <Audio.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "Arduino.h"

/* 需要的库，可以复制
 * plageoj/UrlEncode@^1.0.1
 * bblanchon/ArduinoJson@^7.1.0
*/

#define MALE_QN_QINGSE "male-qn-qingse" // 青涩青年音色
#define MALE_QN_JINGYING "male-qn-jingying" // 精英青年音色
#define MALE_QN_BADAO "male-qn-badao" // 霸道青年音色
#define MALE_QN_DAXUESHENG "male-qn-daxuesheng" // 青年大学生音色
#define FEMALE_SHAONV "female-shaonv" // 少女音色
#define FEMALE_YUJIE "female-yujie" // 御姐音色
#define FEMALE_CHENGSHU "female-chengshu" // 成熟女性音色
#define FEMALE_TIANMEI "female-tianmei" // 甜美女性音色
#define PRESENTER_MALE "presenter_male" // 男性主持人
#define PRESENTER_FEMALE "presenter_female" // 女性主持人
#define AUDIOBOOK_MALE_1 "audiobook_male_1" // 男性有声书1
#define AUDIOBOOK_MALE_2 "audiobook_male_2" // 男性有声书2
#define AUDIOBOOK_FEMALE_1 "audiobook_female_1" // 女性有声书1
#define AUDIOBOOK_FEMALE_2 "audiobook_female_2" // 女性有声书2
#define MALE_QN_QINGSE_JINGPIN "male-qn-qingse-jingpin" // 青涩青年音色-beta
#define MALE_QN_JINGYING_JINGPIN "male-qn-jingying-jingpin" // 精英青年音色-beta
#define MALE_QN_BADAO_JINGPIN "male-qn-badao-jingpin" // 霸道青年音色-beta
#define MALE_QN_DAXUESHENG_JINGPIN "male-qn-daxuesheng-jingpin" // 青年大学生音色-beta
#define FEMALE_SHAONV_JINGPIN "female-shaonv-jingpin" // 少女音色-beta
#define FEMALE_YUJIE_JINGPIN "female-yujie-jingpin" // 御姐音色-beta
#define FEMALE_CHENGSHU_JINGPIN "female-chengshu-jingpin" // 成熟女性音色-beta
#define FEMALE_TIANMEI_JINGPIN "female-tianmei-jingpin" // 甜美女性音色-beta
#define CLEVER_BOY "clever_boy" // 聪明男童
#define CUTE_BOY "cute_boy" // 可爱男童
#define LOVELY_GIRL "lovely_girl" // 萌萌女童
#define CARTOON_PIG "cartoon_pig" // 卡通猪小琪
#define BINGJIAO_DIDI "bingjiao_didi" // 病娇弟弟
#define JUNLANG_NANYOU "junlang_nanyou" // 俊朗男友
#define CHUNZHEN_XUEDI "chunzhen_xuedi" // 纯真学弟
#define LENGDAN_XIONGZHANG "lengdan_xiongzhang" // 冷淡学长
#define BADAO_SHAOYE "badao_shaoye" // 霸道少爷
#define TIANXIN_XIAOLING "tianxin_xiaoling" // 甜心小玲
#define QIAOPI_MENGMEI "qiaopi_mengmei" // 俏皮萌妹
#define WUMEI_YUJIE "wumei_yujie" // 妩媚御姐
#define DIADIA_XUEMEI "diadia_xuemei" // 嗲嗲学妹
#define DANYA_XUEJIE "danya_xuejie" // 淡雅学姐
#define SANTA_CLAUS "Santa_Claus" // Santa Claus
#define GRINCH "Grinch" // Grinch
#define RUDOLPH "Rudolph" // Rudolph
#define ARNOLD "Arnold" // Arnold
#define CHARMING_SANTA "Charming_Santa" // Charming Santa
#define CHARMING_LADY "Charming_Lady" // Charming Lady
#define SWEET_GIRL "Sweet_Girl" // Sweet Girl
#define CUTE_ELF "Cute_Elf" // Cute Elf
#define ATTRACTIVE_GIRL "Attractive_Girl" // Attractive Girl
#define SERENE_WOMAN "Serene_Woman" // Serene Woman

// 声明 voiceTypeStrings 数组为 extern
extern const char* voiceTypeStrings[];

// 音色选择
typedef struct
{
    uint8_t voice_num;
    const char* voice_name[1]; // 改为数组形式
} voice_t;

void MAX98357_Init(void) ;
String Text_conversion(String inputText) ;
//设置语音
void MAX98357_SetText(String inputText) ;
void MAX98357_loop(void);
//设置音色
void MAX98357_Settimbre(String inputText);
//设置声音大小
void MAX98357_SetSound_volume(uint8_t volume);
//设置语速
void MAX98357_SetSpeed(float speed);

#endif /* MAX98357_MINIMAX_H_ */
