
# seplosd

A simple program to read the telemetry information out of Seplos BMSses over serial and put them in MQTT for consumption in systems like Home Assistant.

Based on the C library and tools for SEPLOS BMS (Battery Management System) by @BrucePerens. The modified source-code for this library is available here in accordance with the GPL.

## Build Dependencies
* `libuv1-dev`
* `libjson-c-dev`
* `libpaho-mqtt-dev`
* `libconfig-dev`

## Building
```bash
git clone https://github.com/tylerjwatson/seplosd.git
cd seplosd
make
sudo make install
```

## seplosd Configuration

A default configuration is made available at `/etc/seplosd.conf` and you will need to edit it to your needs.

```conf
# The serial device the BMS is connected to
bms_device = "/dev/ttyUSB0";
# The MQTT topic to publish BMS telemetry data into
topic = "seplos/0";
# A URI of the MQTT broker.  
#
# A null-terminated string specifying the server to which the client will connect. It takes the form protocol://host:port. Currently, protocol must be:
# tcp:// or mqtt:// - Insecure TCP
# ssl:// or mqtts:// - Encrypted SSL/TLS
# ws:// - Insecure websockets
# wss:// - Secure web sockets
#
# The TLS enabled prefixes (ssl, mqtts, wss) are only valid if a TLS version of the library is linked with. For host, you can specify
# either an IP address or a host name. For instance, to connect to a server running on the local machines with the default MQTT port, specify tcp://localhost:1883. 
mqtt_uri = "";
# The client ID used to identify this instance to the MQTT server.  If you are running more than one instance of seplosd then you will
# need to change this.
mqtt_client_id = "seplosd";
# BMS refresh interval in milliseconds.
interval = 10000;
```

## Running seplosd
```bash
systemctl enable --now seplosd
```

## MQTT Format
This is the MQTT output from my battery, and can be used as a sample:
```json
{
    "i": -8.3599996566772461, // load current
    "v": 53.009998321533203, // voltage
    "dv": 0.00099992752075195312, // pack delta voltage
    "p": -443.16357421875, // load power
    "soc": 98, // state of charge
    "soh": 100, // state of health
    "cap": 280, // battery capacity in Ah
    "ncycles": 4, // number of cycles
    "cap_residual": 274, // residual capacity in Ah
    "cap_rated": 280, // rated capacity in Ah
    "bal": false, // true if the balancer is enabled
    "hot": true, // ??? may remove
    "cold": true, // ??? may remove
    "shutdown": false, // true if the battery has shutdown
    "standby": false, // true if the battery is in standby (not discharging or charging)
    "charge": false, // true if the battery is charging
    "discharge": true, // true if the battery is discharging
    "charge_sw": true, // true if the charge FET is enabled
    "discharge_sw": true // true if the discharge FET is enabled 
}
```