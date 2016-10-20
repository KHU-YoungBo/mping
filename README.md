# mping

MAC Ping. One-way packet transmit test tool.

* Without IP protocol.
* One-way, No echo
* Ethernet Only


If you want to see frames(packets) at the receiver side, use **tcpdump** with -e option.

## Usage
```
  ./mping [IF NAME] [MAC ADDR]
```

So, Like this:

```
  ./mping eth0 01:23:45:67:89:0A
```
