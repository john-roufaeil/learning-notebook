# Day I

## Computer Network

A collection of computers and other end devices or peripherals connected together through connecting media to:

### Hardware

- Devices: Computers, Printers, Phones, Switches, Routers
- Medium: Wired, Wireless, Satalite

### Software

- Messages: Information that travels over the medium such as Mails-WhatsApp….etc
- Protocols: Governs how messages flow across network such as http –https-FTP-RDP

### Terminology

- NIC (Network Interface Card): a hardware that enable the device to directly access the network
    - Internal NIC (plugs into the motherboard directly)
    - External NIC(Wireless and USB based )
- MAC Address: Physical Address, Unique address over the world burned on the NIC card
- IP Address:logical address, identify each device on an IP network layer.
- Protocols: Communication rules that all entity must agree on http –https-FTP-RDP
- Topology: how devices are connected (shape) and how message flow from one device to another device

## Connectors

- **Hub**: Allow different nodes to communicate with each other at the same network(Slow the network)
- **Switch**: Allow different nodes to communicate with each other at the same network and time without slowing each other
- **Router**: Allow different networks to communicate with each other
- **Access point (AP)**: allows other Wi-Fi devices to connect to a wired network. An AP is a physical location where Wi-Fi access is available.
- **Repeater**: Regenerate the signal over the same network before the signal becomes too weak or corrupted

## Network Classifications

### According to Covered Area

- Personal Area Networks (PAN)
    - a computer network for interconnecting devices centered on an individual person's workspace.
    - a PAN provides data transmission among devices such as computers, smartphones, tablets and personal digital assistants
- Local Area Networks (LAN)
    - a group of computers connected in small geographical area
    - a limited area such as a residence, school, laboratory, university campus or office building (100 -1000 M)
    - allow users to share files and services
    - high speed of communications
    - under your administrive control
- Metropolitan Area Networks (MAN)
    - a MAN connects an area larger than a LAN but smaller than a WAN (Up to 100 km) such as a city.
    - dedicated or high-performance hardware
- Wide Area Networks (WAN)
    - a WAN is a group of computers connected in Large geographical area such as country
    - a WAN often connects two LANs (WAN Link)
    - WAN can contain multiple smaller networks, such as LANs or MANs.
    - Very low Speed
    - Under your ISP Administrative control
    - example of WAN is Internet

### According to Network Topology

Logical topology: how to send message from device to other
Physical topology: how Computers connected to each other physically (wired)
Dependent on: type and number of equipment and cost

- Point to Point
- Ring / Dual Ring
- Mesh
- Star

### According to Network Model

- Peer to peer: all nodes are equal
- Client/server: some nodes are dedicated to present services

||Peer to peer| Client/server|
|---|---|---|
|Centralization|Local machine central server|All client machines connect to central server to get service|
|Storage| Each machine shares its files equally with others| All files and folders are on dedicated storage on the server, and the client access their files based on database on the server|
|Cost|inexpensive|expensive (OS license)|
|Scalability|in home or small office|medium/large enterprise|
|OS|Client OS|Server OS to handle multiple requests|


## Protocols


## OSI Layers

- Application -> user interface takes data from user - choose protocol
- Presentation -> formats data in the extension (appears more in receiving)
- Session -> validates data is ok
- Transport -> error recovery
- Network
- Data Link
- Physical