# key design decisions of this project


### Java + Spring
The decision to use Java+Spring over other platforms like ASP.NET or Python+Django is based on the following arguments:
* @Bram has no extensive experience with any of the web platforms so every option will pose a learning curve for him.
* @Sijmen has some experience in ASP.NET and Java+Spring, but is more interested in Java+Spring than any other platform.
* Both of us run Linux as the primary operating. This prevents us from using the fully featured IDE Visual Studio for .NET development. This makes .NET development much more difficult and annoying. Java and Python are both extensively supported on Linux with both native runtime's and IDS's.
* This project we want to focus on many cool features on the Arduino and the networking between Arduino and Gateway. We want to prevent as many technical difficulties on the Gateway part. We expect that choosing Python will come with a lot of hassle that we did not sign up for. When using a platform that one of us has some experience in it will be easier to get started and we are able to focus more on the functionalities instead of platform specific details. 

### Timestamps
All timestamps are handled in unix timestamp seconds. This is because dates and times are always annoying to get right when working with multiple platforms. When using unix timestamps there can be no confusion about timezones or date/time formats. 

### Mysql
For the datastore the gateway uses a mysql database. This is because this is one of the popular open source relational database implementations next to Postgre SQL and MariaDB. Because we use Spring with Hibernate ORM, changing the database type later would have no to little impact to the code base since this is supported out of the box. Therefore this decision does not need extensive research and Mysql is just chosen.

### Many tables vs Single table
There are two options for the storage of all measurements.

The first simple way is to make a single table to store all measurements. This table would have the following colums:
 * child id
 * type  (humidity, temperature, brightness)
 * value
 * timestamp
This way it is super simple to add new measurement types. This could even be done without changing code. The downside is that implementing custom business rules for specific measurement types much more difficult is.
 
The second better option is to create a single table and entity for every type of measurement. The downside is that there will be a lot of small tables, but the upside is that it is much easier to have custom things like measurements with multiple values. 