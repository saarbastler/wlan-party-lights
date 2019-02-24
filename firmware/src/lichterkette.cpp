#include <pgmspace.h>

#include "lichterkette.h"

//Adafruit_NeoPixel pixels(NUMBER_OF_PIXELS, D1, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel pixels(NUMBER_OF_PIXELS, 2, NEO_GRBW + NEO_KHZ800);
Mode mode;

const uint32_t COLORS[] PROGMEM = {
  0xffff00, 0xffC000, 0xff8000, 0xff4000, 0xff0000,
  0x00ffff, 0x00ffC0, 0x00ff80, 0x00ff40, 0x00ff00,
  0xff00ff, 0xc000ff, 0x8000ff, 0x4000ff, 0x0000ff,
  0xffffff, 0xffC0ff, 0xff80ff, 0xff40ff,
  0xc0ffff, 0x80ffff, 0x40ffff,
  0xffffc0, 0xffff80, 0xffff40
};

const uint32_t RUN_COLORS[] PROGMEM = {
  0x000000ff, 0x0000ff00, 0x00ff0000,
  0x0000ffff, 0x00ffff00, 0x00ffff00, 0x00ff00ff,
  0xff000000
};

uint32_t colors[NUMBER_OF_PIXELS];
uint8_t pixelIndex[NUMBER_OF_PIXELS];
uint8_t lauflichtIndex;
uint8_t colorIndex;
uint8_t counter;

uint32_t createColor(uint8_t index)
{
  index %= sizeof(COLORS)/sizeof(COLORS[0]);

  uint32_t result;
  memcpy_P(&result, &COLORS[index], 4);
  //printf("Col %02x: %08x\n", index, result);

  return result;
}

void transfer()
{
  pixels.begin();
  for(uint16_t i=0;i < NUMBER_OF_PIXELS;++i)
    pixels.setPixelColor(i, colors[i]);
  pixels.show();
}

void brighter()
{
  uint8_t b= pixels.getBrightness();
  uint8_t diff= b >> 3;
  if( diff < 16 )
    diff= 16;

  if( b + diff >= 255)
    b= 255;
  else
    b += diff;

//Serial.printf("br: %d\n", b);
  pixels.setBrightness(b);
  transfer();
}

void darker()
{
  uint8_t b= pixels.getBrightness();
  uint8_t diff= b >> 3;
  if( diff < 16 )
    diff= 16;

  if( b > diff )
    b-= diff;

//Serial.printf("da: %d\n", b);
  pixels.setBrightness(b);
  transfer();
}

void glow()
{
  for(unsigned i=0;i < NUMBER_OF_PIXELS;++i)
  {
    uint32_t col= colors[i];
    uint8_t pix= pixelIndex[i] & 3;

    uint8_t *ptr= (uint8_t *)&col;

    uint8_t *down= ptr + pix;
    uint8_t *up= ptr + ((pix + 1) & 3);

    if( *up < 240 )
    {
      *up += 8;
      if(*down > 8)
        *down -= 8;
    }
    else
    {
      pixelIndex[i]= (pix + 1) & 3;
    }

    //Serial.printf("PI: %d *UP:%d *DOWN:%d C:%08x\n", pixelIndex, *up, *down, col);
    colors[i]=  col;
  }

  transfer();
}

void run()
{
  if(lauflichtIndex > NUMBER_OF_PIXELS)
    lauflichtIndex= 0;
  if( colorIndex > sizeof(RUN_COLORS)/sizeof(RUN_COLORS[0]))
    colorIndex= 0;

  uint32_t color= colors[lauflichtIndex];

  if( counter < 255-25 )
  {
    counter += 25;
  }
  else
  {
    counter = 5;

    lauflichtIndex++;
    if( lauflichtIndex >= NUMBER_OF_PIXELS)
    {
      lauflichtIndex= 0;
      colorIndex++;
      if( colorIndex >= sizeof(RUN_COLORS)/sizeof(RUN_COLORS[0]))
        colorIndex= 0;
    }

    //Serial.printf("li: %d ci: %d ec: %08x\n", lauflichtIndex, colorIndex, endColor);
  }

  uint32_t endColor;
  memcpy_P(&endColor, &RUN_COLORS[colorIndex], 4);

  uint8_t c1= (uint8_t(endColor >> 24) * counter) >> 8;
  uint8_t c2= (uint8_t(endColor >> 16) * counter) >> 8;
  uint8_t c3= (uint8_t(endColor >> 8) * counter) >> 8;
  uint8_t c4= (uint8_t(endColor) * counter) >> 8;

  color= (c1 << 24) | (c2 << 16) | (c3 << 8) | c4;
  colors[lauflichtIndex]= color;
  transfer();
}

void setAll( uint8_t r, uint8_t g, uint8_t b, uint8_t w)
{
  for(unsigned i=0;i < NUMBER_OF_PIXELS;++i)
    colors[i]= pixels.Color(r,g,b,w);

  transfer();
}

void setRandom()
{
  for(unsigned i=0;i < NUMBER_OF_PIXELS;++i)
  {
    uint8_t rnd= random(0,sizeof(COLORS)/sizeof(COLORS[0]));
    colors[i]= createColor( rnd );
  }
  transfer();
}

void setRandomPixelIndex()
{
  for(unsigned i=0;i < NUMBER_OF_PIXELS;++i)
    pixelIndex[i] = random(0, 3);
}

void setMode(Mode newMode)
{
  switch(newMode)
  {
  case Off:
    setAll(0, 0, 0, 0);
    break;

  case OnlyWhite:
    setAll(0, 0, 0, 255);
    break;

  case Colored:
    setRandom();
    break;

  case Glow:
    pixels.setBrightness(255);
    setRandomPixelIndex();
    break;

  case Run:
    pixels.setBrightness(255);
    setAll(0, 0, 0, 0);
    lauflichtIndex= 0;
    colorIndex= 0;
    counter= 5;
    break;
  }

  Serial.printf("Mode %d -> %d\n", mode, newMode);

  mode= newMode;
}
