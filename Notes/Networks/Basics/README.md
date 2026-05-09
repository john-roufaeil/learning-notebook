# Networks Basics

- Network is a connection between end devices 

- Cables:
    - Copper Cables:
        - Coaxial cable:
            - CORE - copper wire
            - Isolation
            - Copper mesh
            - Outer jacket (PVC)
        - Twisted pair cable
            - CAT 5 (100 Mb/s)
            - CAT 5e (1000 Mb/s)
            - CAT 6 (10 Gb/s)
            - RJ45 Ethernet ends
    - Fiber Optics:
        - Light ray
        - Glass core
        - Glass cladding
        - Plastic
        - Kevlar fibers
        - Outer Jacket

- Network interface
    - Translates binary to the physical media used

- Network protocol
    - Set of rules to communicate between 

## Network Topology

### Bus topology

- one sender, all receivers
- disadvantage: data collision
- used in limited computers number
- low fault tolerance
- less security and privacy

### Ring topology

- easy to implement
- no data collision
- slower transmission
- low fault tolerance

### Mesh topology

- no fault tolerance
- low traffic problems
- more secure
- very high cost (many cables, NICs)
- difficult to install
- difficult to manage and troubleshoot

### Star topology

- commonly used
- central device (swtich or hub)
- scalable
- inexpensive
- single point of failrue (solved by redundancy)

## Casting types

### Unicast (one-to-one)

send from one device to another using mac or ip address

### Multicase (one-to-many)

send from one device to a group of devices using a multicast ip address

### Broadcast (one-to-all)

send from one to device to all devices in my network

### Direct broadcast

send from one device in network A to all devices in network B

## Transmission Modes

### Simplex Mode

Unidirectional communication, one device always sends, another receives.

### Half-duplex mode

Bidirectional communication but one at a time.

### Full-duplex mode

Bidirectional and simultaneous between both

## Network Geographical Area Topology

### PAN Personal Area Network

10m-15m bluetooth, NFC, USB cable, etc...

### LAN Local Area Network

Ethernet

### WLAN Wirless Local Area Network

Wireless

### CAN Campus Area Network

Buildings connected

### MAN Metropolitan Area Network

High speed network (usually fibre)

### WAN Wide Area Network

Internet is a WAN

## Host Role

### Peer to peer (work group)

- all devices are equal and each responsible for its own security
- no central administration
- used only in small offices (up to 10 devices) to avoid complexity.

### Client/Server
 
- client requests service (pc/mobile, etc)
- server receives requests and offers service (server)
- scalable
