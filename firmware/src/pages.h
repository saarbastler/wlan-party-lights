#ifndef PAGES_H_
#define PAGES_H_

#include <memory>

#include <ESP8266WebServer.h>

extern std::unique_ptr<ESP8266WebServer> server;

extern void pageIndex();
//extern void pageForm();
//extern void pageForm(uint8_t r, uint8_t g, uint8_t b, uint8_t w);
extern void glas();
//extern void rgb();
extern void handleNotFound();

#endif // PAGES_H_
