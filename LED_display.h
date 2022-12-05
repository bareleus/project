/** Simple On-board LED flashing program - written by Derek Molloy
*    simple OOP  struture for the Exploring BeagleBone book
*
*    This program uses all four LEDS and can be executed in three ways:
*         myBlink on 1 2
*         myBlink off 1 2
*         myBlink flash 1 2  (flash at time delay intervals)
*         myBlink status 1 2 (get the trigger status)
*
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#ifndef LED_DISPLAY_H
#define LED_DISPLAY_H

using namespace std;

class LED{
   private:
      string path;
      int number;
      virtual void writeLED(string filename, string value);
      virtual void removeTrigger();
   public:
      LED(int number);
      virtual void turnOn();
      virtual void turnOff();
      virtual void flash(string delayms);
      virtual void outputState();
      virtual void blink(int num);
      virtual string status();
      virtual ~LED();
};

#endif
