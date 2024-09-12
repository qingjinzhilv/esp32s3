// #include "My_GetTime.h"

// HTTPClient http;
// WiFiClient httpclient;

// /*
//  * 功能：设置时间函数
//  * 形参：无
//  * 返回值：无
// */
// void GetTime_t::GetTime_Set(void)
// {
//     (this->My_Time_Data).year = 2024;
//     (this->My_Time_Data).month = 9;
//     (this->My_Time_Data).day = 5;
//     (this->My_Time_Data).hour = 18;
//     (this->My_Time_Data).minute = 1;
//     (this->My_Time_Data).second = 00;
// }
// /*
//  * 功能：设置时间函数
//  * 形参：年月日
//  * 返回值：无
// */
// void GetTime_t::GetTime_Set(uint16_t Year,uint8_t Month,uint8_t Day)
// {
//     (this->My_Time_Data).year = Year;
//     (this->My_Time_Data).month = Month;
//     (this->My_Time_Data).day = Day;
//     (this->My_Time_Data).hour = 18;
//     (this->My_Time_Data).minute = 1;
//     (this->My_Time_Data).second = 00;
// }
// /*
//  * 功能：设置时间函数
//  * 形参：时分秒
//  * 返回值：无
// */
// void GetTime_t::GetTime_Set(uint8_t Hour,uint8_t Minute,uint8_t Second)
// {
//     (this->My_Time_Data).year = 2024;
//     (this->My_Time_Data).month = 9;
//     (this->My_Time_Data).day = 5;
//     (this->My_Time_Data).hour = Hour;
//     (this->My_Time_Data).minute = Minute;
//     (this->My_Time_Data).second = Second;
// }
// /*
//  * 功能：设置时间函数
//  * 形参：年月日时分秒
//  * 返回值：无
// */
// void GetTime_t::GetTime_Set(uint16_t Year,uint8_t Month,uint8_t Day,uint8_t Hour,uint8_t Minute,uint8_t Second)
// {
//     (this->My_Time_Data).year = Year;
//     (this->My_Time_Data).month = Month;
//     (this->My_Time_Data).day = Day;
//     (this->My_Time_Data).hour = Hour;
//     (this->My_Time_Data).minute = Minute;
//     (this->My_Time_Data).second = Second;
// }

// /*
//  * 功能：时间获取函数
//  * 形参：无
//  * 返回值：时间字符串
// */
// int httpCode = 0;
// String GetTime_t::GetTime_Str(void)
// {
//     String response = "  ";
//     if(httpCode != 200)
//     {
//         // 连接苏宁网站的授时网页
//         String GetUrl = "http://quan.suning.com/getSysTime.do";
//         http.setTimeout(5000);
//         http.begin(httpclient,GetUrl);
//         delay(100);
//         // 从网站获得网页内容
//         httpCode = http.GET();
        
//         if (httpCode > 0) 
//         {
//             Serial.printf("[HTTP] GET... code: %d\n", httpCode);
//             if (httpCode == HTTP_CODE_OK)
//             {
//                 //读取响应内容
//                 response = http.getString();
//             }
//         } 
//         else 
//         {
//             response = "  ";
//             httpCode = 0;
//             Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
//         }
//         http.end();
//     }
//     return response;
// }

// /*
//  * 功能：时间解析函数
//  * 形参：无
//  * 返回值：无
// */
// void GetTime_t::GetTime_Data_Analyze(String time_str)
// {
//     // String time_str = GetTime_Str();
//     Serial.println(time_str);
//     char buf[100],n = 0,at[3];
//     n = time_str.lastIndexOf("sysTime1\":\"");
//     time_str.toCharArray(buf,100);
//     strncpy(at,buf + n + 13,2);
//     (this->My_Time_Data).year = 2000 + atoi(at);
//     strncpy(at,buf + n + 15,2);
//     (this->My_Time_Data).month = atoi(at);
//     strncpy(at,buf + n + 17,2);
//     (this->My_Time_Data).day = atoi(at);
//     strncpy(at,buf + n + 19,2);
//     (this->My_Time_Data).hour = atoi(at);
//     strncpy(at,buf + n + 21,2);
//     (this->My_Time_Data).minute = atoi(at);
//     strncpy(at,buf + n + 23,2);
//     (this->My_Time_Data).second = atoi(at);
// }
