# ICMP Echo Server

An "ICMP Echo Server". 

Listens for ICMP echo requests, and replies to it using raw sockets.

This was written with the intention to demonstrate that raw sockets (AF_PACKET) could be used to listen for packets that were not enabled in the firewall.

In fact, due to the nature of ICMP, if this is run with icmp **NOT** disabled on the firewall, it will result in a duplication:

```console
4 bytes from 192.168.122.141: icmp_seq=1 ttl=64 time=0.311 ms
64 bytes from 192.168.122.141: icmp_seq=1 ttl=64 time=48784 ms (DUP!)
64 bytes from 192.168.122.141: icmp_seq=2 ttl=64 time=0.333 ms
64 bytes from 192.168.122.141: icmp_seq=2 ttl=64 time=48784 ms (DUP!)
64 bytes from 192.168.122.141: icmp_seq=3 ttl=64 time=0.344 ms
64 bytes from 192.168.122.141: icmp_seq=3 ttl=64 time=48784 ms (DUP!)
64 bytes from 192.168.122.141: icmp_seq=4 ttl=64 time=0.370 ms
64 bytes from 192.168.122.141: icmp_seq=4 ttl=64 time=48784 ms (DUP!)
64 bytes from 192.168.122.141: icmp_seq=5 ttl=64 time=0.342 ms
64 bytes from 192.168.122.141: icmp_seq=5 ttl=64 time=48784 ms (DUP!)
```
