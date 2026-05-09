# IoT Value Chain — Complete Lecture Notes

---

## 1. What is IoT?

**IoT (Internet of Things)** is a network of physical objects embedded with sensors and software that connect and exchange data over the internet.

### Evolution of Terminology
| Term | Meaning |
|------|---------|
| M2M | Machine-to-Machine (telecom origin) |
| IoT | Internet of Things (most common today) |
| IIoT | Industrial Internet of Things |
| IoE | Internet of Everything (Cisco marketing term) |
| AIoT | AI + IoT (coined 2021, faded quickly) |
| IoMT / IoFT | Internet of Military/Flying Things |

> Today, **IoT** and **IIoT** are the two dominant terms.

### Internet Evolution
Each new connected device category spawns new applications:
- **PCs** → Gmail, Facebook, YouTube
- **Smartphones** → WhatsApp, Viber, Android Market
- **Wearables** → Activity trackers, health monitors
- **Cars/Appliances/Land** → Predictive maintenance, smart metering, smart agriculture

---

## 2. IoT 5 Main Pillars (Use Case Categories)

| Pillar | Key Characteristics |
|--------|-------------------|
| **Home & Consumer** | High perceived HW value, low SW value perception, many devices, low revenue per app |
| **Cities & Industry** | High SW value, high ROI, high revenues, fewer but expensive apps |
| **Health & Body** | 12–20 years R&D, ultra-low power, high investment, high ROI |
| **Transport & Mobility** | Huge data traffic, location-based, requires good network coverage |
| **Buildings & Infrastructure** | High risk, long battery life needed, relies on past-proven tech (technology lag) |

**Formula:** 25B devices ÷ 1000 = 25M applications × 5 SW jobs/app = **125M SW jobs**

### Top IoT Segments by Project Count (2018, 1600 projects)
1. Smart City — 23%
2. Connected Industry — 17%
3. Connected Building — 12%
4. Connected Car — 11%
5. Smart Energy — 10%

### Top by Planned Budget (2025, UNCTAD)
1. **Factories** ($1.2T–$3.7T)
2. **Cities** ($930B–$1.66T)
> Same top 2 as 2018, just swapped positions.

---

## 3. IoT Value Chain Building Blocks

Analogy: compare mobile value chain to IoT value chain

| Mobile | IoT Equivalent |
|--------|---------------|
| Mobile phone + App | Sensor Device + App |
| Wi-Fi | Wireless Sensor Network |
| Home Wi-Fi Router | IoT Gateway |
| ISP (ADSL/4G) | Carrier Network |
| App Cloud Server | Application Enablement Platform (AEP) |
| Google Play Store | Device & Gateway Remote Management |

### The 8 Components of the IoT Value Chain
1. Sensors / Devices
2. Wireless Sensor Networks
3. Gateways
4. Carrier Networks
5. Application Enablement Platforms (AEP)
6. Analytics Platforms
7. Device & Gateway Remote Management
8. End-to-End System Integrators

---

## 4. Component Deep Dives

### 4.1 Sensors / Devices

- Small computers that sense and transmit data
- **Prototyping boards** (POC/testing): Raspberry Pi, Arduino, Intel Edison, MasterOfThings IoT Kit
- **Real devices** include: low-power CPU, wireless modem (BLE/LoRa/SigFox/NB-IoT), memory, RTOS, firmware
- Built by **Embedded System Engineers**
- Companies: Libelium, Eurotech, Telit, Yanzi; firmware specialists: Witekio

#### Real-Time Operating Systems (RTOS)
| RTOS | Best For | MQTT Support |
|------|----------|-------------|
| **Zephyr** | Security, multi-core, scalable | Native |
| **Riot-OS** | Low-power IoT, modular | Lightweight built-in |
| **Contiki-OS** | IPv6, resource-constrained | Via Contiki MQTT-SN |
| **FreeRTOS** | Real-time, microcontrollers | Via 3rd-party libraries |
| **TinyOS** | Wireless sensor networks | No native MQTT |
| **OpenWrt** | Gateways/routers | Via packages |

---

### 4.2 Wireless Sensor Networks (WSN)

Low-power short/medium-range communication (LPWAN):

| Technology | Range | Power | Data | Notes |
|-----------|-------|-------|------|-------|
| NFC | ~cm | Passive (reader-powered) | Static | Replaces QR codes |
| RFID | ~10–15m | Passive/Active | Static | Product tagging |
| BLE (5.0) | ~300m | Very low | Low | Personal area |
| ZigBee | ~100m | Low | Low | Mesh networks |
| Z-Wave | ~100m | Low | Low | Smart home |
| LoRa / LoRaWAN | ~1–15km | Very low | Low | Smart city, agriculture |
| SigFox | ~10–50km | Ultra-low | Very low | Global IoT carrier |
| NB-IoT | ~10km | Low | Medium | Carrier-operated |

**Key trade-off:** Higher data rate OR longer distance → more power consumed. You can never have all three (high rate + long range + low power) simultaneously.

#### Frequency Bands
| Region | Unlicensed IoT Band |
|--------|-------------------|
| Europe & Egypt | 868 MHz (ISM band) |
| North & South America | 902–928 MHz |
| Africa | 863–870 MHz |
| Asia | 920–925 MHz |
| Australia | 915–928 MHz |
| China | 779–787 MHz |
| India | 865–867 MHz |

> IoT devices certified for EU will work in Egypt. Devices from other regions will NOT be accepted in Egypt (frequency interference risk).

#### Radio Spectrum Zones
- **Permeable Zone** (low freq): penetrates concrete — used by SigFox, LoRa (EU 868 MHz)
- **Semi-Permeable Zone** (mid freq): difficult through dense objects
- **Line-of-Sight Zone** (high freq): needs clear path, long distances

---

### 4.3 IoT Gateways

- Bridges WSN ↔ Carrier Network
- The more wireless technologies supported = better gateway
- Houses powerful CPUs and memory (determines device capacity)
- Intel provides CPU + modem chips → Cisco/Dell build gateways using "Intel Inside"
- Companies: Intel, Cisco, Dell, FaltCom (acquired by Telia), Alleantia
- **Edge/Fog Computing**: part of the application can run on the gateway itself

#### LoRa Network Servers (manage multiple gateways)
- TTN (The Things Network) — community-driven, open source
- ChirpStack (formerly LoRa Server) — Go-based, scalable
- Loriot — supports public/private/hybrid
- MyThings — for MIoTy networks

---

### 4.4 Carrier Networks

| Network Type | Bandwidth | Power | Notes |
|-------------|-----------|-------|-------|
| 3G/4G/5G | High | High | Traditional mobile, roaming issues |
| Fixed (ADSL/Fiber) | High | High | WE (Telecom Egypt) |
| SigFox UnaBiz | Very low | Ultra-low | 70+ countries, 7-year battery life |
| NB-IoT (4G/5G mMTC) | Low | Low | Ericsson-proposed standard |
| LEO Satellites | Low–medium | Low | Lacuna, Swarm, Starlink — for remote areas |

#### SIM Evolution for IoT
| Form | Year | Size | Type |
|------|------|------|------|
| 2FF (Mini SIM) | 1996 | 25×15mm | Physical |
| 3FF (Micro SIM) | 2003 | 15×12mm | Physical |
| 4FF (Nano SIM) | 2012 | 12.3×8.8mm | Physical |
| MFF2 (eSIM/eUICC) | 2016 | <2mm² | Soldered, remote provisioning |
| iSIM | — | Fraction of mm² | Integrated in SoC (CPU+Radio+SIM) |

- **eSIM**: change operator profile remotely without physical swap
- **iSIM**: eSIM integrated directly into the main chip — cheapest option

#### Connectivity Management Platforms (CMP)
Used to activate/deactivate SIMs, monitor data per device, manage roaming via eSIM:
- Jasper Wireless → acquired by Cisco → **Cisco Control Center**
- Ericsson DCP → Ericsson Accelerator → sold to **Aeris**
- Nokia WING
- Vodafone IoT (2024: joint venture with Microsoft)
- Verizon ThingSpace
- Global SIM providers: KORE, Arkessa (Wireless Logic), Emnify, Onomondo, eseye

---

### 4.5 Application Enablement Platforms (AEP)

The AEP is the most complex component — "where the IoT magic happens." Analogy: **AEP is to a smart city what an OS is to a computer.**

#### Why AEP is Needed — 5 IoT Development Challenges
| # | Challenge |
|---|-----------|
| 1 | **Domain knowledge** — understand the problem before selecting sensors |
| 2 | **Connectivity & application protocols** — MQTT, CoAP, Modbus, HTTP |
| 3 | **Data storage** — elastic, non-disruptive scaling |
| 4 | **Real-time action execution at scale** — most complex technical challenge |
| 5 | **Multi-device end-user presentation** — phones, tablets, watches, LEDs |

> AEP solves challenges **2, 3, 4, 5** + technical/project mandates. Domain knowledge (#1) is always **out of scope** of AEP.

#### Technical Mandates
- **Real-time**: no delay (e.g., heartbeat pacemaker)
- **Availability**: always-on servers
- **Reliability**: consistent results for same inputs
- **Scalability**: auto-scale with device count growth

#### Project Management Mandates
- Tight delivery timelines
- Rare competent IoT resources
- Budget overruns common (scope creep from many stakeholders)

#### 4 Core Modules of a True IoT AEP
| Module | Purpose |
|--------|---------|
| **Sensor/Device Profile Definition** | Define device types, data types, protocols, drivers |
| **Monitoring, Enrichment & Orchestration** | Real-time monitoring, rule engine, trigger actions |
| **User Definition & Administration** | Manage developers + end-user access rights |
| **Visual IDE** | Drag-and-drop app development — most important for developers |

#### AEP Platform Families
| Family | Examples | Focus |
|--------|---------|-------|
| Productivity-focused | **MasterOfThings**, PTC ThingWorx | Speed + quality of IoT dev |
| Open source | ThingsBoard, FiWare | Free libraries, community-driven |
| SCADA-origin / IIoT | GE Predix (failed), Schneider/WonderWare, Siemens/MindSphere | Industrial automation |
| Database-origin | SAP Leonardo (PlatOne) | Big data storage + existing tools |
| Messaging bus | Carriots (Altair) | Device-to-device messaging |

#### Optional AEP Applications (MasterOfThings out-of-box)
- **Alarm Management** — map-based, handles missing/invalid/out-of-range data
- **Task Management** — assign field tasks to teams/individuals
- **Asset Management** — track installation, movement, decommission of devices
- **Global Calendar** — schedule management for workforce

---

### 4.6 Analytics Platforms

Used after years of data accumulation to find patterns and correlations:
- **Open source**: Hadoop
- **Commercial**: Splunk, Grok, Watson IoT, OneMind, OverOTT

---

### 4.7 Device & Gateway Remote Management

Like a "Play Store" for IoT devices — remote firmware updates (Firmware Over the Air - FOTA) and configuration:
- **Open source**: Kaa, Balena
- **Commercial**: Bosch IoT Suite, Nokia IMPACT

> Poor firmware development = security/privacy risks (open doors for unauthorized access)

---

### 4.8 End-to-End System Integrators

Companies with skills across the full value chain. Key roles:
- **Solution Architect** (most critical — handles domain knowledge)
- Application Developers
- System Integrators
- Project Managers

---

## 5. Smart City Layered Architecture

```
┌──────────────────────────────────────────────┐
│         City IoT Applications Layer          │
│  Street Lighting | Parking | Waste | etc.    │
├──────────────────────────────────────────────┤
│         City IoT Platform (AEP)              │
│         MasterOfThings — Shared Services     │
├──────────────────────────────────────────────┤
│         Devices & Interfaces Layer           │
│  Connect each device ONCE, use in MANY apps  │
└──────────────────────────────────────────────┘
```

---

## 6. IoT Forecasts vs. Actuals

| Metric | 2013 Estimate (for 2020) | Actual (2020/2025) |
|--------|--------------------------|-------------------|
| Connected devices | 50B (Ericsson & Cisco) | ~28B by 2025 |
| Business value | $1T (Harbor Research) | $1.25T by end of 2018 |

**Revenue breakdown (actual):**
- >50% → Software & managed services
- >30% → Connectivity & network services
- ~5% → Enablement HW (modems)
- ~10% → Smart Cities use cases

---

## 7. IoT Impact on Business & People

IoT reverses traditional processes:
- **Before**: Customer calls support (IVR)
- **After**: Service provider proactively contacts customer

Examples:
- Connected car → dealership auto-schedules maintenance
- Connected appliance → authorized technician proactively books repair
- Supply chain → spare parts planned from real field data, not historical estimates

---

## 8. LEO Satellites for IoT

- Orbit: 500–1,000km altitude
- Speed: ~28,000 km/h
- Lifespan: 5–8 years (then burn up in atmosphere)
- Thousands planned/launched since 1998
- Rising demand due to remote area logistics (shipment, agriculture)
- Players: Lacuna, Swarm, FossaSat, Starlink, OneWeb

---

## 9. Key Protocols

| Protocol | Use Case |
|----------|---------|
| **MQTT** | Primary IoT device↔platform protocol (lightweight, bidirectional, ISO standard) |
| **CoAP** | Constrained devices, REST-like |
| **HTTP/HTTPS** | External system integration with AEP |
| **Modbus** | Industrial automation / SCADA |

> A device without MQTT support should raise serious questions about its IoT fitness.

---

## 10. IoT Job Profiles by Value Chain Component

| Value Chain Layer | Job Roles |
|------------------|-----------|
| Sensors/Devices | Embedded Systems Engineer, Hardware Engineer |
| Wireless Networks | RF Engineer, Network Designer |
| Gateways | Network Engineer, Edge Computing Developer |
| Carrier Networks | Telecom Engineer, Connectivity Manager, CMP Admin |
| AEP | IoT Solution Architect, IoT App Developer, IoT Platform Admin |
| Analytics | Data Scientist, Big Data Engineer |
| Remote Management | DevOps / IoT OTA Engineer |
| System Integration | Solution Architect, Project Manager, Full-stack IoT Developer |

## 11. Radio Wave Spectrum & IoT Frequencies

### What is a Hertz?
- 1 kHz = 1,000 cycles/second
- 1 MHz = 1,000,000 cycles/second
- 1 GHz = 1,000,000,000 cycles/second
- **Wavelength** = distance from wave crest to crest; shorter wavelength = higher frequency
- Human hearing range: 
  - Operates in sound (mechanical) waves spectrum - unrelated to electromagnetic spectrum 
  - 20 Hz – 20,000 Hz (20 kHz)
  - Below 20 Hz → infrasound
  - Above 20 kHz → ultrasound
- Human visible light range: ~ 400 THz (red) to 790 THz (violet)... below visible light 
  - Below visible light: infrared, microwaves, radio waves
  - Above visible light: ultraviolet, x-rays, gamma waves

### The Electromagnetic Spectrum (Low → High Frequency)
Radio waves occupy 3 kHz – 300 GHz of the electromagnetic spectrum. Beyond that: Infrared → Visible Light → Ultraviolet → X-rays → Gamma rays.

### Radio Wave Spectrum — Named Bands
| Band | Frequency Range | Common Uses |
|------|----------------|-------------|
| **VHF** (Very High Freq) | 30 MHz – 300 MHz | Broadcast TV (Ch 2–13), FM radio |
| **UHF** (Ultra High Freq) | 300 MHz – 3 GHz | Broadcast TV (UHF 14–83), cell phones, garage door openers, remote-controlled toys |
| **L Band** | 1 GHz – 2 GHz | Cell phones, GPS (global positioning) |
| **S Band** | 2 GHz – 4 GHz | Wi-Fi (2.4 GHz), Bluetooth, wireless medical telemetry, satellite radio, weather radar |
| **C Band** | 4 GHz – 8 GHz | Satellite TV, cable TV satellite transmissions |
| **X Band** | 8 GHz – 12 GHz | Police radar, weather radar |
| **Ku Band** | 12 GHz – 18 GHz | Satellite TV networks |
| **K Band** | 18 GHz – 27 GHz | Highway toll tags |
| **Ka Band** | 27 GHz – 40 GHz | Satellite communications |
| **W Band** | 75 GHz – 110 GHz | Security alarms, advanced radar |

> Most white/unmarked areas on the spectrum chart are **reserved for military, federal government, and industry use.**

### IoT-Relevant Frequency Allocations
| Frequency | Technology / Use |
|-----------|----------------|
| **13.56 MHz** | NFC (Near Field Communication) |
| **433 / 868 MHz** | SigFox, LoRa, LoRaWAN, Mioty, Weightless-P — **EU & Egypt unlicensed ISM band** |
| **902–928 MHz** | LoRa/SigFox equivalent for North & South America |
| **2.4 GHz** | Wi-Fi (802.11), Bluetooth / BLE — unlicensed worldwide (used by 300+ consumer devices) |
| **5.8 GHz** | Wi-Fi (802.11ac/n), security alarms |
| **700 MHz – 2.6 GHz** | 4G LTE, NB-IoT, LTE-M (carrier-licensed, auctioned spectrum) |
| **Sub-6 GHz / mmWave** | 5G mMTC (massive IoT), 5G NTN (satellite IoT) |
| **MulteFire / LAA** | Unlicensed LTE eMTC — uses Listen-Before-Talk (LBT) to share spectrum |

### Radio Penetration Zones (from the spectrum chart)
| Zone | Frequency Range | Behavior | IoT Examples |
|------|----------------|----------|-------------|
| **Permeable Zone** | Low (sub-1 GHz) | Penetrates concrete & dense objects — most valuable for IoT | SigFox, LoRa, LoRaWAN @ EU 868 MHz |
| **Semi-Permeable Zone** | Mid (1–3 GHz) | Difficult to penetrate dense objects | Wi-Fi, Bluetooth, GPS, satellite radio |
| **Line-of-Sight Zone** | High (3+ GHz) | Long distance but blocked by trees/buildings; short unobstructed only | Police radar, highway toll tags, satellite TV |

### Key Spectrum Facts for IoT
- Each country **regulates its own spectrum map** — countries must agree on shared allocations
- **Globally agreed** unlicensed bands: Wi-Fi (2.4 GHz), Bluetooth, GSM/2G/3G/4G, GPS, satellite TV
- **NOT globally agreed**: sub-GHz IoT bands — differ per region (see unlicensed IoT band table in section 4.2)
- Using wrong frequencies in a region risks **catastrophic interference** with medical ICU devices, military systems, or flight control systems
- IoT devices certified for EU/868 MHz **will work in Egypt** — devices from other regions will not

---

## 12. Wireless Technologies — Deep Dive

### 12.1 NFC (Near Field Communication)

| Property | Detail |
|----------|--------|
| **Frequency** | 13.56 MHz |
| **Range** | A few centimeters (typically < 4 cm) |
| **Data Rate** | 106–424 kbps |
| **Power** | Passive — powered by the reader's electromagnetic field; no battery in tag |
| **Data Type** | Static / pre-stored (read-only tags) or read-write smart cards |
| **Standard** | ISO 14443, ISO 18092 |
| **Communication** | Bidirectional (reader ↔ tag) |

**How it works:** The NFC reader emits an RF field that wirelessly charges the passive tag with just enough power to transmit its stored data back.

**Use Cases:** Contactless payments, access control badges, product authentication, smart posters, device pairing

**Limitations:**
- Extremely short range (by design — for security)
- Static data only on passive tags — cannot sense changing conditions
- Not suitable for real-time sensing or telemetry
- Very low throughput

---

### 12.2 RFID (Radio Frequency Identification)

| Property | Detail |
|----------|--------|
| **Frequency** | Low Freq: 125–134 kHz / High Freq: 13.56 MHz / UHF: 860–960 MHz |
| **Range** | Passive UHF: ~10–15m; Active tags: up to 100m |
| **Data Rate** | Low (up to ~640 kbps UHF) |
| **Power** | Passive: no battery (reader-powered); Active: battery-powered |
| **Data Type** | Static identifier / stored data |
| **Standard** | ISO 18000, EPC Gen2 |

**RFID vs NFC:**
| Feature | RFID | NFC |
|---------|------|-----|
| Range | Up to 15m (passive UHF) | < 4 cm |
| Communication | Typically one-way | Bidirectional |
| Multi-tag reading | Yes (hundreds at once) | No |
| Smartphone built-in | No (dedicated reader) | Yes |
| Focus | Supply chain, logistics | Payments, pairing |

**Use Cases:** Warehouse inventory, supply chain, animal tagging, library systems, vehicle access

**Limitations:**
- Static data — cannot sense live environmental changes
- UHF blocked by metal and liquids
- No native internet connectivity (requires reader + backend)
- Passive tags fully dependent on reader proximity for power

---

### 12.3 BLE (Bluetooth Low Energy)

| Property | Detail |
|----------|--------|
| **Frequency** | 2.4 GHz ISM band (2.402–2.480 GHz), 40 channels |
| **Range** | BLE 4.x: ~10–30m; **BLE 5.0: up to 300m** (open space) |
| **Data Rate** | 125 kbps – 2 Mbps (BLE 5.0) |
| **Power** | Ultra-low — coin cell can last months to years |
| **Topology** | Point-to-point, broadcast, mesh (BLE 5.0+) |
| **Latency** | ~3ms (vs Classic Bluetooth ~100ms) |
| **Standard** | IEEE 802.15.1, Bluetooth SIG |

**BLE vs Classic Bluetooth:** BLE sacrifices throughput for dramatic power savings. Not suitable for audio/video streaming.

**Use Cases:** Fitness wearables, smart home sensors, indoor asset tracking, beacon-based location, medical devices, short-range device-to-gateway link

**Limitations:**
- Max ~300m (BLE 5.0 line of sight) — not for wide-area outdoor deployments
- 2.4 GHz band is congested (shared with Wi-Fi, ZigBee, microwaves)
- Cannot penetrate thick walls reliably (semi-permeable zone)
- BLE mesh adds complexity and latency
- Not a carrier-grade network — self-managed only

---

### 12.4 LoRa & LoRaWAN

**LoRa** = physical radio modulation (proprietary Semtech chip)
**LoRaWAN** = open MAC/network protocol layer built on top of LoRa

| Property | Detail |
|----------|--------|
| **Frequency** | EU: 868 MHz / US: 902–928 MHz / Asia: 920–925 MHz (unlicensed ISM) |
| **Range** | Urban: 2–5 km; Rural: **up to 15 km**; Record: 730,360 km (moon reflection, 1W + 25m dish) |
| **Data Rate** | 0.3–50 kbps (adaptive data rate — ADR) |
| **Power** | Very low — years of battery life |
| **Topology** | Star-of-stars: devices → gateways → network server → AEP |
| **Network License** | **Unlicensed** — no carrier subscription required |
| **Standard** | LoRa Alliance |
| **Payload** | Up to ~250 bytes |

**Network Servers:** TTN (community/free), ChirpStack, Loriot, LoRaServer

**Use Cases:** Smart agriculture, smart city (parking, waste bins, street lighting), building monitoring, environmental sensing, water/gas metering, asset tracking

**Limitations:**
- Very low data rate — no video, audio, or large payloads
- EU duty cycle restriction: max **1% transmission time** per hour
- Half-duplex — cannot send and receive at the same time
- No guaranteed QoS — best-effort delivery
- Gateway infrastructure must be self-deployed or community-shared (no global operator)
- Gateway capacity: ~1,000–2,000 devices depending on message frequency
- LoRa chip is proprietary to Semtech (though protocol is open)

---

### 12.5 SigFox (now SigFox UnaBiz)

| Property | Detail |
|----------|--------|
| **Frequency** | EU: 868 MHz; US: 902 MHz (Ultra-Narrowband — UNB) |
| **Range** | Urban: ~10 km; Rural: **up to 50 km** |
| **Data Rate** | **100–600 bps** (extremely low by design) |
| **Power** | Ultra-low — **7+ years on a single battery** |
| **Message limit** | Max **140 uplink / 4 downlink messages per device per day** |
| **Payload size** | Max **12 bytes** uplink, 8 bytes downlink |
| **Network License** | **Operator model** — SigFox owns and runs the network globally |
| **Coverage** | 70+ countries as one unified global network |
| **Modem cost** | **Cheaper than NB-IoT modems** |

**SigFox vs LoRa:**
| Feature | SigFox | LoRa/LoRaWAN |
|---------|--------|-------------|
| Network ownership | SigFox (operator) | Self-deployed / community |
| Modem cost | **Cheaper** | More expensive |
| Global roaming | Built-in (one network) | Requires per-country deployment |
| Message limit | Hard cap (140/day) | Duty-cycle limited (~flexible) |
| Payload size | **12 bytes max** | Up to ~250 bytes |
| Downlink capability | Very limited (4/day) | More flexible |

**Use Cases:** Utility metering, cold chain monitoring, asset tracking, any low-frequency status-only reporting

**Limitations:**
- **Hard daily message cap** (140 up / 4 down) — cannot report frequently
- **Tiny 12-byte payload** — only simple scalar values, no complex data structures
- Very limited downlink — hard to send commands to device
- Operator dependency — service continuity tied to SigFox business decisions
- Not suitable for real-time or high-frequency IoT use cases

---

### 12.6 MIoTy (ETSI Standard)

| Property | Detail |
|----------|--------|
| **Frequency** | Sub-GHz ISM: 868 MHz (EU), 915 MHz (US) |
| **Range** | Up to ~15 km (similar to LoRa) |
| **Data Rate** | ~100 bps – 1 kbps |
| **Power** | Ultra-low (comparable to LoRa/SigFox) |
| **Modulation** | **Telegram Splitting Multiple Access (TSMA)** — unique differentiator |
| **Standard** | **ETSI TS 103 357** (international open standard) |
| **Network Server** | MyThings platform (by Behrtech) |

**Key differentiator — Telegram Splitting (TSMA):**
Instead of sending a message as a single burst (like LoRa), MIoTy splits each message into many small sub-packets spread across different time slots and frequencies. This makes it highly resilient to interference, jamming, and signal collision in dense deployments.

**MIoTy vs LoRa:**
| Feature | MIoTy | LoRa |
|---------|-------|------|
| Interference robustness | **Very high** (TSMA) | Moderate |
| High-density scalability | **Better** (less collision) | Degrades at high density |
| Standard body | ETSI (international) | LoRa Alliance |
| Ecosystem size | Smaller, growing | Large, established |
| Public gateway infrastructure | Limited | Wide (TTN, etc.) |

**Use Cases:** High-density industrial IoT, large-scale smart metering, RF-noisy factory environments, city-scale deployments where many devices share the same band

**Limitations:**
- Much smaller ecosystem than LoRa — fewer off-the-shelf devices and gateways
- Less community infrastructure (no public network equivalent to TTN)
- Relatively newer and less field-proven than LoRa or SigFox
- Lower data rate than LoRa at equivalent conditions

---

### 12.7 Technology Comparison Summary

| Technology | Range | Frequency | Data Rate | Power | Network Model | Max Payload | Best For |
|-----------|-------|-----------|-----------|-------|--------------|-------------|----------|
| **NFC** | ~4 cm | 13.56 MHz | 106–424 kbps | Passive | Point-to-point | Static/small | Payments, pairing |
| **RFID** | cm–15m | 125kHz–960MHz | Low | Passive/Active | Reader-based | Static ID | Asset tracking, inventory |
| **BLE 5.0** | ~300m | 2.4 GHz | 125kbps–2Mbps | Ultra-low | P2P / Mesh | Medium | Wearables, indoor IoT |
| **LoRaWAN** | 2–15km | 868/915 MHz | 0.3–50 kbps | Very low | Self-deployed | ~250 bytes | Smart city, agriculture |
| **SigFox** | 10–50km | 868/902 MHz | 100–600 bps | Ultra-low | Operator-run | **12 bytes** | Low-freq metering, tracking |
| **MIoTy** | ~15km | 868/915 MHz | 100bps–1kbps | Ultra-low | Self-deployed | Small | Dense industrial, metering |

> **Golden rule:** You can never simultaneously achieve **high data rate + long range + low power**. Every technology trades off at least one dimension.
