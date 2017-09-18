# Desisions 

### Getting arduinos on the network
For getting arduinos on the internet there are a couple of options. 
* Giving every arduino a random IP
  - Negatives
    - Possibility of arduinos having the same IP.
    - IP's will be all over the place.
  - Positives
    - Easy to implement
* Using a standard IP when booting and asking the gateway via HTTP for a new one.
  - Negatives
    - only one arduino can initialize at the same time.
    - ip addresses wil be sent over http and the arduino needs to restart its network for the IP to apply
  - Positives
    - no need for a dhcp server
* Using DHCP to supply the arduinos with IPs
  - Negatives
    - There needs to be an DHCP server on the network
  - Positives
    - The Ethernet library and DHCP will make sure every arduino gets an unique IP.
    - DHCP is reliable
    
### Identifying the arduinos
There are a couple of options to identify an arduino on the network
* Using the IP addres from the arduino to identify it.
  - Negatives
    - Ip addresses can change, the data won't be linked to the right station in the database
    - The is a maximum amount of IPv4 addresses
    - Childs can not be named becouse of the changing IP addresses
  - Positives
    - easy to implement
* Using MAC address to identify an arduino
  - Negatives
    - A MAC address can be changed
    - Someone with malicious intentions can inject false data by spoofing his mac address
  - Positives
    - Every arduino already needs to ha an unique MAC address
* Using a unique ID supplied by the gateway and stored in the EEPROM
  - Negatives
    - The ID may get to a very number after some time of having a lot of diffrent arduinos (and ID wil only be used once becouse of the database
  - Positives
    - ID's can be linked to a name in the gateway for easy management
    - Every arduino will have an unique ID througout its lifetime
    - You can add as many arduino as the network allows
