[![docker badge](https://img.shields.io/badge/docker%20image-available-brightgreen.svg)](https://hub.docker.com/r/sijmenhuizenga/iotweathergateway)
[![traivs badge](https://img.shields.io/travis/SijmenHuizenga/IoT-WeatherStation/gateway.svg)](https://travis-ci.org/SijmenHuizenga/IoT-WeatherStation)
[Gateway Decisions](decisions.md)

# Weather System Gateway
This gateway is the central link in the weahter network. Measurements from children are stored in this gateway and the data is forwarded to the jorgapi. This gateway als has a dashboard to display measurements and control children settings.

## Running the Gateway
Pre-requirement: a running empty mysql database and corresponding username and password. Read [this tutorial](https://dev.mysql.com/doc/mysql-getting-started) or use the following docker-compose.yaml configuration to run on raspberry pi with docker:

```
version: '3'
services:
  gatewaystore:
    image: hypriot/rpi-mysql
    volumes:
      - /home/pi/gateway/store:/var/lib/mysql
    environment:
      - MYSQL_DATABASE=gatewaydb
      - MYSQL_USER=[DB USER]
      - MYSQL_PASSWORD=[DB PASS]
      - MYSQL_RANDOM_ROOT_PASSWORD=yes
      - MYSQL_ROOT_PASSWORD=[RANDOM NUMBER]
    ports:
      - "3306:3306"
```

Whatever method you use to run the gateway, you will need a configuration file to link the java application to the mysql database. An example configuraiton file is as follows:

`gateway.properties`
```properties
spring.datasource.url=jdbc:mysql://[MYSQL DB IP]:[MYSL DB PORT]/[MYSQL DB NAME]?autoReconnect=true&useSSL=false
spring.datasource.username=[DB USER]
spring.datasource.password=[DB PASS]
spring.datasource.driver-class-name=com.mysql.jdbc.Driver
```

To run the gateway there are two options. Either run the java application directly from the runnable jar or use docker to run the gateway as a containerized application.

### Running the gateway as Jar
Download the latest runnable jar from [here](https://github.com/SijmenHuizenga/IoT-WeatherStation/releases).

Run the Jar with the following command:

``java -jar gateway.jar --spring.config.location=[PATH TO PROPERTIES FILE]``

Now the gateway dashboard is available on port 80 and children can start sending data to it. To get the forwarding of data to the jorg api to work you need to visit te dashboard in your browser and configure the url, username and password for the jorgapi.

### Running the gateway with Docker
First, make sure you have docker installed. Installing docker on the raspberry pi 3 is done with the following command:

``curl -sSL https://get.docker.com | sh``

Next, install docker-compose using the following commands:

``
sudo apt-get install python-pip
sudo pip install docker-compose
``

Next, create a file create `docker-compose.yaml` and fill it with the following content:

```yaml
version: '3'
services:
  gateway:
    image: sijmenhuizenga/iotweathergateway:latest   # use 'sijmenhuizenga/iotweathergateway:rpi' on raspberry pi
    volumes:
      - [PATH TO PROPERTIES FILE]:/gateway.properties
    depends_on:
      - gatewaystore
    ports:
      - "80:80"
```

Last step is to run `docker-compose up` to start the gateway application and database.

Now the gateway dashboard is available on port 80 and children can start sending data to it. To get the forwarding of data to the jorg api to work you need to visit te dashboard in your browser and configure the url, username and password for the jorgapi.


## Developing
This application is a developed in Java using the [Spring Boot](https://projects.spring.io/spring-boot/) platform. The project is built and packaged using [Maven](https://maven.apache.org/). 

The dashboard webapp uses React together with some other external javascript and css resources. The decision is made to keep this webapp simple. The webapp source code does not need to be compiled to work. It uses a standalone version of Babel and a development version of React. The decision for this can be read [here](decisions.md).

### Package from Source
1. Clone this repo
2. Run `mvn package` to build the runnable .jar
3. Run `docker build -t sijmenhuizenga/iotweathergateway .` to build the docker image for normal computers
3a. Run `docker build -t sijmenhuizenga/iotweathergateway:rpi -f Dockerfile-rpi .`
