#ifndef LICHTERKETTE_H_
#define LICHTERKETTE_H_

#include <Adafruit_NeoPixel.h>

extern Adafruit_NeoPixel pixels;

#define NUMBER_OF_PIXELS    25

enum Mode
{
  Off, OnlyWhite, Colored, Glow, Run
};

extern Mode mode;
extern void setMode(Mode mode);

extern void setAll( uint8_t r, uint8_t g, uint8_t b, uint8_t w);
extern void brighter();
extern void darker();
extern void glow();
extern void run();

#endif // LICHTERKETTE_H_
