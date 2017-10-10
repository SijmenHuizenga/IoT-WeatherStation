# IoT-WeatherStation
A arduino weather station with gateway. University of Applied Science Arnhem en Nijmegen (HAN): Internet of Things (IoT)

[CLICK HERE TO GO TO THE CHILD](https://github.com/SijmenHuizenga/IoT-WeatherStation/tree/child)

[CLICK HERE TO GO TO THE GATEWAY](https://github.com/SijmenHuizenga/IoT-WeatherStation/tree/gateway)

# System Vision
This weather system measures environment weather data from multiple cihld weather stations into a single location named the Gateway. This gateway stores all measured data in a database and makes the data available through a http rest api and a web dashboard. The data is also forwarded to another centralized weather measuring system called the JorgApi.

The childs are built using arduino's with networking shields and many sensors. The exact data that these childs are able to record are specified in the chapter Measurements.

An overview of the system is shown in the below image:
![Overview](imgs/Architectural-overview-diagram.png)

## System flow

A installation of the weather system always exist of 1 gateway. 

After plugging in the new weather child, the child will initialize all the sensors. After initializing all the sensors, the arduino will connect to the network using the DHCP Server.
When the arduino is connected to the network it wil send its IP to the Gateway. The gateway will respond with the timestamp and the childs new ID. The ID will be saved in the EEPROM so it wil be available after a restart.
As soon as the child has received its ID it wil start sending data using that ID to the gateway. The gateway stores all the measurements from the child in a database.

If the power goes off from the child, and the child is powered on again the child will not try to register again. The child will initialize its sensors en connect to the internet just like a new child would.
After the child is connected to the network it wil send its ID and IP address to the gateway so the Gateway knows where it can connect to the child
After this de child wil start sending its data the same way a new child would.

Children all have a green, orange and red led. Always one of these leds will be turned on depending on the temperature. When the temperature is higher than some threshold it will be orange, and the red led will light up if the temperature is higher than another threshold. These two thresholds can be configured using rest endpoints on the children. 

The gateway stores information about the ip's of its children. Using this information the gateway can always reach the children. This is useful for configuring the thresholds on the children. The gateway has endpoints for this configurating the children so that users can talk to the gateway without having to worry about the exact location of the children. 


## Child <-> Gateway communication
The child and gateway communicate with each other to exchange information. Both provide a rest api that is documented using Swagger. The gateway api specs of the child can be found [here](https://github.com/SijmenHuizenga/IoT-WeatherStation/blob/child/api.yaml). To view the rendered version of this specifications use the [swagger editor](editor.swagger.io). The api specs of the gateway can be viewed on the url `/swagger` and `/swagger-ui.html` on a running gateway. All request between gateway and child are explained below.

### Measurements
The childs have many sensors that measure all kind of data. Here are all the data types specified.

#### Humidity
Humidity is measured using a [Si7021](https://www.silabs.com/documents/public/data-sheets/Si7021-A20.pdf) sensor. The data transmitted from the children to the gateway is in percentages with float percision between 0 and 80. 

#### Temperature
Temperature is measured using the same sensor as Humidity in [degrees celsius](https://en.wikipedia.org/wiki/Celsius) with float precision between 10 and 85.

#### Brightness
Light is measured using a light diode and is transmitted to the gateway in floats between 0 and 100 where 0 is no light and 100 is the maximum amount of light that the diode can measure.

#### Pressure
Atmospheric Pressure is implemented in the whole code base but a sensor is not available. Therefor this data will always be empty.