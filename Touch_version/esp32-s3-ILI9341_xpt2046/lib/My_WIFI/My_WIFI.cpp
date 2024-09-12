#include "MY_WIFI.h"

void WIFI::WIFI_Connect(void)
{
    WiFi.mode(WIFI_STA);
    WiFi.begin();
    while (WiFi.status()!= WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());
}

void WIFI::WIFI_Connect(string ssid)
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str());
    while (WiFi.status()!= WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Connected, MAC address: ");
    Serial.println(WiFi.macAddress());
}

void WIFI::WIFI_Connect(string ssid, string password)
{
    Myssid = ssid;
    Mypassword = password;
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(),password.c_str());
    while (WiFi.status()!= WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Connected, MAC address: ");
    Serial.println(WiFi.macAddress());
}

uint32_t WIFI::WIFI_GetIP(void)
{
    return WiFi.localIP();
}

void WIFI::WIFI_Disconnect(void)
{
    WiFi.disconnect();
}

string WIFI::WIIF_SSID(void)
{
    return Myssid;
}

string WIFI::WIFI_Password(void)
{
    return Mypassword;
}
