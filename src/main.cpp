#include <heltec.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <main_macros.h>

const char *restService = "http://ubuntu-pi4:8081/filter-events/";

void connectWifi() 
{
  Heltec.display->init();
  Heltec.display->flipScreenVertically();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);

  Heltec.display->drawString(0, 0, "Connecting to WiFi...");
  Heltec.display->display();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("connecting to WiFi..");
    delay(2000);
  }

  Serial.println("connected to WiFi");
  Serial.print("ip: ");
  Serial.println(WiFi.localIP());
  Serial.print("gateway ip: ");
  Serial.println(WiFi.gatewayIP());

  Heltec.display->clear();
  Heltec.display->drawString(0, 0, "Connected to WiFi");
  Heltec.display->drawString(0, 10, "IP: ");
  Heltec.display->drawString(50, 10, WiFi.localIP().toString());
  Heltec.display->drawString(0, 20, "Gateway: ");
  Heltec.display->drawString(50, 20, WiFi.gatewayIP().toString());
  Heltec.display->display();

  delay(2000);
}

void disconnectWifi()
{
  Serial.println("### disconnecting from WiFi..");
  WiFi.disconnect();
}

void createFilterEvent(String name, String message)
{
  Serial.println("### create filter event");

  HTTPClient http;
  http.begin(restService);
  http.addHeader("Content-Type", "application/json");

  // prepare JSON
  DynamicJsonDocument doc(2048);
  doc["name"] = name;
  doc["message"] = message;

  // serialize JSON
  String json;
  serializeJson(doc, json);
  Serial.println(json);
  
  int httpCode = http.POST(json);
  Serial.print("http response code: ");
  Serial.println(httpCode);
  if (httpCode != 201)
  {
    Serial.println("error on HTTP request");
  }
  http.end();
}

void getFilterEvent(long id)
{
  Serial.println("### get filter event for id: " + (String)id);

  HTTPClient http;
  http.begin(restService + (String)id);
  int httpCode = http.GET();

  Serial.print("http response code: ");
  Serial.println(httpCode);

  if (httpCode == 200)
  {
    String payload = http.getString();
    Serial.print("payload: ");
    Serial.println(payload);

    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, payload);
    if (error)
    {
      Serial.print("deserialize json failed: ");
      Serial.println(error.f_str());
    }
    else
    {
      long id = doc["id"];
      const char *name = doc["name"];

      Heltec.display->clear();
      Heltec.display->drawString(0, 0, (String)id);
      Heltec.display->drawString(0, 10, name);
      Heltec.display->display();
    }
  }
  else
  {
    Serial.println("error on HTTP request");
  }

  http.end();
}

void setup()
{

  Serial.begin(115200);
  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Disable*/, true /*Serial Enable*/);

}

void loop()
{
  connectWifi();
  createFilterEvent("test", "esp32");
  getFilterEvent(-1);
  disconnectWifi();

  delay(10000);
}