// The subcrible.cpp takes no argument
// Example: ./subcrible

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h"
#include "LED_display.h"
#include "GPIO.h"
using namespace std;
using namespace exploringBB;

#define ADDRESS     "tcp://io.adafruit.com:1883"
#define CLIENTID    "Beagle2"
#define TOPIC1       "bareleus/feeds/project.gpio-led"
#define TOPIC2       "bareleus/feeds/project.led-display"
#define AUTHMETHOD  "bareleus"
#define AUTHTOKEN   "aio_dSzD436GaB3AaF2VS1BlElGIyjRV"
#define QOS         1
#define TIMEOUT     10000L

volatile MQTTClient_deliveryToken deliveredtoken;

void delivered(void *context, MQTTClient_deliveryToken dt) {
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}


int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    //int i;
    char* payloadptr;
    printf("Message arrived\n");
    printf("     topic: %s\n", topicName);
    printf("   message: ");
    payloadptr = (char*) message->payload;
    string button = payloadptr;
    
    // Create objects
    // GPIO
    GPIO light(60);
    
    // GPIO display
    if (button == "ON" || button == "OFF"){
    	cout << "The GPIO LED is turning " << payloadptr << endl;
    	button == "ON" ? light.setValue(HIGH) : light.setValue(LOW);	
    }
    // LEDs display
    LED leds[4] = { LED(0), LED(1), LED(2), LED(3)};
    // LEDs display
    if (button == "0" || button == "1" || button == "2" || button == "3"){   
       if(leds[stoi(button)].status() == "1"){
	   cout << "The LED #" << payloadptr << " is turning OFF" << endl;
	   leds[stoi(button)].turnOff();}
       else if(leds[stoi(button)].status() == "0"){
	   cout << "The LED #" << payloadptr << " is turning ON" << endl;
	   leds[stoi(button)].turnOn();} 
       else {leds[stoi(button)].turnOn();}
    }
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}


void connlost(void *context, char *cause) {
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}

int main(int argc, char* argv[]) {
    MQTTClient client;
    MQTTClient_connectOptions opts = MQTTClient_connectOptions_initializer;
    int rc;
    int ch;

    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    opts.keepAliveInterval = 20;
    opts.cleansession = 1;
    opts.username = AUTHMETHOD;
    opts.password = AUTHTOKEN;
    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);
    if ((rc = MQTTClient_connect(client, &opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(-1);
    }
    printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n"
           "Press Q<Enter> to quit\n\n", TOPIC1, CLIENTID, QOS);
    MQTTClient_subscribe(client, TOPIC1, QOS);
    MQTTClient_subscribe(client, TOPIC2, QOS);

    do {
        ch = getchar();
    } while(ch!='Q' && ch != 'q');
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}

