/*
 * Dual Load Cell Data Acquisition System
 * 
 * This sketch reads data from two HX711 load cells and sends
 * the force values to a PC via serial communication.
 * 
 * Hardware:
 * - Load Cell 1 (Green-White cables): HX711 on pins 2-3
 * - Load Cell 2 (Yellow-Orange cables): HX711 on pins 4-5
 * 
 * Communication Protocol:
 * 1. Arduino sends 'C' to establish connection
 * 2. Python responds with 'H' (handshake)
 * 3. Arduino sends force values (one per line)
 * 4. Python sends 'Q' to quit
 * 
 * Author: Sertaç Emre Kara
 * Last Update: February 2026
 * Version: 0.3
 */

#include <Thread.h>
#include "HX711.h"
#include "Sistem.h"

// ==================== PIN DEFINITIONS ====================
// Load Cell 1 - Green-White cables
const int LOADCELL1_DOUT_PIN = 2;
const int LOADCELL1_SCK_PIN = 3;

// Load Cell 2 - Yellow-Orange cables
const int LOADCELL2_DOUT_PIN = 4;
const int LOADCELL2_SCK_PIN = 5;

// ==================== OBJECTS ====================
HX711 loadcell1;
HX711 loadcell2;
Sistem system;
Thread dataThread = Thread();

// ==================== CALIBRATION PARAMETERS ====================
// Load Cell 1 (500kg Zemic L6E3)
const float LOADCELL1_SCALE = 0.4699;
const float LOADCELL1_INTERCEPT = -35007;
const float LOADCELL1_OFFSET = -16.5;  // Newton

// Load Cell 2 (200kg Mavin NA2)
const float LOADCELL2_SCALE = 0.1723;
const float LOADCELL2_INTERCEPT = 378.56;
const float LOADCELL2_OFFSET = 4.8;  // Newton

// Conversion factor
const float KG_TO_NEWTON = 9.807;

// ==================== SAMPLING PARAMETERS ====================
const int SAMPLES_PER_READING = 10;  // Number of samples to average
const int THREAD_INTERVAL = 100;     // ms between data transmissions

// ==================== GLOBAL VARIABLES ====================
long reading1 = 0;
long reading2 = 0;
float average1 = 0;
float average2 = 0;
float force1 = 0;  // Force from load cell 1 (Newtons)
float force2 = 0;  // Force from load cell 2 (Newtons)
int sample_count = 0;


// ==================== CALLBACK FUNCTION ====================
/**
 * Thread callback function
 * Sends current force values to PC via serial
 */
void sendDataCallback() {
  const byte ARRAY_SIZE = 2;
  float parameters[ARRAY_SIZE] = {force1, force2}; 
  
  system.Write(parameters, ARRAY_SIZE);
}


// ==================== SETUP ====================
void setup() {
  // Initialize serial communication
  system.initiate();
  system.establishContact(); 
  
  // Initialize load cells
  loadcell1.begin(LOADCELL1_DOUT_PIN, LOADCELL1_SCK_PIN);
  loadcell2.begin(LOADCELL2_DOUT_PIN, LOADCELL2_SCK_PIN);
  
  // Configure data transmission thread
  dataThread.onRun(sendDataCallback);
  dataThread.setInterval(THREAD_INTERVAL);
}


// ==================== MAIN LOOP ====================
void loop() {
  // Check if data should be sent
  if (dataThread.shouldRun()) {
    dataThread.run();
  }
  
  // Read Load Cell 1
  if (loadcell1.is_ready()) {
    reading1 = loadcell1.read();
  }
  
  // Read Load Cell 2
  if (loadcell2.is_ready()) {
    reading2 = loadcell2.read();
  }
  
  // Accumulate readings for averaging
  average1 += reading1;
  average2 += reading2;
  sample_count++;
  
  // When enough samples collected, calculate forces
  if (sample_count >= SAMPLES_PER_READING) {
    // Calculate average raw values
    average1 /= SAMPLES_PER_READING; 
    average2 /= SAMPLES_PER_READING;
    
    // Convert Load Cell 1 reading to force (Newtons)
    // Formula: Force = ((SCALE * raw + INTERCEPT) * 0.001 * g) - OFFSET
    force1 = (LOADCELL1_SCALE * average1 + LOADCELL1_INTERCEPT) * 0.001;  // kg
    force1 = force1 * KG_TO_NEWTON - LOADCELL1_OFFSET;  // Newton
    
    // Convert Load Cell 2 reading to force (Newtons)
    force2 = (LOADCELL2_SCALE * average2 + LOADCELL2_INTERCEPT) * 0.001;  // kg
    force2 = force2 * KG_TO_NEWTON - LOADCELL2_OFFSET;  // Newton
    
    // Reset accumulators
    sample_count = 0;
    average1 = 0;
    average2 = 0;
  }
}


/*
 * ==================== CALIBRATION NOTES ====================
 * 
 * To recalibrate the load cells:
 * 
 * 1. Find zero offset:
 *    - Run with no load
 *    - Record the force values
 *    - Update LOADCELL1_OFFSET and LOADCELL2_OFFSET
 * 
 * 2. Find scale and intercept:
 *    - Apply known weights (e.g., 10kg, 20kg, 50kg)
 *    - Record raw ADC values for each weight
 *    - Plot: Weight (kg) vs Raw Value
 *    - Fit linear regression: Weight = SCALE * Raw + INTERCEPT
 *    - Update LOADCELL1_SCALE, LOADCELL1_INTERCEPT, etc.
 * 
 * 3. Example calibration procedure:
 *    a) Comment out the force conversion lines
 *    b) Uncomment the debug print line in sendDataCallback():
 *       system.Print(parameters, ARRAY_SIZE);
 *    c) Apply known loads and record raw values
 *    d) Calculate calibration coefficients
 *    e) Update constants above
 * 
 * Current calibration:
 * - Load Cell 1: Zemic L6E3 (500kg capacity)
 * - Load Cell 2: Mavin NA2 (200kg capacity)
 */
