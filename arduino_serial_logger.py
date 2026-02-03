"""
Arduino Serial Multi-Sensor Data Logger
Author: Sertaç Emre Kara
Version: 0.3
Last Updated: February 2026

Description:
    This program reads multiple sensor data from Arduino via serial port
    and logs the data to an Excel spreadsheet with real-time visualization.
    Currently configured for 2 sensors (Normal Force Fz and Tangential Force Fx).
"""

from tkinter import Tk, Label, messagebox
import serial
import serial.tools.list_ports
from tk_tools import Gauge
import xlsxwriter
import time


# ==================== CONFIGURATION ====================
SERIAL_PORT = 'COM4'
BAUD_RATE = 115200
SENSOR_COUNT = 2
MAX_TIME = 10  # seconds
EXCEL_FILENAME = 'experiment.xlsx'

# ==================== GLOBAL VARIABLES ====================
t = 0
t_new = 0
t_old = 0
row = 0
col = 0
first_contact = False
serial_count = 0
start_time = 0
this_time = 0

# Initialize data arrays
serial_in_array = [0] * SENSOR_COUNT
sensor_values = [0] * SENSOR_COUNT
sensor_values_old = [0] * SENSOR_COUNT


# ==================== FUNCTIONS ====================
def on_closing():
    """Handle window closing event with confirmation dialog"""
    if messagebox.askokcancel("Quit", "Do you want to quit?"):
        arduino.write("Q".encode())
        arduino.close()
        workbook.close()
        window.destroy()


def serial_event():
    """
    Main serial communication handler
    Reads data from Arduino, processes it, and writes to Excel
    """
    global t, t_new, t_old, row, col
    global first_contact, serial_count
    global serial_in_array, sensor_values, sensor_values_old
    global start_time, this_time
    
    in_byte = arduino.readline()
    
    if in_byte:
        if not first_contact:
            # Establish initial connection with Arduino
            if in_byte == b'C':
                print('Connection Established!')
                arduino.flushInput()
                first_contact = True
                arduino.write("H".encode())
                start_time = time.time()
            else:
                print('Connection Failed!')
        else:
            # Process incoming sensor data
            elapsed_time = time.time() - start_time
            
            serial_in_array[serial_count] = in_byte
            serial_count += 1
            
            if serial_count >= SENSOR_COUNT:
                # Parse all sensor values
                for i in range(SENSOR_COUNT):
                    sensor_values[i] = float((serial_in_array[i])[:-2])
                
                # ==================== REAL-TIME VISUALIZATION ====================
                gauge.grid()
                gauge.set_value(sensor_values[0])
                
                # ==================== EXCEL LOGGING ====================
                worksheet.write(row + 1, 0, elapsed_time, center_format)
                
                for i in range(SENSOR_COUNT):
                    worksheet.write(row + 1, i + 1, sensor_values[i], center_format)
                
                # Update variables for next iteration
                sensor_values_old = sensor_values.copy()
                t_old = t_new
                t_new += elapsed_time - this_time
                t += elapsed_time
                row += 1
                
                # Print current values to console
                print(f"Load Cell 1: {sensor_values[0]:.2f} N, Load Cell 2: {sensor_values[1]:.2f} N")
                
                # Prepare for next data packet
                arduino.flushInput()
                arduino.write("H".encode())
                serial_count = 0
                this_time = elapsed_time
            
            # Reset time tracking if max time reached
            if t_new >= MAX_TIME:
                t_new = 0
                t_old = 0
    
    # Schedule next serial check
    window.after(1, serial_event)


# ==================== MAIN PROGRAM ====================
if __name__ == "__main__":
    print("=" * 50)
    print("Arduino Serial Data Logger")
    print("=" * 50)
    
    # Initialize serial connection
    try:
        arduino = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=.1)
        print(f"Connected to {SERIAL_PORT} at {BAUD_RATE} baud")
    except serial.SerialException as e:
        print(f"Error: Could not open serial port {SERIAL_PORT}")
        print(f"Details: {e}")
        exit(1)
    
    # Initialize Tkinter window
    window = Tk()
    window.title("Arduino Data Logger")
    window.geometry("400x300")
    
    # Create Excel workbook
    workbook = xlsxwriter.Workbook(EXCEL_FILENAME)
    worksheet = workbook.add_worksheet()
    worksheet.set_tab_color('red')
    
    # Define Excel formatting
    header_format = workbook.add_format({
        'bold': True,
        'font_color': 'red',
        'italic': True,
        'align': 'center'
    })
    
    center_format = workbook.add_format({'align': 'center'})
    
    # Write Excel headers
    worksheet.write(0, 0, 'Time (s)', header_format)
    
    sensor_names = ['Load Cell 1 (N)', 'Load Cell 2 (N)']
    for i, name in enumerate(sensor_names):
        worksheet.write(0, i + 1, name, header_format)
    
    # Create gauge widget for Force display
    gauge = Gauge(
        parent=window,
        max_value=20000.0,
        label='Load Cell 1',
        unit='N'
    )
    
    # Start serial communication
    serial_event()
    
    # Set window close protocol
    window.protocol("WM_DELETE_WINDOW", on_closing)
    
    # Start GUI main loop
    window.mainloop()
