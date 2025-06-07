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

void testWebsite()
{
  Serial.println("=== Website Test ===");

  HTTPClient http;
  WiFiClient client;

  http.begin(client, "http://www.example.com/");

  int httpCode = http.GET();

  if (httpCode > 0)
  {
    LOGService::info("HTTP GET request successful! HTTP Status Code: %d\n", httpCode);

    if (httpCode == HTTP_CODE_OK)
    {
      String payload = http.getString();
      Serial.println("--- Response Payload (first 200 chars) ---");
      Serial.println(payload.substring(0, min((int)payload.length(), 200)));
      Serial.println("----------------------------------------");
    }
    else
    {
      LOGService::info("Received non-OK HTTP status code: %d\n", httpCode);
    }
  }
  else
  {
    LOGService::error("HTTP GET request failed! Error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
  Serial.println("=== Website Test Completed ===\n");
}

void test()
{
  Serial.println("====== Testing ======\n");
  delay(1000);
  Serial.println("=== LED Test ===");
  delay(1000);
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
  delay(1000);

  Serial.println("=== LED Test Completed ===\n");
  Serial.println("=== WiFi Test ===");

  String ssid = ROMService::readString(SSID_START_ADDR, SSID_MAX_LEN);
  String password = ROMService::readString(PASS_START_ADDR, PASS_MAX_LEN);

  bool connected = false;

  WiFi.mode(WIFI_STA);

  if (ssid.length() > 0 && password.length() > 0)
  {
    LOGService::info(("Attempting to connect with saved credentials: SSID=" + ssid + "\n").c_str());
    WiFi.begin(ssid.c_str(), password.c_str());

    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED && (millis() - startTime < WIFI_TIMEOUT_MS))
    {
      delay(1000);
      Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED)
    {
      connected = true;
    }
    else
    {
      Serial.println();
      LOGService::info("Failed to connect with saved credentials.\n");
      WiFi.disconnect();
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
    while (WiFi.status() != WL_CONNECTED && (millis() - startTime < WIFI_TIMEOUT_MS))
    {
      delay(1000);
      Serial.print(".");
    }
  }

  Serial.println();

  if (WiFi.status() == WL_CONNECTED)
  {
    LOGService::info("WiFi Connected!\n");
    LOGService::info(("IP Address: " + WiFi.localIP().toString() + "\n").c_str());
    LOGService::info(("MAC Address: " + WiFi.macAddress() + "\n").c_str());
    LOGService::info("RSSI: %d dBm\n", WiFi.RSSI());
    Serial.println("=== WiFi Test Completed ===\n");

    testWebsite();

    WiFi.disconnect();
  }
  else
  {
    LOGService::error("WiFi not Connected!\n");
    WiFi.disconnect();
    Serial.println("=== WiFi Test Completed ===\n");
  }

  delay(1000);
  Serial.println("====== Test Completed ======\n");
}

void setup()
{
  Serial.begin(115200);

  delay(1000);

  Serial.println();

  LOGService::info("Booting...\n");
  ROMService::initialize(EEPROM_SIZE);
  LOGService::info("ROM initialized with size: %d bytes.\n", EEPROM_SIZE);
  LOGService::success("Booted.\n");
  LOGService::info("Enter command:\n");

  delay(1000);

  Serial.setTimeout(10);
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
    LOGService::info(("Unknown command: " + command + "\n").c_str());
  }

  LOGService::info("Enter command:\n");
}