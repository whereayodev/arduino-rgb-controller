# Arduino RGB LED Controller

Simple and efficient Arduino controller for RGB LED strips with smooth transitions and brightness control. Supports both common cathode and common anode RGB strips.

## Features

- Smooth color transitions
- Brightness control with improved low-level response
- Power on/off with fade effect
- Support for both common cathode and common anode RGB strips
- JSON status reporting
- Serial command interface

## Hardware Setup
![Connection Scheme](https://github.com/user-attachments/assets/ee64c2dd-b6e7-471f-96a2-5f080aacaa16)
### 🔧 Components Used
- **Arduino Nano** – Microcontroller
- **IRLZ34N / IRF3205 / 2302 MOSFET** – 3 pcs. (one for each channel: R, G, B)
- **Resistors** – 3 pcs. **1kΩ**
- **RGB LED Strip** – Common anode type
- **Power Supply 5V**

### ⚡ Wiring

| Component         | Arduino Nano | Notes                      |
|------------------|-------------|---------------------------|
| **Red Channel**  | D3          | Connected via MOSFET and 1kΩ resistor |
| **Blue Channel** | D5          | Connected via MOSFET and 1kΩ resistor |
| **Green Channel**| D6          | Connected via MOSFET and 1kΩ resistor |
| **Common Anode (VCC)** | +5V    | Power supply for the LED strip |
| **GND**         | GND         | Common ground |


## Serial Communication

- Baud rate: 115200
- Line ending: newline (\n)

## Commands

| Command           | Example             | Description                               |
| ----------------- | ------------------- | ----------------------------------------- |
| rgb R,G,B         | `rgb 255,0,0`       | Set RGB color (0-255 for each channel)    |
| brightness N      | `brightness 50`     | Set brightness level (0-100%)             |
| power on/off      | `power on`          | Turn strip on/off with fade effect        |
| status            | `status`            | Get current state as JSON                 |
| toggle_strip_type | `toggle_strip_type` | Switch between common cathode/anode modes |

## JSON Response Format

```json
{
  "state": true,
  "color": {
    "r": 255,
    "g": 0,
    "b": 0
  },
  "brightness": 100,
  "applied": {
    "r": 255,
    "g": 0,
    "b": 0
  }
}
```

## Configuration

- Default strip type: Common Cathode
- To change strip type: use `toggle_strip_type` command or modify `COMMON_CATHODE` constant
- Default pins can be changed in define statements at the start of the sketch

## Example Usage

```
rgb 255,0,0    // Set full red
brightness 50  // Set 50% brightness
power off     // Turn off with fade
power on      // Turn on with fade
```
