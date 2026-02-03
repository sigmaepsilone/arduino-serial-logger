/*
 * Sistem (System) Class - System Management Wrapper
 * 
 * This class provides a high-level interface for system initialization
 * and serial communication. It wraps the Monitor class and handles
 * serial port configuration.
 * 
 * Usage:
 *   Sistem system;
 *   system.initiate();           // Initialize serial port
 *   system.establishContact();   // Connect with PC
 *   system.Write(data, size);    // Send data
 * 
 * Author: Sertaç Emre Kara
 * Last Update: February 2026
 * Version: 0.3
 */

#ifndef SISTEM_LIB_H
#define SISTEM_LIB_H

#include <Arduino.h>
#include "Monitor.h"

// ==================== SERIAL PORT CONFIGURATION ====================
// Define which serial port to use for PC communication
// Change this if using Serial1, Serial2, etc.
#define SISTEM_SERIAL Serial 

class Sistem {

public: 
    /**
     * Constructor
     * Initializes Monitor object with configured serial port
     */
    Sistem();
    
    /**
     * Initialize serial communication
     * Sets baud rate to 115200 and timeout to 10ms
     */
    void initiate();
    
    /**
     * Establish connection with PC
     * Delegates to Monitor::establishContact()
     */
    void establishContact();
    
    /**
     * Print formatted data array (for debugging)
     * Delegates to Monitor::Print()
     * @param data Array of float values
     * @param arrSize Number of elements in array
     */
    void Print(float data[], int arrSize);
    
    /**
     * Write data array to serial port (for logging)
     * Delegates to Monitor::Write()
     * @param data Array of float values
     * @param arrSize Number of elements in array
     */
    void Write(float data[], int arrSize);

private:
    Monitor _monitor;  // Monitor object for serial communication
};

#endif
