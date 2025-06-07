#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "services/LOGService.h"
#include "services/ROMService.h"
#include "services/CommandService.h"

#define SSID_MAX_LEN 33
#define PASS_MAX_LEN 64

#define SSID_START_ADDR 0
#define PASS_START_ADDR (SSID_START_ADDR + SSID_MAX_LEN)
#define EEPROM_SIZE (PASS_START_ADDR + PASS_MAX_LEN)

const long WIFI_TIMEOUT_MS = 15000;
const char *targetUrl = "http://www.example.com/";

void testWebsite()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    return;
  }
  LOGService::divider("Website Test");
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
  WiFi.disconnect();
  LOGService::divider("Website Test Completed");
}

void testLED()
{
  LOGService::divider("LED Test");
  LOGService::info("LED ON\n");
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  LOGService::info("LED OFF\n");
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  LOGService::info("LED ON\n");
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  LOGService::info("LED OFF\n");
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  LOGService::info("LED ON\n");
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  LOGService::info("LED OFF\n");
  digitalWrite(LED_BUILTIN, HIGH);
  LOGService::divider("LED Test Completed");
}

void testWiFi()
{
  LOGService::divider("WiFi Test");
  String ssid = ROMService::readString(SSID_START_ADDR, SSID_MAX_LEN);
  String password = ROMService::readString(PASS_START_ADDR, PASS_MAX_LEN);
  bool connected = false;
  WiFi.mode(WIFI_STA);
  if (ssid.length() > 0 && password.length() > 0)
  {
    LOGService::info(("Attempting to connect with saved credentials: SSID=" + ssid + "\n").c_str());
    WiFi.begin(ssid.c_str(), password.c_str());

    unsigned long startTime = millis();
    unsigned long difference = millis() - startTime;
    while (WiFi.status() != WL_CONNECTED && ((difference = millis() - startTime) < WIFI_TIMEOUT_MS))
    {
      LOGService::drawProgressBar(difference, WIFI_TIMEOUT_MS, 50);
      delay(1000);
    }
    LOGService::drawProgressBar(WIFI_TIMEOUT_MS, WIFI_TIMEOUT_MS, 50);
    if (WiFi.status() == WL_CONNECTED)
    {
      connected = true;
    }
    else
    {
      LOGService::info("Failed to connect with saved credentials.\n");
    }
  }
  if (!connected)
  {
    LOGService::info("Enter SSID:\n");
    ssid = CommandService::waitForInput();
    LOGService::info("Enter Password:\n");
    password = CommandService::waitForInput();
    ROMService::writeString(SSID_START_ADDR, ssid, SSID_MAX_LEN);
    ROMService::writeString(PASS_START_ADDR, password, PASS_MAX_LEN);
    LOGService::info("Credentials saved to ROM.\n");
    WiFi.begin(ssid.c_str(), password.c_str());
    unsigned long startTime = millis();
    unsigned long difference = millis() - startTime;
    while (WiFi.status() != WL_CONNECTED && ((difference = millis() - startTime) < WIFI_TIMEOUT_MS))
    {
      LOGService::drawProgressBar(difference, WIFI_TIMEOUT_MS, 50);
      delay(1000);
    }
    LOGService::drawProgressBar(WIFI_TIMEOUT_MS, WIFI_TIMEOUT_MS, 50);
  }
  if (WiFi.status() == WL_CONNECTED)
  {
    LOGService::success("WiFi Connected!\n");
    LOGService::info(("IP Address: " + WiFi.localIP().toString() + "\n").c_str());
    LOGService::info(("MAC Address: " + WiFi.macAddress() + "\n").c_str());
    LOGService::info("RSSI: %d dBm\n", WiFi.RSSI());
    LOGService::divider("WiFi Test Completed");
  }
  else
  {
    LOGService::error("WiFi not Connected!\n");
    LOGService::divider("WiFi Test Completed");
  }
}

void test()
{
  LOGService::divider("Testing");
  testLED();
  testWiFi();
  testWebsite();
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