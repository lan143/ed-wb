# ed-wb

`ed-wb` is an Arduino/PlatformIO library for the ESP32 that controls Wiren Board Modbus RTU devices, plus a few third-party Modbus devices, over RS-485.

> **Status: alpha / experimental (version 0.1.0).** The API may change, and not every device/firmware combination has been validated. Test on a bench before using it in production.

## Supported devices

Create a device with the matching `add<Class>(address)` factory on `EDWB::WirenBoard`. Each factory returns a pointer to the device and registers it so that `loop()` updates it.

| Class | Device | Purpose | Selected methods |
| --- | --- | --- | --- |
| `M1W2` | WB-M1W2 | 2 × 1-Wire temperature inputs | `getTemperature(channel)` |
| `MAI6` | WB-MAI6 | 6 analog inputs | `getSensorTypeP`, `setSensorTypeP`, `getMeasurementP`, `getMeasurementN`, `getCalculatedValueP` |
| `MAP12E` | WB-MAP12E | Three-phase energy meter | `getVoltage`, `getCurrent`, `getActivePower`, `getForwardActiveEnergy`, `setCTTransformRatio` |
| `MCM8` | WB-MCM8 | 8 discrete inputs with button counters | `getInputState`, `getShortPressCount`, `setDebouncingTime` |
| `MR6C` | WB-MR6C | 6 relays and 6 inputs | `setRelayChannelState`, `getRelayChannelState`, `getInputChannelState`, `getInputCounter` |
| `MS` | WB-MS | Multi-sensor: temperature, humidity, light, air quality, 1-Wire | `getTemperature`, `getHumidity`, `getLightLevel`, `getAirQuality` |
| `MSW` | WB-MSW | Multi-sensor with CO2, sound, and motion | `getCO2Value`, `enableCO2Sensor`, `getSoundPressure`, `getMotion` |
| `LED` | WB-LED | LED controller/dimmer for RGBW and CCT strips | `setChannelBrightness`, `setRGBColor`, `setMode` |
| `ANENJI4200` | ANENJI 4200 W hybrid inverter (third-party) | Inverter telemetry and settings | `getWorkMode`, `getPvPower`, `getBatteryVoltage`, `setOutputPriority` |
| `QDY30A` | QDY30A level sensor (third-party) | Liquid level measurement | `getLevel`, `getUnitOfMeasurement` |
| `MTD262MB` | MTD262MB presence sensor (third-party) | Human presence detection | `isHumanDetected()` |

`MAP12E`, `MR6C`, and `LED` are firmware-version-aware; see [Firmware-version gating](#firmware-version-gating).

## Hardware requirements

- An ESP32 board running the Arduino framework. The library metadata targets `espressif32` / `arduino`.
- An RS-485 half-duplex transceiver connected to an ESP32 UART.
- Wiren Board or compatible Modbus RTU slave devices on the same bus, each with a unique address and matching serial settings.
- If the transceiver has a DE/RE direction pin, pass it as `rtsPin` to `WirenBoard`. The pin is forwarded to `iarduino_Modbus`; auto-direction transceivers do not need it.
- The library does **not** configure the Serial port. Set the pins, baud rate, parity, and stop bits in your sketch before constructing `WirenBoard`.

Use proper RS-485 wiring for your installation (twisted pair, common ground, and bus termination where required).

## Installation

### PlatformIO

Add the library to `lib_deps` using its Git URL:

```ini
[env:esp32dev]
platform = espressif32
framework = arduino
lib_deps =
    https://github.com/lan143/ed-wb.git
```

PlatformIO reads `library.json` and installs the declared dependencies (`log`, `iarduino_Modbus`, and `ed-utils`). `ed-utils` provides `<version/version.h>` (`Version` and `parseVersion`) needed by `src/device/wb_device.h`.

To pin the dependencies explicitly:

```ini
lib_deps =
    https://github.com/lan143/ed-wb.git
    https://github.com/tremaru/iarduino_Modbus.git
    https://github.com/lan143/ed-utils.git
    log
```

### Arduino IDE

1. Install ESP32 board support for the Arduino IDE.
2. Clone or download this repository and place it in your sketchbook `libraries` folder (for example `Arduino/libraries/ed-wb`), or use **Sketch → Include Library → Add .ZIP Library**.
3. Install `iarduino_Modbus`, `log`, and `ed-utils` (https://github.com/lan143/ed-utils), which provides `<version/version.h>`.
4. Restart the Arduino IDE.

There is no `examples/` folder in this repository; the Quick Start below can be copied into a sketch.

## Quick Start

The following sketch configures `Serial1` for 9600 8N1, passes a DE/RE pin to the Modbus client, adds an `MR6C` at address 1, and switches relay channel 1 on.

```cpp
#include <Arduino.h>
#include <wirenboard.h>

using namespace EDWB;

// Serial1 is configured by the sketch. The second argument is the RS-485
// DE/RE pin passed to iarduino_Modbus.
WirenBoard wb(Serial1, 4);

void setup()
{
    Serial.begin(115200);

    // RX = 16, TX = 17: replace these with the pins used on your board.
    Serial1.begin(9600, SERIAL_8N1, 16, 17);
    wb.init(200); // Modbus timeout, milliseconds

    MR6C* mr6c = wb.addMR6C(1); // device address 1
    if (!mr6c->setRelayChannelState(1, true)) { // relay channel 1 (1-based)
        Serial.println("Failed to switch relay");
    }
}

void loop()
{
    wb.loop(); // call continuously
}
```

Make sure the Wiren Board device uses the same serial settings (9600 8N1 in this example). If your transceiver switches direction automatically, construct the client as `WirenBoard wb(Serial1);` instead. Replace the pin numbers with the ones used by your wiring.

## Core API

### `EDWB::WirenBoard`

| Method | Description |
| --- | --- |
| `WirenBoard(Stream& serial)` | Creates the controller on an already-configured `Stream`. |
| `WirenBoard(Stream& serial, uint8_t rtsPin)` | Same, with an RS-485 DE/RE pin forwarded to `ModbusClient`. |
| `void init(uint32_t timeout)` | Starts the Modbus client, selects RTU mode, and sets the timeout in milliseconds. Call after configuring the serial port. |
| `void loop()` | Processes all registered devices and drives asynchronous retries. Call it continuously from `loop()`. |
| `bool changeSpeed(uint32_t newSpeed)` | Broadcasts a bus speed change to Modbus address 0. See the warning below. |
| `ModbusClient* getClient()` | Returns the underlying Modbus client for raw Modbus operations. |

### Factory methods

The factories are `addM1W2`, `addMAI6`, `addMAP12E`, `addMCM8`, `addMR6C`, `addMS`, `addMSW`, `addLED`, `addANENJI4200`, `addQDY30A`, and `addMTD262MB`. Each takes a `uint8_t address` and returns the corresponding device pointer.

### Changing bus speed

`changeSpeed(newSpeed)` writes `newSpeed / 100` to holding register `0x006E` at Modbus address `0` (broadcast). This changes the speed of **all Wiren Board devices on the bus**, not just one device. A wrong value can take the whole bus offline.

The library does not reconfigure the ESP32 UART; your sketch must also change the serial port to the new speed after the write. `WBDevice::setSpeed(uint32_t speed)` validates the standard rates 1200, 2400, 4800, 9600, 19200, 38400, 57600, and 115200 bit/s for firmware-aware devices.

## Device usage notes

### Return conventions

Device APIs use three related styles:

- `std::pair<T, bool>`: `first` is the value and `second` is the Modbus success flag.

  ```cpp
  auto result = ms.getTemperature();
  if (result.second) {
      float temperature = result.first;
  }
  ```

- `Result<T>`: `_value` is the value and `_success` is the success flag. This style is used by `M1W2`, `MSW`, and `LED`.

  ```cpp
  auto result = msw.getCO2Value();
  if (result._success) {
      uint16_t co2 = result._value;
  }
  ```

- `WBDevice` configuration methods (available on `MAP12E`, `MR6C`, and `LED`) return `std::pair<T, WBDeviceError>` from getters and `WBDeviceError` from setters. Compare against `WBDeviceErrorNone` for success.

### Firmware-version gating

`MAP12E`, `MR6C`, and `LED` inherit from `WBDevice`. The base class reads the firmware version from input registers starting at `0x00FA`. If initialization cannot load and parse the version, `loop()` retries every 30 seconds (`FW_VERSION_RETRY_INTERVAL_MS` in `src/device/wb_device.cpp`). Until the version is loaded, version-dependent methods may fail, so keep calling `wb.loop()` after `setup()`.

### Channel indexing

Channel numbering is not uniform across the library. Check the relevant header before calling a method:

- `MR6C`: relay channels are 1–6; input channels are 1–6, with `0` selecting a special register.
- `MCM8`: pins are 1–8.
- `M1W2`: channels are 1–2.
- `MS`: 1-Wire channels are 1–2.
- `LED`: channels are 1–4; pairs are 1–2.
- `MAP12E`: energy-meter channels are 1–4; the `MAP12EPhase` and `MAP12ELine` enums are 0-based.
- `MAI6`: the exception — channel arguments are 0–5.

### Raw Modbus access

`getClient()` exposes the underlying `ModbusClient*` when a device register is not wrapped by this library. Use it with the same serial configuration and bus rules as the higher-level API.

## Dependencies

| Dependency | Status | Purpose |
| --- | --- | --- |
| `log` | Declared in `library.json` | Debug logging used by the library (for example in `wb_device.cpp`). |
| `iarduino_Modbus` | Declared in `library.json` as `https://github.com/tremaru/iarduino_Modbus` | Provides `ModbusClient`, the Modbus RTU master. |
| `ed-utils` | Declared in `library.json` | Provides `<version/version.h>` with `Version` and `parseVersion`, used by `src/device/wb_device.h` and `wb_device.cpp`. |

## Known issues

- **Alpha API.** Names, return types, and device coverage may change between releases.

## License

MIT License. Copyright (c) 2025 Kravchenko Artyom. See [LICENSE](LICENSE).
