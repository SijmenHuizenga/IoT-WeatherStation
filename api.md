# REST API Specification
The specification of all the endpoints that this gateway supports.

### POST /humidity
Register a new humidity measurement
#### Body
JSON

``{"stationid": 1, "humidity": 15.5}``

#### Response
| Status code | Body | Description | 
| --- | --- | --- |
| 200 | empty | data is received correctly | 
| 400 | error message in plain text | request is wrong | 

