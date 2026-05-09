# Open Systems Interconnection Model

First 3 considered software
Last 3 considered hardware

## 1. Application Layer

- Determines the protocol

## 2. Presentation Layer

- Transaltes to binary
- Compresses data
- Encrypts/decrypts data using SSL

## 3. Session Layer

- determines transmission mode (simplex/half-duplex/full-duplex)
- Authentication/Authorization
- Session Management

## 4. Transport Layer

- Segments/desegments data
- Controls flow (transfer rate)
- Determines transmission protocol (UDP/TCP)

- Port Number
- Sequence number

## 5. Network Layer

- Logical addressing
- Routing from source to destination
- IP address, Packet

### Internet Protocol (IP) Address

- 4 octets, each 8 bit, total 32 bits
- contains network address and host address
- subnet mask: network address = 1s, host address = 0s

#### Classes

1. Class A: first octet 1-126... 255,0,0,0 subnet
2. Class B: first octet 128-191... 255.255.0.0 subnet
3. Class C: first octet 192-223... 255.255.255.0 subnet
4. Class D: first octet 124-239... multicast
5. Class E: first octet 240-255

x.x.x.0 -> network address
x.x.x.255 -> broadcast

#### Public/Private IP

- public is unique
1. Class A -> 10.0.0.0-10.255.255.255... subnet 255.0.0.0... large organisation
2. Class B -> 172.16.0.0-172.31.255.255... subnet 255.255.0.0... midsize
3. Class C -> 192.168.0.0 - 192.168.255.255... subnet 255.255.255.0... home, office, small company

IPv6 128 bit, 8 groups colon separated

### DNS

Domain name system, translates ip to domain name and vice versa

## 6. Data Link Layer

- Physical addressing
- MAC is 12 Digit hexadecimal for the NIC
- MAC address, Frame
- CSMA carrier sense multiple access, checks if active devices are sending data before sending to avoid collision

### NIC

Moves from data link layer to physical layer

wired and wireless

intel, broadcom, tplink add MAC hexa... 6 octets each 8 bits, 48 bit total... first 3 octets vendor, last 3 octet unique

## 7. Physical Layer

- Translates to bits -> signals or light