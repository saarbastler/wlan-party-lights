#include "pages.h"
#include "files.h"
#include "lichterkette.h"

const char MIME_HTML[] PROGMEM = "text/html";
const char ACTION_OFF[] PROGMEM = "off";
const char ACTION_WHITE[] PROGMEM = "white";
const char ACTION_COLORED[] PROGMEM = "colored";
const char ACTION_BRIGHTER[] PROGMEM = "brighter";
const char ACTION_DARKER[] PROGMEM = "darker";
const char ACTION_GLOW[] PROGMEM = "glow";
const char ACTION_RUN[] PROGMEM = "run";
const char ACTION_WIFI[] PROGMEM = "wifi";

void addLamp(String& page, const char *color)
{
  page += FPSTR("<img src=\"/glas.svg?c=");
  page += FPSTR(color);
  page += FPSTR("\">");
}

void addLamps(String& page, const char *color)
{
  addLamp(page, color);
  addLamp(page, color);
  addLamp(page, color);
}

void addButton(String& page, const char *action, const char *title, bool selected= false)
{
  page += FPSTR("<p><button name=\"a\" value=\"");
  page += FPSTR(action);
  if( selected )
    page += FPSTR("\" class=\"sel\"><bb>");
  else
    page += FPSTR("\"><bb>");

  page += FPSTR(title);
  page += FPSTR("</bb></button>");
}

/** Is this an IP? */
boolean /*WiFiManager::*/isIp(String str)
{
  for (size_t i = 0; i < str.length(); i++)
  {
    int c = str.charAt(i);
    if (c != '.' && (c < '0' || c > '9'))
    {
      return false;
    }
  }
  return true;
}

/** IP to String? */
String /*WiFiManager::*/toStringIp(IPAddress ip)
{
  String res = "";
  for (int i = 0; i < 3; i++)
  {
    res += String((ip >> (8 * i)) & 0xFF) + ".";
  }
  res += String(((ip >> 8 * 3)) & 0xFF);
  return res;
}

/** Redirect to captive portal if we got a request for another domain. Return true in that case so the page handler do not try to handle the request again. */
boolean /*WiFiManager::*/captivePortal()
{
  if (!isIp(server->hostHeader()) )
  {
    Serial.println(F("Request redirected to captive portal"));
    server->sendHeader("Location", String("http://") + toStringIp(server->client().localIP()), true);
    server->send ( 302, "text/plain", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
    server->client().stop(); // Stop is needed because we sent no content length
    return true;
  }
  return false;
}

void pageIndex()
{
  Serial.println(server->uri());
  Serial.println(server->hostHeader());

  if (captivePortal())
    return;

  if(server->hasArg("a"))
  {
    String action= server->arg("a");

    if( action == FPSTR(ACTION_OFF))
      setMode(Off);
    else if(action == FPSTR(ACTION_WHITE))
      setMode(OnlyWhite);
    else if(action == FPSTR(ACTION_COLORED))
      setMode(Colored);
    else if(action == FPSTR(ACTION_GLOW))
      setMode(Glow);
    else if(action == FPSTR(ACTION_RUN))
      setMode(Run);
    else if(action == FPSTR(ACTION_BRIGHTER))
      brighter();
    else if(action == FPSTR(ACTION_DARKER))
      darker();
    else if(action == FPSTR(ACTION_WIFI))
    {
      if(WiFi.status() == WL_CONNECTED)
        WiFi.disconnect(true);

      ESP.reset();
    }
  }

  String page = FPSTR(INDEX_HTML_PREFIX);
  switch(mode)
  {
    case OnlyWhite:
      addLamps(page, PSTR("fff"));
      break;
    case Colored:
      addLamp(page, PSTR("f00"));
      addLamp(page, PSTR("00f"));
      addLamp(page, PSTR("0ff"));
      break;
    case Glow:
      addLamp(page, PSTR("060"));
      addLamp(page, PSTR("0f0"));
      addLamp(page, PSTR("060"));
      break;
    case Run:
      addLamp(page, PSTR("000"));
      addLamp(page, PSTR("00f"));
      addLamp(page, PSTR("000"));
      break;
    case Off:
    default:
      addLamps(page, PSTR("111"));
  }

  addButton(page, ACTION_OFF, PSTR("Aus"), mode == Off);
  addButton(page, ACTION_WHITE, PSTR("Weiß"), mode == OnlyWhite);
  addButton(page, ACTION_COLORED, PSTR("Bunt"), mode == Colored);

  uint8_t brightness= pixels.getBrightness();

  if(mode == OnlyWhite || mode == Colored)
  {
    if( brightness < 255)
      addButton(page, ACTION_BRIGHTER, PSTR("Heller"));

    if( brightness > 11)
      addButton(page, ACTION_DARKER, PSTR("Dunkler"));
  }

  addButton(page, ACTION_GLOW, PSTR("Glimmen"), mode == Glow);
  addButton(page, ACTION_RUN, PSTR("Lauflicht"), mode == Run);
  addButton(page, ACTION_WIFI, PSTR("WLan"), false);

  page += FPSTR(INDEX_HTML_SUFFIX);

  server->send(200, "text/html", page);
}

void pageForm(uint8_t r, uint8_t g, uint8_t b, uint8_t w)
{
  String page = FPSTR(FORM_HTML);
  page.replace("{r}", String(r));
  page.replace("{g}", String(g));
  page.replace("{b}", String(b));
  page.replace("{w}", String(w));

  server->send(200, "text/html", page);
}

void pageForm()
{
  pageForm(0,0,0,0);
}

void rgb()
{
  if(server->hasArg("r") && server->hasArg("g") && server->hasArg("b") )
  {
    uint8_t r= server->arg("r").toInt();
    uint8_t g= server->arg("g").toInt();
    uint8_t b= server->arg("b").toInt();
    uint8_t w= 0;
    if( server->hasArg("w"))
      w= server->arg("w").toInt();

    setAll( r, g, b, w);
    //Serial.printf("R:%d G:%d B:%d W:%d\n", r,g,b,w);

    pageForm(r, g, b, w);
  }
  else
  {
    server->send(400, "text/plain", "missing arguments");
  }
}

void glas()
{
  String svg= FPSTR(GLAS_SVG);
  if(server->hasArg("c"))
  {
    String col= server->arg("c");
    if(col.length() == 3 || col.length() == 6)
    {
      unsigned i;
      for(i=0;i < col.length();i++)
      {
        char c= col[i];
        if( c < '0' << (c > '9' && c < 'A') || (c > 'F' && c < 'a') || c > 'f')
          break;
      }
      if( i == col.length())
        svg.replace("00f", col);
    }
  }

  server->send(200,"image/svg+xml", svg);
}

void handleNotFound()
{
  if (captivePortal()) // If captive portal redirect instead of displaying the error page.
   return;

  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server->uri();
  message += "\nMethod: ";
  message += (server->method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server->args();
  message += "\n";
  for (uint8_t i=0; i<server->args(); i++){
    message += " " + server->argName(i) + ": " + server->arg(i) + "\n";
  }
  server->send(404, "text/plain", message);

  Serial.println(message);
}
