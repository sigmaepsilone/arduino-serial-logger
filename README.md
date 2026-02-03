# Arduino Serial Multi-Sensor Data Logger

A Python-based GUI application for real-time data acquisition from Arduino sensors with Excel logging and visualization capabilities.

## 📋 Overview

This application reads multiple sensor data streams from an Arduino via serial communication, displays real-time values using GUI widgets, and logs all data to an Excel spreadsheet for later analysis. Designed for dual HX711 load cell systems.

## ✨ Features

- **Real-time Data Acquisition**: Continuous serial communication with Arduino
- **Multi-Sensor Support**: Handles multiple sensor inputs simultaneously
- **Excel Logging**: Automatic timestamped data logging to XLSX format
- **Visual Feedback**: Real-time gauge display for primary sensor
- **Connection Handshaking**: Robust connection establishment protocol
- **Clean Exit**: Proper resource cleanup on application close

## 🔧 Hardware Requirements

- Arduino board (tested with Arduino Uno/Teensy)
- Force sensors or other analog sensors
- USB cable for serial connection
- Windows PC (or modify COM port for Linux/Mac)

## 💻 Software Requirements

- Python 3.10 or higher
- Conda (recommended) or pip

## 📦 Installation

### Option 1: Using Conda (Recommended)

1. Clone the repository:
```bash
git clone https://github.com/yourusername/arduino_serial.git
cd arduino_serial
```

2. Create conda environment:
```bash
conda env create -f environment.yml
conda activate arduino_serial
```

### Option 2: Using pip

1. Clone the repository:
```bash
git clone https://github.com/yourusername/arduino_serial.git
cd arduino_serial
```

2. Create virtual environment and install dependencies:
```bash
python -m venv venv
# On Windows:
venv\Scripts\activate
# On Linux/Mac:
source venv/bin/activate

pip install -r requirements.txt
```

## 🚀 Usage

1. **Connect your Arduino** to the computer via USB

2. **Update the serial port** in the code if needed:
```python
SERIAL_PORT = 'COM4'  # Change to your port (COM3, COM5, etc.)
```

3. **Run the application**:
```bash
python arduino_serial_logger.py
```

4. The application will:
   - Establish connection with Arduino
   - Display real-time sensor values
   - Log data to `experiment.xlsx`

5. **Close the application** using the window close button (X) to ensure proper cleanup

## 📊 Arduino Setup

Your Arduino sketch should:

1. Send 'C' character upon startup to establish connection
2. Wait for 'H' (handshake) from Python
3. Send sensor data as newline-terminated strings
4. Handle 'Q' character for clean shutdown

Example Arduino code structure:
```cpp
void setup() {
  Serial.begin(115200);
  Serial.println('C');  // Connection signal
}

void loop() {
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    
    if (cmd == 'H') {  // Handshake received
      // Read and send sensor data
      float fz = readFzSensor();
      float fx = readFxSensor();
      
      Serial.println(fz);
      Serial.println(fx);
    }
    
    if (cmd == 'Q') {  // Quit command
      // Clean shutdown
    }
  }
}
```

## ⚙️ Configuration

Key parameters can be modified at the top of `arduino_serial_logger.py`:

```python
SERIAL_PORT = 'COM4'        # Serial port name
BAUD_RATE = 115200          # Communication speed
SENSOR_COUNT = 2            # Number of sensors
MAX_TIME = 10               # Time limit for display reset
EXCEL_FILENAME = 'experiment.xlsx'  # Output file name
```

## 📁 Project Structure

```
arduino_serial/
│
├── arduino_serial_logger.py    # Main application
├── requirements.txt             # Python dependencies
├── environment.yml              # Conda environment specification
├── .gitignore                   # Git ignore rules
└── README.md                    # This file
```

## 📝 Output Format

The Excel file (`experiment.xlsx`) contains:
- **Column A**: Timestamp (seconds from start)
- **Column B**: Load Cell 1 force (Newtons)
- **Column C**: Load Cell 2 force (Newtons)

## 🐛 Troubleshooting

### Serial Port Issues
- **"Could not open serial port"**: Check if the correct COM port is specified
- Run `python -m serial.tools.list_ports` to see available ports
- Ensure no other application is using the same port

### Connection Problems
- Verify Arduino is sending 'C' character on startup
- Check baud rate matches between Python and Arduino
- Try pressing the reset button on Arduino

### Import Errors
- Ensure all dependencies are installed: `pip install -r requirements.txt`
- For tkinter issues on Linux: `sudo apt-get install python3-tk`

## 🔄 Version History

- **v0.3** (Feb 2026): Code cleanup, English documentation, GitHub preparation
- **v0.2** (Jan 2021): Multi-sensor support, Excel logging
- **v0.1**: Initial release

## 👨‍💻 Author

**Sertaç Emre Kara**  
Mechanical Engineering Instructor  
Fırat University, Turkey

## 📄 License

This project is open source and available under the [MIT License](LICENSE).

## 🤝 Contributing

Contributions, issues, and feature requests are welcome!

## 🙏 Acknowledgments

- Built for robotics and mechatronics research at Fırat University
- Part of SCARA robot force control research project

---

**Note**: This application was developed for educational and research purposes in mechanical engineering and robotics applications.
