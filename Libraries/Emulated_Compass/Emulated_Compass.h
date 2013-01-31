#ifndef COMPASS_H_
#define COMPASS_H_

#include "WProgram.h"
#include <Wire.h>

class Compass
{
  private:
    int HMCAdrs;
    int slaveAdrs;
    byte headingData[2];
    int i;
    int headingVal;

    //For delay
    int req_time;

  public:
    int count;

    //**Constructor, declares all variables needed
    Compass();

    //**Setup for setup function, initializes addresses & starts "Wire"
    void setup();

    //**Data functions for loop
    double req_read();

    //The compass needs at least 70 microseconds delay after request
    // command, so if we do them separately, we can do something else
    // in that time instead of just delaying and doing nothing
    //Doing them separately can save time if we
    void request();
    double read();

    char direction();
    void serial_print();
};



#endif
