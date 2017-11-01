# Setting up DHCP on the Raspberry Pi

## Installing the DHCP Service
Make sure you have root privileges, you can do this by logging in as root or using "sudo" before every command. Here we are logging in as root.

`sudo su`

Now lets install the DHCP server

`apt install isc-dhcp-server`

After the installation you will get some errors that look like the following, this is normal.
```
● isc-dhcp-server.service - LSB: DHCP server
   Loaded: loaded (/etc/init.d/isc-dhcp-server; generated; vendor preset: enabled)
   Active: failed (Result: exit-code) since Tue 2017-09-26 08:24:18 UTC; 63ms ago
     Docs: man:systemd-sysv-generator(8)
  Process: 1522 ExecStart=/etc/init.d/isc-dhcp-server start (code=exited, status=1/FAILURE)

sep 26 08:24:16 raspberrypi dhcpd[1534]: 
sep 26 08:24:16 raspberrypi dhcpd[1534]: No subnet declaration for wlan0 (no IPv4 addr…es).
sep 26 08:24:16 raspberrypi dhcpd[1534]: ** Ignoring requests on wlan0.  If this is no…what
sep 26 08:24:16 raspberrypi dhcpd[1534]:    you want, please write a subnet declaration
sep 26 08:24:18 raspberrypi isc-dhcp-server[1522]: Starting ISC DHCPv4 server: dhcpdche…ed!
sep 26 08:24:18 raspberrypi isc-dhcp-server[1522]:  failed!
sep 26 08:24:18 raspberrypi systemd[1]: isc-dhcp-server.service: Control process exit…tus=1
sep 26 08:24:18 raspberrypi systemd[1]: Failed to start LSB: DHCP server.
sep 26 08:24:18 raspberrypi systemd[1]: isc-dhcp-server.service: Unit entered failed state.
sep 26 08:24:18 raspberrypi systemd[1]: isc-dhcp-server.service: Failed with result '…ode'.
Hint: Some lines were ellipsized, use -l to show in full.
```
## Configuring the DHCP server

Now using the correct interface edit the configuration file.

First find the interface name.
```
root@raspberrypi:/home/pi# ip addr
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN group default qlen 1
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
    inet 127.0.0.1/8 scope host lo
       valid_lft forever preferred_lft forever
    inet6 ::1/128 scope host 
       valid_lft forever preferred_lft forever
2: eth0 <-----: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc pfifo_fast state UP group default qlen 1000
    link/ether b8:27:eb:f7:2b:04 brd ff:ff:ff:ff:ff:ff
    inet 192.168.178.2/24 brd 192.168.178.255 scope global eth0
       valid_lft forever preferred_lft forever
    inet6 fe80::ba27:ebff:fef7:2b04/64 scope link 
       valid_lft forever preferred_lft forever
3: wlan0: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc pfifo_fast state DOWN group default qlen 1000
    link/ether b8:27:eb:a2:7e:51 brd ff:ff:ff:ff:ff:ff
```

In this case we are using the eth0 interface. Using this interface edit the following file.

`nano /etc/default/isc-dhcp-server`

You should see the following.

```
# Defaults for isc-dhcp-server (sourced by /etc/init.d/isc-dhcp-server)

# Path to dhcpd's config file (default: /etc/dhcp/dhcpd.conf).
#DHCPDv4_CONF=/etc/dhcp/dhcpd.conf
#DHCPDv6_CONF=/etc/dhcp/dhcpd6.conf

# Path to dhcpd's PID file (default: /var/run/dhcpd.pid).
#DHCPDv4_PID=/var/run/dhcpd.pid
#DHCPDv6_PID=/var/run/dhcpd6.pid

# Additional options to start dhcpd with.
#       Don't use options -cf or -pf here; use DHCPD_CONF/ DHCPD_PID instead
#OPTIONS=""

# On what interfaces should the DHCP server (dhcpd) serve DHCP requests?
#       Separate multiple interfaces with spaces, e.g. "eth0 eth1".
INTERFACESv4="<add your interface here>"
INTERFACESv6=""
```

Now let configure the range that the DHCP can allocate. This is done in the dhcpd.conf file.

`nano /etc/dhcp/dhcpd.conf`

You should edit the file to look like this.

```
# dhcp configuration.

option domain-name "daycare.net";
option domain-name-servers ns1.daycare.net;

default-lease-time 600;
max-lease-time 7200;

# The ddns-updates-style parameter controls whether or not the server will
# attempt to do a DNS update when a lease is confirmed. We default to the 
# behavior of the version 2 packages ('none', since DHCP v2 didn't
# have support for DDNS.)
ddns-update-style none;  

#Becouse this is an internal network and this is the only DHCP server
authoritative;

#This is the basic subnet and the range that is free to be allocated by the DHCP Server
subnet 192.168.178.0 netmask 255.255.255.0 {
  range 192.168.178.100 192.168.178.200;
  option routers 192.168.178.1;
}

```

Change the Ip ranges to the ip addresses used in your network.

Now we can restart the service.

`service isc-dhcp-server restart`

Now check if the service is started correctly.

`service isc-dhcp-server status`

You should see the following.

```
● isc-dhcp-server.service - LSB: DHCP server
   Loaded: loaded (/etc/init.d/isc-dhcp-server; generated; vendor preset: enabled)
   Active: active (running) since Tue 2017-09-26 09:15:26 UTC; 5s ago
     Docs: man:systemd-sysv-generator(8)
  Process: 1628 ExecStart=/etc/init.d/isc-dhcp-server start (code=exited, status=0/SUCCESS)
   CGroup: /system.slice/isc-dhcp-server.service
           └─1641 /usr/sbin/dhcpd -4 -q -cf /etc/dhcp/dhcpd.conf eth0

sep 26 09:15:24 raspberrypi systemd[1]: Starting LSB: DHCP server...
sep 26 09:15:24 raspberrypi isc-dhcp-server[1628]: Launching IPv4 server only.
sep 26 09:15:24 raspberrypi dhcpd[1640]: Wrote 0 leases to leases file.
sep 26 09:15:24 raspberrypi dhcpd[1641]: Server starting service.
sep 26 09:15:26 raspberrypi isc-dhcp-server[1628]: Starting ISC DHCPv4 server: dhcpd.
sep 26 09:15:26 raspberrypi systemd[1]: Started LSB: DHCP server.

```



