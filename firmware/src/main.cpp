#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

#define DEBUG_ESP_HTTP_SERVER

//needed for library
#include <DNSServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager

#include <memory>

#include "files.h"
#include "pages.h"
#include "lichterkette.h"
#include "DebounceInput.h"

std::unique_ptr<ESP8266WebServer> server;
std::unique_ptr<DNSServer> dnsServer;

unsigned long lastMillis;

#define BUTTON  0
// DNS server
constexpr byte    DNS_PORT = 53;

DebounceInput<BUTTON> button;
/*void lk()
{
  server->send_P(200,"image/png", data_lk_png, size_lk_png);
}*/

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  button.setup();

  setAll(50, 0, 0, 0);

  char name[]= "lichterkette";
  wifi_station_set_hostname(name);

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  wifiManager.setDebugOutput(false);
  wifiManager.setConfigPortalTimeout(30);
  //reset saved settings
  //wifiManager.resetSettings();

  wifiManager.autoConnect(name);

  if(WiFi.status() != WL_CONNECTED)
  {
    IPAddress ip(192,168,200,200);
    IPAddress subnet(255,255,255,0);

    dnsServer.reset(new DNSServer());

    WiFi.mode(WIFI_AP);

    WiFi.softAPConfig(ip, ip, subnet);

    WiFi.softAP("licherkette");

    delay(500); // Without delay I've seen the IP address blank
    Serial.print("Soft-AP IP address = ");
    Serial.println(WiFi.softAPIP());

    Serial.print("Starting Webserver at: ");
    Serial.println(ip);

    dnsServer->setErrorReplyCode(DNSReplyCode::NoError);
    dnsServer->start(DNS_PORT, "*", ip);
  }

  server.reset(new ESP8266WebServer(80));

  server->on(F("/"), HTTP_GET, pageIndex);
  server->on(F("/rgb"), HTTP_GET, rgb);
  server->on(F("/form"), HTTP_GET, pageForm);
  server->on(F("/glas.svg"), HTTP_GET, glas);
  //server->on("/col", rgb);
  server->onNotFound(handleNotFound);
  server->begin();

  Serial.println( "Webserver is running");

  setAll(0, 50, 0, 0);

  lastMillis= millis();
  setMode( Colored );
}

void buttonClicked(bool value, unsigned long duration)
{
  Serial.print("Button: ");
  Serial.print(value);
  Serial.print(" for ");
  Serial.print(duration);
  Serial.println();

  if( value == true && duration > 10000)
  {
    setMode( Off );

    WiFiManager wifiManager;
    wifiManager.resetSettings();
    ESP.reset();
  }
  else if(!value)
  {
    if( mode == Run )
      setMode( Off );
    else
      setMode( Mode(mode + 1));
  }
}

void loop()
{
  if(dnsServer)
    dnsServer->processNextRequest();

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

  button.cyclic(buttonClicked);
}
