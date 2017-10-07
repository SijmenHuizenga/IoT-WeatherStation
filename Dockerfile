FROM java:8-jre
MAINTAINER Sijmen Huizenga en Bram Otten

ENTRYPOINT ["/usr/bin/java", "-jar", "gateway.jar", "--spring.config.location=gateway.properties"]
EXPOSE 80

ADD target/gateway.jar gateway.jar