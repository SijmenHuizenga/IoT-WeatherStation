### Running the mysql database with docker
```
docker run --name gatewaydb -d \
    -e MYSQL_DATABASE=gatewaydb \
    -e MYSQL_USER=gatewayuser \
    -e MYSQL_PASSWORD=somegatewayuserdbpass \
    -e MYSQL_RANDOM_ROOT_PASSWORD=yes \
    -p 1234:3306 \
    -v /dockerdata/gatewaydb:/var/lib/mysql \
    mysql
```