#ifndef MY_WIFI_H
#define MY_WIFI_H

#include <iostream>
#include "WiFi.h"

using namespace std;


class WIFI
{
    private:
        string Myssid;
        string Mypassword;
    public:
        void WIFI_Connect(void);
        void WIFI_Connect(string ssid);
        void WIFI_Connect(string ssid, string password);
        void WIFI_Disconnect(void);
        string WIIF_SSID(void);
        string WIFI_Password(void);
        uint32_t WIFI_GetIP(void);
};



#endif
