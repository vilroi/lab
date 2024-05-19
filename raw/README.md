# Raw Sockets

Fooling around with raw sockets ([raw(7)](https://www.man7.org/linux/man-pages/man7/raw.7.html)), especially [packet(7)](https://www.man7.org/linux/man-pages/man7/packet.7.html).

- icmp: First attempt to send and receive icmp messages. hacky, incomplete.

- icmp_echo_server: The rewrite of the above. Cleaner, and actually works.

- go_raw: Raw sockets with go. Listens for, ARP messages, parses it, and dumps it to stdout.
