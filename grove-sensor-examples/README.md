# Grove Hardware Sensor Kit
![photo_grove_starter_kit](../content/soracom-grove-all.png)

## Grove hardware examples

- [Grove Accelerometer](./grove-accelerometer)
- [Grove Button](./grove-button)
- [Grove Buzzer](./grove-buzzer)
- [Grove DHT Sensor](./grove-dht)
- [Grove GPS](./grove-gps)
- [Grove Range Finder](./grove-rangefinder)
- [Grove Magnetic Reed Switch](./grove-magnetic)
- [Power Manager](./power-management)

## Grove Port/Digital PIN mapping on the MKR Connector

- The MKR Connector operates at 5V0
- The MKR GSM 1400 operates at 3V3
- For Grove connectors D0 to D4, the secondary digital I/O is not connected.
- The Grove connector D5/D6 has both digital pins connected to Digital PIN 5 and Digital PIN 6.

Additional information on the MKR Connector Carrier is available online at
[MKR ConnectorCarrier](https://www.arduino.cc/en/Guide/MKRConnectorCarrier)

Grove Connector | Grove PIN | Wire      | Description | IDE |
|----           |-----------|-----------|-------------|-----|
| Blue terminal | VIN       |           | Input 7V0 to 16V0
|               | 5V0       |           | Output 5V0
|               | 3V3       |           | Output 3V3
|               | GND       |           | Ground
|               |           |           |             |     |
| D0            | P1        | YELLOW    | Digital PIN | 0
|               | P2        | WHITE     | not connected
|               | P3        | RED       | 5V0
|               | P4        | BLACK     | Ground
| D1            | P1        | YELLOW    | Digital PIN | 1
|               | P2        | WHITE     | not connected
|               | P3        | RED       | 5V0
|               | P4        | BLACK     | Ground
| D2            | P1        | YELLOW    | Digital PIN | 2
|               | P2        | WHITE     | not connected
|               | P3        | RED       | 5V0
|               | P4        | BLACK     | Ground
| D3            | P1        | YELLOW    | Digital PIN | 3
|               | P2        | WHITE     | not connected
|               | P3        | RED       | 5V0
|               | P4        | BLACK     | Ground
| D4            | P1        | YELLOW    | Digital PIN | 4
|               | P2        | WHITE     | not connected
|               | P3        | RED       | 5V0
|               | P4        | BLACK     | Ground
| D5 / D6       | P1        | YELLOW    | Digital PIN | 5
|               | P2        | WHITE     | Digital PIN | 6
|               | P3        | RED       | 5V0
|               | P4        | BLACK     | Ground
| TWI           | P1        | YELLOW    | Digital PIN | 11 SDA
|               | P2        | WHITE     | Digital PIN | 12 DCL
|               | P3        | RED       | 5V0
|               | P4        | BLACK     | Ground
| Serial        | P1        | YELLOW    | Digital PIN | RX5
|               | P2        | WHITE     | Digital PIN | TX5
|               | P3        | RED       | 5V0
|               | P4        | BLACK     | Ground
