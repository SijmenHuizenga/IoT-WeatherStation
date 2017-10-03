# IoT-WeatherStation
A arduino weather station with gateway. University of Applied Science Arnhem en Nijmegen (HAN): Internet of Things (IoT)

[CLICK HERE TO GO TO THE CHILD](https://github.com/SijmenHuizenga/IoT-WeatherStation/tree/child)

[CLICK HERE TO GO TO THE GATEWAY](https://github.com/SijmenHuizenga/IoT-WeatherStation/tree/gateway)

![Overview](https://github.com/SijmenHuizenga/IoT-WeatherStation/raw/master/Architectural-overview-diagram.png)

## Nieuw weerstation inpluggen
After plugging in the new weather child, the child will initialize all the sensors. After initializing all the sensors, the arduino will connect to the network using the DHCP Server on the Raspberry Pi

After the arduino is connected to the network it wil send its IP to the Gateway on the Rapsberry Pi, this happens using a Restfull http POST request containing JSON.  The gateway will respond with the timestamp and the childs new ID, also in Restfull html response code 200 containing JSON. The ID will be saved in the EEPROM so it wil be saved after a restart.

After the child has received its ID it wil start sending data using that ID using JSON over restfull html.

## Bestaand weerstation inpluggen
After plugging in an already registerd child, the child will initialize its sensors en connect to the internet just like a new child would.

After the child is connected to the network it wil send its ID and IP address to the gateway so the Gateway nos the new IP to coorespond with the ID.

After this de child wil start sending its data the same way a new child would.

## The Pi
The Pi constantly listens on a DHCP server and on the Gateway. This way it can give new IPś to the children and collect the data from the children.

When te Gateway receives data it saves it to the database and then send it to the [Jorg API](http://iot.jorgvisch.nl/Help).

