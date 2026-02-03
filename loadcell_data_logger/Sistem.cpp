/*
 * Sistem (System) Class Implementation
 * 
 * Author: Sertaç Emre Kara
 * Last Update: February 2026
 * Version: 0.3
 */

#include "Sistem.h"

/**
 * Constructor
 * Initialize Monitor with the configured serial port
 */
Sistem::Sistem() : _monitor(SISTEM_SERIAL) {
    // Monitor is initialized with serial port reference
}

/**
 * Initialize serial communication
 * Configures baud rate and timeout for optimal data transfer
 */
void Sistem::initiate() {
    SISTEM_SERIAL.begin(115200);   // High speed for real-time data
    SISTEM_SERIAL.setTimeout(10);  // 10ms timeout for reads
}

/**
 * Establish connection with PC
 * Wrapper for Monitor::establishContact()
 */
void Sistem::establishContact() {
    _monitor.establishContact();
}

/**
 * Print formatted data array to serial
 * Useful for debugging - shows human-readable values
 * 
 * @param data Array of float values
 * @param arrSize Number of elements
 */
void Sistem::Print(float data[], int arrSize) {
    _monitor.Print(data, arrSize);
}

/**
 * Write data array to serial for PC logging
 * Main method for sending sensor data to Python script
 * 
 * @param data Array of float values (e.g., force readings)
 * @param arrSize Number of elements
 */
void Sistem::Write(float data[], int arrSize) {
    _monitor.Write(data, arrSize);
}
