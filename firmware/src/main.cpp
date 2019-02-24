#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

#define DEBUG_ESP_HTTP_SERVER

//needed for library
#include <DNSServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager

#include <memory>

//#include "lk.h"
#include "files.h"
#include "pages.h"
#include "lichterkette.h"

std::unique_ptr<ESP8266WebServer> server;

unsigned long lastMillis;

/*void lk()
{
  server->send_P(200,"image/png", data_lk_png, size_lk_png);
}*/

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  setAll(50, 0, 0, 0);

  char name[]= "lichterkette";
  wifi_station_set_hostname(name);

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  wifiManager.setDebugOutput(false);
  //reset saved settings
  //wifiManager.resetSettings();

  wifiManager.autoConnect(name);

  //delay(500);
  Serial.print("Starting Webserver at: ");
  Serial.println(WiFi.localIP());

  server.reset(new ESP8266WebServer(80));

  server->on("/", HTTP_GET, pageIndex);
  server->on("/rgb", HTTP_GET, rgb);
  server->on("/form", HTTP_GET, pageForm);
  server->on("/glas.svg", HTTP_GET, glas);
  //server->on("/col", rgb);
  server->onNotFound(handleNotFound);
  server->begin();

  Serial.println( "Webserver is running");

  setAll(0, 50, 0, 0);

  lastMillis= millis();
  setMode( Colored );
}

void loop()
{
  server->handleClient();

  unsigned long now= millis();
  if( now - lastMillis > 100)
  {
    if( mode == Glow)
      glow();
    else if (mode == Run)
      run();

    lastMillis= now;
  }
}
