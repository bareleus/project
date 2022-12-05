/** Simple On-board LED flashing program - written by Derek Molloy
*    simple OOP  struture for the Exploring BeagleBone book
*
*    This program uses all four LEDS and can be executed in three ways:
*         myBlink on 1 2
*         myBlink off 2 1
*         myBlink flash 2 3  (flash at time delay intervals)
*         myBlink status 0 2 (get the trigger status)
*
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<unistd.h>
#include "LED_display.h"
using namespace std;

#define LED_PATH "/sys/class/leds/beaglebone:green:usr"

LED::LED(int number){
   this->number = number;
   // much easier with C++11 using to_string(number)
   ostringstream s;    // using a stream to contruct the path
   s << LED_PATH << number;   //append LED number to LED_PATH
   path = string(s.str());    //convert back from stream to string
}

void LED::writeLED(string filename, string value){
   ofstream fs;
   fs.open((path + filename).c_str());
   fs << value;
   fs.close();
}

void LED::removeTrigger(){
   writeLED("/trigger", "none");
}

void LED::turnOn(){
   //cout << "Turning LED" << number << " on." << endl;
   removeTrigger();
   writeLED("/brightness", "1");
}

void LED::turnOff(){
   //cout << "Turning LED" << number << " off." << endl;
   removeTrigger();
   writeLED("/brightness", "0");
}

void LED::flash(string delayms = "50"){
   cout << "Making LED" << number << " flash." << endl;
   writeLED("/trigger", "timer");
   writeLED("/delay_on", delayms);
   writeLED("/delay_off", delayms);
}

void LED::blink(int num){
   cout << "Making LED" << number << " blink." << endl;	
   cout << "Blinking LED " << num << " times." << endl;
   removeTrigger();
   // A count up variable
   int counter = 0;
   // Loop to blink the n times LED
   while(counter!=num){
      writeLED("/brightness","1"); // Turn on the LED
      sleep(1); // Stay on a sec
      writeLED("/brightness","0"); // Turn off the LED
      sleep(1); // Stay off for a sec
      counter++; // Keep track the number of time that the LED has been blinked.   
   }
}

string LED::status(){
   fstream fs;
   fs.open((path + "/brightness").c_str(), fstream::in);
   string line;
   getline(fs, line);
   fs.close();
   return line;
}

void LED::outputState(){
   ifstream fs;
   fs.open( (path + "/trigger").c_str());
   string line;
   while(getline(fs,line)) cout << line << endl;
   fs.close();
}

LED::~LED(){}
