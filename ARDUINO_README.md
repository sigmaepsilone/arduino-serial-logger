# Arduino Load Cell Data Logger - Hardware Documentation

Dual HX711 load cell data acquisition system for Arduino with serial communication.

## 📋 Overview

This Arduino sketch reads data from two HX711 load cell amplifiers, converts raw readings to force values (Newtons), and transmits the data to a PC via serial communication for logging and visualization.

## 🔧 Hardware Requirements

### Required Components
- **Arduino Board** (Uno, Mega, or compatible)
- **2x HX711 Load Cell Amplifiers**
- **2x Load Cells** (any capacity)
- **USB Cable** for Arduino
- **Jumper Wires**

### Current Configuration
- Load Cell 1: Zemic L6E3 (500kg capacity)
- Load Cell 2: Mavin NA2 (200kg capacity)

## 📌 Pin Connections

### Load Cell 1 (Green-White cables)
```
HX711 Pin    →    Arduino Pin
DOUT         →    Digital Pin 2
SCK          →    Digital Pin 3
VCC          →    5V
GND          →    GND
```

### Load Cell 2 (Yellow-Orange cables)
```
HX711 Pin    →    Arduino Pin
DOUT         →    Digital Pin 4
SCK          →    Digital Pin 5
VCC          →    5V
GND          →    GND
```

## 📚 Required Libraries

Install these libraries via Arduino IDE Library Manager:

1. **HX711** by Bogdan Necula
   - Library Manager: Search "HX711"
   - Or: https://github.com/bogde/HX711

2. **Thread** by Ivan Seidel
   - Library Manager: Search "Thread"
   - Or: https://github.com/ivanseidel/ArduinoThread

## 📁 File Structure

```
arduino_serial/
│
├── loadcell_data_logger.ino    # Main sketch
├── Monitor.h                    # Serial communication header
├── Monitor.cpp                  # Serial communication implementation
├── Sistem.h                     # System wrapper header
├── Sistem.cpp                   # System wrapper implementation
└── ARDUINO_README.md           # This file
```

## 🚀 Installation

1. **Install Libraries**
   ```
   Arduino IDE → Tools → Manage Libraries
   Search and install: HX711, Thread
   ```

2. **Upload Sketch**
   - Open `loadcell_data_logger.ino` in Arduino IDE
   - Select your board: Tools → Board → Arduino Uno (or your board)
   - Select port: Tools → Port → COMx
   - Click Upload button

3. **Verify Connection**
   - Open Serial Monitor (115200 baud)
   - You should see 'C' being sent
   - This indicates Arduino is ready for PC connection

## ⚙️ Configuration

### Adjusting Pin Assignments

Edit these lines in `loadcell_data_logger.ino`:

```cpp
const int LOADCELL1_DOUT_PIN = 2;  // Change if needed
const int LOADCELL1_SCK_PIN = 3;
const int LOADCELL2_DOUT_PIN = 4;
const int LOADCELL2_SCK_PIN = 5;
```

### Calibration Parameters

**IMPORTANT**: You must calibrate for your specific load cells!

Current values (for reference):
```cpp
// Load Cell 1 (500kg Zemic L6E3)
const float LOADCELL1_SCALE = 0.4699;
const float LOADCELL1_INTERCEPT = -35007;
const float LOADCELL1_OFFSET = -16.5;  // Newton

// Load Cell 2 (200kg Mavin NA2)
const float LOADCELL2_SCALE = 0.1723;
const float LOADCELL2_INTERCEPT = 378.56;
const float LOADCELL2_OFFSET = 4.8;  // Newton
```

### Sampling Rate Configuration

```cpp
const int SAMPLES_PER_READING = 10;  // Samples to average (reduce noise)
const int THREAD_INTERVAL = 100;     // Data transmission interval (ms)
```

## 🔄 Communication Protocol

### Handshake Sequence

1. **Startup**: Arduino sends `'C'` repeatedly (200ms intervals)
2. **Connection**: Python responds (any character)
3. **Ready**: Arduino waits for commands

### Data Transfer

1. **Python sends**: `'H'` (handshake)
2. **Arduino sends**: 
   ```
   123.456\n
   78.901\n
   ```
   (Two force values, one per line)

3. **Repeat**: Python sends `'H'` for next reading

### Shutdown

1. **Python sends**: `'Q'` (quit)
2. **Arduino**: Returns to connection mode

## 📊 Calibration Guide

### Step 1: Find Zero Offset

```cpp
// In setup(), add for debugging:
void setup() {
    // ... existing code ...
    
    // Comment out force calculations temporarily
    // Use Print instead of Write in sendDataCallback
}
```

1. Run with no load on cells
2. Record force values shown
3. Update offsets in code:
```cpp
const float LOADCELL1_OFFSET = -16.5;  // Your value here
const float LOADCELL2_OFFSET = 4.8;    // Your value here
```

### Step 2: Linear Calibration

1. **Collect Data Points**:
   - Apply known weights (e.g., 0kg, 10kg, 20kg, 50kg)
   - Record raw ADC values for each weight
   - Create table:
   
   | Weight (kg) | Raw Value |
   |-------------|-----------|
   | 0           | 12345     |
   | 10          | 23456     |
   | 20          | 34567     |
   | ...         | ...       |

2. **Calculate Calibration**:
   - Use linear regression: `Weight = SCALE × Raw + INTERCEPT`
   - Many online calculators available
   - Or use Excel LINEST function

3. **Update Constants**:
```cpp
const float LOADCELL1_SCALE = 0.4699;      // Your slope
const float LOADCELL1_INTERCEPT = -35007;  // Your intercept
```

### Step 3: Verification

1. Apply known test loads
2. Compare measured vs actual forces
3. Adjust if needed

## 🐛 Troubleshooting

### No Serial Response
- **Check**: USB cable connection
- **Check**: Correct COM port selected
- **Check**: Baud rate = 115200
- **Try**: Press Arduino reset button

### Incorrect Force Values
- **Check**: Load cells properly connected
- **Check**: HX711 power (5V) stable
- **Check**: Calibration coefficients
- **Try**: Run calibration procedure again

### Noisy Readings
- **Increase**: `SAMPLES_PER_READING` (e.g., to 20)
- **Check**: Electrical noise sources nearby
- **Check**: Ground connections
- **Try**: Add capacitor (100nF) near HX711 VCC

### "HX711 not found" Errors
- **Check**: DOUT and SCK pin connections
- **Check**: HX711 power supply
- **Check**: Load cell wiring to HX711
- **Try**: Different HX711 module (may be defective)

### Connection Timeouts
- **Check**: Python script is running
- **Check**: Correct COM port in Python
- **Try**: Increase delay in `establishContact()` to 500ms

## 📝 Code Modification Examples

### Change to 3 Load Cells

1. Update array size:
```cpp
const byte ARRAY_SIZE = 3;  // Change from 2 to 3
float parameters[ARRAY_SIZE] = {force1, force2, force3};
```

2. Add third HX711:
```cpp
const int LOADCELL3_DOUT_PIN = 6;
const int LOADCELL3_SCK_PIN = 7;
HX711 loadcell3;
```

3. Update Python `SENSOR_COUNT = 3`

### Debug Mode (Print to Serial Monitor)

In `sendDataCallback()`, replace:
```cpp
system.Write(parameters, ARRAY_SIZE);
```

With:
```cpp
system.Print(parameters, ARRAY_SIZE);
```

### Adjust Update Rate

For faster updates (50ms):
```cpp
const int THREAD_INTERVAL = 50;  // Changed from 100
```

For more stable readings (reduce noise):
```cpp
const int SAMPLES_PER_READING = 20;  // Changed from 10
```

## 🔬 Advanced Features

### Custom Filtering

Add exponential moving average filter:
```cpp
// In global variables:
float filtered_force1 = 0;
float filtered_force2 = 0;
const float ALPHA = 0.3;  // Smoothing factor (0-1)

// In loop(), after force calculation:
filtered_force1 = ALPHA * force1 + (1 - ALPHA) * filtered_force1;
filtered_force2 = ALPHA * force2 + (1 - ALPHA) * filtered_force2;

// Use filtered values in parameters array
```

### Temperature Compensation

If your load cells drift with temperature:
```cpp
// Add temperature sensor reading
float temp = readTemperature();  // Your sensor
float temp_coeff = 0.001;        // Calibrate this
force1 = force1 * (1 + temp_coeff * (temp - 25));
```

## 📖 Class Documentation

### Monitor Class
- **Purpose**: Handle serial communication protocol
- **Methods**:
  - `establishContact()`: Wait for PC connection
  - `Write()`: Send data on handshake
  - `Print()`: Debug output

### Sistem Class  
- **Purpose**: System initialization wrapper
- **Methods**:
  - `initiate()`: Configure serial port
  - `establishContact()`: Delegate to Monitor
  - `Write()`: Delegate to Monitor

## 🔗 Related Resources

- **HX711 Library**: https://github.com/bogde/HX711
- **Thread Library**: https://github.com/ivanseidel/ArduinoThread
- **Load Cell Guide**: https://learn.sparkfun.com/tutorials/load-cell-amplifier-hx711-breakout-hookup-guide
- **Calibration Tool**: Use Python script in this repo

## 👨‍💻 Author

**Sertaç Emre Kara**  
Mechanical Engineering Instructor  
Fırat University, Turkey

Part of SCARA robot force control research project.

## 📄 Version History

- **v0.3** (Feb 2026): Generic load cell naming, comprehensive documentation
- **v0.2** (Sep 2020): Dual load cell support
- **v0.1**: Initial release

---

**Note**: This system was developed for research purposes. Calibration values are specific to the hardware used and must be recalibrated for your setup.
