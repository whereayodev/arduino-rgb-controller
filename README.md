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

1. Connect RGB strip to Arduino:
   - Red → Pin 3
   - Green → Pin 6
   - Blue → Pin 5
   - Power/Ground according to your strip type

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
