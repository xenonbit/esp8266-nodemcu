#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include "services/LOGService.h"
#include "services/ROMService.h"
#include "services/CommandService.h"
#include "services/WiFiService.h"

#define SSID_MAX_LEN 33
#define PASS_MAX_LEN 64

#define SSID_START_ADDR 0
#define PASS_START_ADDR (SSID_START_ADDR + SSID_MAX_LEN)
#define EEPROM_SIZE (PASS_START_ADDR + PASS_MAX_LEN)

const char *targetUrl = "http://www.example.com/";

void testLED()
{
  LOGService::divider("LED Test");
  pinMode(LED_BUILTIN, OUTPUT);
  for (int i = 0; i < 3; i++)
  {
    LOGService::info("LED ON\n");
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
    LOGService::info("LED OFF\n");
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
  }
  LOGService::divider("LED Test Completed");
}

void testWiFi()
{
  LOGService::divider("WiFi Test");
  String ssid = ROMService::readString(SSID_START_ADDR, SSID_MAX_LEN);
  String password = ROMService::readString(PASS_START_ADDR, PASS_MAX_LEN);
  if (ssid.length() > 0 && password.length() > 0)
  {
    LOGService::info(("Attempting to connect with saved credentials: SSID=" + ssid + "\n").c_str());
    WiFiService::connect(ssid.c_str(), password.c_str());
    if (!WiFiService::waitForConnection(true))
    {
      LOGService::info("Failed to connect with saved credentials.\n");
    }
  }
  if (!WiFiService::isConnected())
  {
    LOGService::info("Enter SSID:\n");
    ssid = CommandService::waitForInput();
    LOGService::info("Enter Password:\n");
    password = CommandService::waitForInput();
    ROMService::writeString(SSID_START_ADDR, ssid, SSID_MAX_LEN);
    ROMService::writeString(PASS_START_ADDR, password, PASS_MAX_LEN);
    LOGService::info("Credentials saved to ROM.\n");
    WiFiService::connect(ssid.c_str(), password.c_str());
    WiFiService::waitForConnection(true);
  }
  if (WiFiService::isConnected())
  {
    LOGService::success("WiFi Connected!\n");
    LOGService::info(("IP Address: " + WiFiService::localIP().toString() + "\n").c_str());
    LOGService::info(("MAC Address: " + WiFiService::macAddress() + "\n").c_str());
    LOGService::info("RSSI: %d dBm\n", WiFiService::RSSI());
  }
  else
  {
    LOGService::error("WiFi not Connected!\n");
  }
  LOGService::divider("WiFi Test Completed");
}

void testHttpRequest()
{
  LOGService::divider("HTTP Request Test");
  if (WiFiService::isConnected())
  {
    LOGService::info("URL: %s\n", targetUrl);
    HTTPClient http;
    WiFiClient client;
    http.begin(client, targetUrl);
    int httpCode = http.GET();
    if (httpCode > 0)
    {
      if (httpCode == HTTP_CODE_OK)
      {
        LOGService::info("HTTP status code: %d\n", httpCode);
      }
      else
      {
        LOGService::error("HTTP status code: %d\n", httpCode);
      }
    }
    else
    {
      LOGService::error("HTTP GET request failed! Error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  }
  else
  {
    LOGService::error("WiFi not Connected!\n");
  }
  LOGService::divider("HTTP Request Test Completed");
}

void test()
{
  LOGService::divider("Testing");
  testLED();
  testWiFi();
  testHttpRequest();
  WiFiService::disconnect();
  LOGService::divider("Test Completed");
}

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println();
  LOGService::divider("Booting");
  ROMService::initialize(EEPROM_SIZE);
  LOGService::info("ROM initialized with size: %d bytes.\n", EEPROM_SIZE);
  LOGService::divider("Booted");
  LOGService::info("Enter command:\n");
}

void loop()
{
  String command = CommandService::waitForInput();
  if (command == "test")
  {
    test();
  }
  else if (command == "reset")
  {
    LOGService::info("Clearing ROM...\n");
    ROMService::clear();
    LOGService::success("ROM cleared.\n");
  }
  else
  {
    LOGService::info("Unknown command: %s", command);
  }
  LOGService::info("Enter command:\n");
}