#ifndef WiFiService_H
#define WiFiService_H
#include <ESP8266WiFi.h>
#include <Arduino.h>

namespace WiFiService
{
    IPAddress localIP();
    String macAddress();
    int8_t RSSI();
    void connect(const char *ssid, const char *passphrase = (const char *)__null);
    bool isConnected();
    void disconnect(bool wifioff = false);
    bool waitForConnection(bool serialOut = false);
}
#endif
