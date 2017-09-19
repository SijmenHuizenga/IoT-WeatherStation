# System Vision
This weather system measures environment weather data from multiple cihld weather stations into a single location named the Gateway. This gateway stores all measured data in a database and makes the data available through a http rest api. The data is also sent forward to another centralized weather measuring system.

The childs are built using arduino's with networking shields and many sensors. The exact data that these childs are able to record are specified in the chapter Measurements.

### Measurements
The childs have many sensors that measure all kind of data. Here are all the data types specified.

#### Humidity
Humidity is measured using a [Si7021](https://www.silabs.com/documents/public/data-sheets/Si7021-A20.pdf) sensor. The data transmitted from the children to the gateway is in percentages with float percision between 0 and 80. 

![Si7021](http://ecx.images-amazon.com/images/I/41WHSopsYnL._SL256_.jpg)

#### Temperature
Temperature is measured using the same sensor as Humidity in [degrees celsius](https://en.wikipedia.org/wiki/Celsius) with float precision between 10 and 85.

#### Brightness
Light is measured using a light diode and is transmitted to the gateway in floats between 0 and 100 where 0 is no light and 100 is the maximum amount of light that the diode can measure.

![Light Diode](http://www.alselectro.com/images/LDR_thumb_w800f4or.jpg)

#### Pressure
Atmospheric Pressure will be measured with a device that is yet unknown.


