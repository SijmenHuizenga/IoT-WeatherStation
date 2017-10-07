[![docker badge](https://img.shields.io/badge/docker%20image-available-brightgreen.svg)](https://hub.docker.com/r/sijmenhuizenga/iotweathergateway)
[![traivs badge](https://img.shields.io/travis/SijmenHuizenga/IoT-WeatherStation/gateway.svg)](https://travis-ci.org/SijmenHuizenga/IoT-WeatherStation)

# Weather System Gateway
This gateway is the central link in the weahter network. Measurements from children are stored in this gateawy and the data is forwarded to the jorgapi. This gateway als has a dashboard to display measurements and control children settings.

### Running the gateway with Docker
First, make sure you have docker installed. Than create a `gateway.properties` file to store the application properties. This file could look like this: 

```properties
spring.datasource.url=jdbc:mysql://gatewaystore/gatewaydb?autoReconnect=true&&useSSL=false
spring.datasource.username=gatewayuser
spring.datasource.password=gatewaystorepass
```
Next, create a file create `docker-compose.yaml` and fill it with the following content:

```yaml
version: '3'
services:
  gatewaystore:
    image: mysql
    volumes:
      - [replace with path to persistant db store location]:/var/lib/mysql
    environment:
      - MYSQL_DATABASE=gatewaydb
      - MYSQL_USER=gatewayuser
      - MYSQL_PASSWORD=gatewaystorepass
      - MYSQL_RANDOM_ROOT_PASSWORD=yes
  gateway:
    image: sijmenhuizenga/iotweathergateway:latest
    volumes:
      - [replace with path to gateway.properties]:/gateway.properties
    depends_on:
      - gatewaystore
    ports:
      - "80:80"
```
**It is advised to change the `gatewayuser` and `gatewaystorepass` values in the `docker-compose.yaml` and `gateway.properties` file to different values.**

Last step is to run `docker-compose up` to start the gateway application and database.

### Package from Source
1. Clone this repo
2. Run `mvn package` to build the runnable .jar
3. Run `docker build -t sijmenhuizenga/iotweathergateway .` to build the docker image