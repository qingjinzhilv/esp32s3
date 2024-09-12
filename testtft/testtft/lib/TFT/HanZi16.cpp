#include "HanZi16.h"
#include <TFT_eSPI.h>
void drawHanzi(const char c[3], uint32_t color, TFT_eSPI tft)
{ // 显示单一汉字
  int32_t x = tft.getCursorX();
  int32_t y = tft.getCursorY();
  for (int k = 0; k < 10; k++)
    if (hanzi16[k].Index[0] == c[0] && hanzi16[k].Index[1] == c[1] && hanzi16[k].Index[2] == c[2])
    {
      tft.drawBitmap(x, y, hanzi16[k].hz16_Id, hanzi16[k].hz_width, 16, color);
    }
}
void drawHanzi_Y(int32_t x, int32_t y, const char str[], uint32_t color, TFT_eSPI tft)
{
  // 显示整句汉字，字库目前有限，比较简单，没有换行功能，上下输出，左右输出是在函数内实现
  int y0 = y;
  for (int i = 0; i < strlen(str); i += 3)
  {
    drawHanzi(str + i, color, tft);
    tft.setCursor(tft.getCursorX(),tft.getCursorY()+20);
  }
}
void drawHanzi_X(int32_t x, int32_t y, const char str[], uint32_t color, TFT_eSPI tft)
{
  // 显示整句汉字，字库目前有限，比较简单，没有换行功能，上下输出，左右输出是在函数内实现
  int x0 = x;
  for (int i = 0; i < strlen(str); i += 3)
  {
    drawHanzi(str + i, color, tft);
    tft.setCursor(tft.getCursorX()+16,tft.getCursorY());
  }
}