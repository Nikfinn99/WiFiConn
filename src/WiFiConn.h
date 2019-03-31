#ifndef wificonn_h
#define wificonn_h

#include <led.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>

class WiFiConn_CLASS
{

public:
  WiFiConn_CLASS() {}

  bool connect(const char *s = nullptr, const char *p = nullptr)
  { //connect to Wifi

    WiFi.mode(WIFI_STA);
    delay(1);

    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.print("*WC Already connected to ");
      Serial.println(WiFi.SSID());
      Serial.printf("*WC IP: %s\n", WiFi.localIP().toString().c_str());
      return true;
    }
    else if (WiFi.SSID().length() > 5 && WiFi.psk().length() >= 8)
    {
      Serial.println("*WC Using last saved values.");
      Serial.print("*WC Connecting to ");
      Serial.println(WiFi.SSID());

      WiFi.begin();
      return wait_for_connection();
    }
    else if (s && p && p[0] != '\0' && s[0] != '\0')
    {
      Serial.println("*WC Using user credentials");
      Serial.print("*WC Connecting to ");
      Serial.println(s);

      WiFi.begin(s, p);
      return wait_for_connection();
    }
    else
    {
      Serial.println("*WC no credentials available");

      WiFiManager wm;
      wm.startConfigPortal();

      ESP.restart();
      while (1)
      {
        delay(500);
        DEBUG_LED.toggle();
      }

      return false;
    }
  }

  void checkConnection()
  {
    if (WiFi.status() != WL_CONNECTED)
    {
      this->connect();
    }
  }

protected:
  bool wait_for_connection(const uint8_t max_seconds = 10, bool fatal = true)
  { //wait until connection established
    uint8_t i = 0;
    const uint8_t delay_ms = 50;
    Serial.print("*WC connecting.");
    while (WiFi.status() != WL_CONNECTED && i++ < (max_seconds * 1000) / delay_ms)
    { //wait with timeout
      delay(delay_ms);
      Serial.print(".");
    }
    if (i >= ((max_seconds * 1000) / delay_ms) + 1)
    { //connection unsuccesfull
      if (fatal)
      {
        Serial.print("\n");
        Serial.printf("*WC Could not connect to %s", WiFi.SSID().c_str());
        Serial.print("*WC Opening WifiManager");

        ESP.restart();
        while (1)
        {
          delay(500);
          DEBUG_LED.toggle();
        }
      }
      else
      {
        return false;
      }
    }
    else
    {
      Serial.printf(" connected! \n*WC IP: %s\n", WiFi.localIP().toString().c_str());
      return true;
    }
  }
};

WiFiConn_CLASS WiFiConn;

#endif
