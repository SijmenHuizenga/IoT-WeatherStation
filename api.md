# REST API Specification
The specification of all the endpoints that this gateway supports.

### PUT /child/{id}/measurements
Registering measurements from a client can be done by through endpoint. The body of the request contains json with the measurements. If the type of data is not available because the sensor is not connector or the Arduino has decided that the data should not be sent this iteration, than the field should not be specified. A full data example is as follows: 

```json
{
    "humidity": 12.5,
    "brightness": 66.8,
    "temperature": 20.5,
    "pressure": 22.2
}
```

The meaning of the numbers of the fields are further specified in the ![system specification](https://github.com/SijmenHuizenga/IoT-WeatherStation/blob/master/system%20specification.md).

### GET /child/
Get the details of all children

### GET /child/{id}
Get the details of the child with id {id}