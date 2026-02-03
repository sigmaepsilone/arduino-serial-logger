/*
 * Monitor Class Implementation
 * 
 * Author: Sertaç Emre Kara
 * Last Update: February 2026
 * Version: 0.3
 */

#include "Monitor.h"

/**
 * Constructor - Initialize with serial port reference
 */
Monitor::Monitor(Stream& port) : SERIAL_PORT(port) {
    // No additional initialization needed
}

/**
 * Establish connection with PC
 * Continuously sends 'C' character until PC responds
 * This ensures Python script is ready before data transmission
 */
void Monitor::establishContact() {
    while (SERIAL_PORT.available() <= 0) {
        SERIAL_PORT.print('C');  // Send connection signal
        delay(200);              // Wait 200ms between attempts
    }
}

/**
 * Print formatted data for debugging
 * Output format: param0=1.234 || param1=5.678 || ...
 * Useful for monitoring values in Serial Monitor
 * 
 * @param data Array of float values
 * @param arrSize Number of elements in array
 */
void Monitor::Print(float data[], int arrSize) {
    for (byte i = 0; i <= arrSize; i++) {
        SERIAL_PORT.print("param");
        SERIAL_PORT.print(i);
        SERIAL_PORT.print("=");
        SERIAL_PORT.print(data[i], 3);  // 3 decimal places
        SERIAL_PORT.print(" || ");
    }
    SERIAL_PORT.println("");  // End line
}

/**
 * Write data array to serial port for PC logging
 * 
 * Protocol:
 * 1. Wait for 'H' (handshake) command from PC
 * 2. Send each data value on separate line
 * 3. If 'Q' (quit) received, re-establish connection
 * 
 * @param data Array of float values to send
 * @param arrSize Number of elements in array
 */
void Monitor::Write(float data[], int arrSize) {
    if (SERIAL_PORT.available() > 0) {
        uint8_t inByte = SERIAL_PORT.read();
        
        // Handshake command - send data
        if (inByte == 'H') {
            // Send each value on new line (3 decimal places)
            for (int i = 0; i <= arrSize - 1; i++) {
                SERIAL_PORT.println(data[i], 3);
            }
        }
        
        // Quit command - reset connection
        if (inByte == 'Q') {
            Monitor::establishContact();
        }
    }
}
