#ifndef _INCLUDE_DEBOUNCE_INPUT_H_
#define _INCLUDE_DEBOUNCE_INPUT_H_

#include <Arduino.h>

template<uint8_t INPUT_INDEX, unsigned long DEBOUNCE_TIME = 5>
class DebounceInput
{
public:

  void setup()
  {
    pinMode(INPUT_INDEX , INPUT_PULLUP);
    lastValue= (digitalRead(INPUT_INDEX) != LOW);
  }

  template<class Callback>
  void cyclic(Callback&& callback)
  {
    bool value= (digitalRead(INPUT_INDEX) != LOW);
    if( waiting )
    {
      unsigned long diff= millis() - lastChange;
      if( diff > DEBOUNCE_TIME )
      {
        waiting= false;
        if( value != lastValue )
        {
          lastValue= value;
          callback(lastValue, duration);
        }
      }
    }
    else
    {
      if(value != lastValue)
      {
        waiting= true;
        unsigned long now= millis();
        duration= now - lastChange;
        lastChange= now;
      }
    }
  }

private:

  unsigned long duration = 0;
  unsigned long lastChange;
  bool lastValue;
  bool waiting= false;
};

#endif
