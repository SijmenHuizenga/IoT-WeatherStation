# IoT-WeatherStation
A arduino weather station with gateway. University of Applied Science Arnhem en Nijmegen (HAN): Internet of Things (IoT). This branch contains the code for child arduino's. These childs measure environmental data and sent it to the gateway.

## Pre Requirements
To compile and upload this code the following libraries are required:
 * Ethernet2 
 * SparkFun_Si7021_Breakout_Library

To enable using the programmed MAC adress of the ethernet shield, you need to add the following line to the top of the file `Ethernet2.h` in the `libraries/Ethernet2/src` directory on your computer:
`#define WIZ550io_WITH_MACADDRESS`
