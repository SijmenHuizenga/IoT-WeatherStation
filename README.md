# IoT-WeatherStation
A arduino weather station with gateway. University of Applied Science Arnhem en Nijmegen (HAN): Internet of Things (IoT). This branch contains the code for child arduino's. These childs measure environmental data and sent it to the gateway.

## Pre Requirements
To compile and upload this code the following libraries are required:
 * Ethernet2 
 * SparkFun_Si7021_Breakout_Library



## Program design

### Wether_child.ino
This is the main program, this is where al the timers run and the order of functions is managed.

### Debug.h / cpp

In this file all the debugging is managed. Debugging writes the output to serial.

To enable or disable debugging for a feature uncomment or comment the correstonding tag.

Example:

`//#define DEBUGNETWORK` <-- network debugging disabled.

`#define DEBUGSENSOR` <-- sensor debugging enabled.

#### How to use debugging
There are three debugging functions:

`startDebugging();` <-- starts serial if debugging is enabled

`debug("debug message", DEBUG_TYPE);` <-- print the debug data without a new line, doesn add debugging type to output.

`debugln("debug message", DEBUG_TYPE);` <-- print the debug data with a new line, adds [DEBUG_TYPE] infront of the debug line.

Example:
```
startDebugging();
int i = 3;

debugln(i, WEBCLIENT);
```
Output: `[NET] 3`

### HttpClient.h / cpp
Sends measured data, registers the child and logs in the child.

`updateHttpClient()` <-- manages which data to send to the gateway or wheater to listen for an answer.

### Led.h / cpp
Manages the 3 leds for displaying the temperature in comparisont to the threshholds.

How to use:

Define the 3 leds using the following code:
```
#define REDLED <red led pin>
#define YELLOWLED <yellow led pin>
#define GREENLED <green led pin>
```

### Network.h / cpp
enables the arduino to send data using the ethernet shield.

To enable using the programmed MAC adress of the ethernet shield, you need to add the following line to the top of the file `Ethernet2.h` in the `libraries/Ethernet2/src` directory on your computer:
`#define WIZ550io_WITH_MACADDRESS`

how to use:

connectEthernet() <-- connects to the ethernet using DHCP so no IP or MAC configuration is needed.
 
### Sensors.h / cpp
Used to setup and read values from the sensors.

how to use:

`setupSensors()` <-- setup and start all sensors
`getHumidity()` <-- returns humidity in % as a float
`getTemperature()` <-- returns temperature in C as a float
`getBrightness()` <-- returns brightness as a float

### Timer.h / cpp
this function can be used to call funtions on a set interval.

How to use:

Example:
```
TimedAction* sendWeatherDataTimer = new TimedAction(); <-- Create a timer per object.
sendWeatherDataTimer->setDelay(5000); <-- set the interval in milliseconds
sendWeatherDataTimer->setCallback(sendWeatherToGateway); <-- set a funtion to call at the interval.
```

# Installation
How to go from an Arduino, an ethernetshield, and a couple of sensors to a weatherboys weatherstation.

## Prerequisites
What code and libraries do you need to make an awesome weatherboys weatherstation.

### Code
**ClearArduino**
This program makes sure there is no unwanted values in te EEPROM.
**Weather_Child**
This is the awesome weatherstation code, it does *Everything*.

### Libraries
**SparkFun Si7021 Breakout Library** [***Avalable here***](https://github.com/sparkfun/Si7021_Breakout)
This is the library that reads the Si7021 values.
**Ethernet2** [***Avalable here***](https://github.com/adafruit/Ethernet2)
This library is needed for the ethernet shield 2. This code has only been tested on the ethernet shield 2.
