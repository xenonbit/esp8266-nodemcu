#include "WiFiService.h"
#include <ESP8266WiFi.h>
#include "LOGService.h"

namespace WiFiService
{
    const long WIFI_TIMEOUT_MS = 15000;

    void connect(const char *ssid, const char *passphrase)
    {
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, passphrase);
    }

    bool isConnected()
    {
        return WiFi.status() == WL_CONNECTED;
    }

    void disconnect(bool wifioff)
    {
        if (!isConnected())
        {
            return;
        }
        WiFi.disconnect(wifioff);
    }

    bool waitForConnection(bool serialOut)
    {
        unsigned long startTime = millis();
        unsigned long difference = millis() - startTime;

        while (!isConnected() && ((difference = millis() - startTime) < WIFI_TIMEOUT_MS))
        {
            if (serialOut)
            {
                LOGService::drawProgressBar(difference, WIFI_TIMEOUT_MS, 50);
            }
            delay(1000);
        }

        if (serialOut)
        {
            LOGService::drawProgressBar(WIFI_TIMEOUT_MS, WIFI_TIMEOUT_MS, 50);
        }

        return isConnected();
    }

    IPAddress localIP()
    {
        return WiFi.localIP();
    }
    String macAddress()
    {
        return WiFi.macAddress();
    }
    int8_t RSSI()
    {
        return WiFi.RSSI();
    }
}