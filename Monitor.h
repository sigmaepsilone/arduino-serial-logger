/*
 * Monitor Class - Serial Communication Handler
 * 
 * This class manages serial communication with a PC for data logging.
 * It implements a handshake protocol and provides methods for sending
 * sensor data arrays.
 * 
 * Communication Protocol:
 * - establishContact(): Sends 'C' until PC responds
 * - Write(): Waits for 'H', then sends data array
 * - Print(): Formatted output for debugging
 * 
 * Author: Sertaç Emre Kara
 * Last Update: February 2026
 * Version: 0.3
 */

#ifndef MONITOR_H
#define MONITOR_H

#include <Arduino.h>

class Monitor {
  
public:
    /**
     * Constructor
     * @param port Serial port to use (e.g., Serial, Serial1)
     */
    Monitor(Stream& port);

    /**
     * Establish initial connection with PC
     * Sends 'C' continuously until response received
     */
    void establishContact();
    
    /**
     * Print formatted data to serial monitor (for debugging)
     * Format: param0=value || param1=value || ...
     * @param data Array of float values to print
     * @param arrSize Number of elements in array
     */
    void Print(float data[], int arrSize); 

    /**
     * Write data array to serial port (for data logging)
     * Waits for 'H' handshake, then sends values line-by-line
     * @param data Array of float values to send
     * @param arrSize Number of elements in array
     */
    void Write(float data[], int arrSize); 

private:
    Stream& SERIAL_PORT;  // Reference to serial port
};

#endif
