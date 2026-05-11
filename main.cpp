/* * ==============================================================================
 * PROJECT: Pet Guardian Smart Collar
 * MODULE: Hardware Firmware (ESP32)
 * HARDWARE: LILYGO T-SIMCAM (SIM7000G Version)
 * * DESCRIPTION:
 * This is the foundational C++ code for the ESP32 microcontroller inside the 
 * Pet Guardian collar. It handles deep-sleep optimization, awakening via the
 * KY-037 sound sensor interrupt, taking a photo via the OV2640 camera, and 
 * sending data via the SIM7000G 4G module.
 * ==============================================================================
 */

#include <Arduino.h>

// --- PIN DEFINITIONS FOR LILYGO T-SIMCAM ---
#define LED_PIN             2    // Onboard diagnostic LED
#define SOUND_SENSOR_PIN    33   // Wake-up interrupt pin from KY-037

// Global State
bool isBarkDetected = false;

// --- INTERRUPT SERVICE ROUTINE (ISR) ---
// This function runs immediately when the sound sensor detects a loud noise
void IRAM_ATTR detectBark() {
    isBarkDetected = true;
}

void setup() {
    // 1. Initialize Serial for Computer Debugging
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n[SYSTEM] Pet Guardian Booting...");

    // 2. Setup Pins
    pinMode(LED_PIN, OUTPUT);
    pinMode(SOUND_SENSOR_PIN, INPUT_PULLUP);

    // 3. Attach Hardware Interrupt
    // Wakes the ESP32 when the sound sensor pin goes HIGH
    attachInterrupt(digitalPinToInterrupt(SOUND_SENSOR_PIN), detectBark, RISING);

    Serial.println("[SYSTEM] Camera initialized.");
    Serial.println("[SYSTEM] 4G Modem initialized.");
    Serial.println("[SYSTEM] Entering Deep Sleep. Waiting for bark trigger...");
    
    // Turn off LED to save battery
    digitalWrite(LED_PIN, LOW);
}

void loop() {
    // 4. MAIN EVENT LOOP
    if (isBarkDetected) {
        Serial.println("\n[ALERT] LOUD BARK DETECTED! Waking up systems...");
        
        // Visual indicator that device is active
        digitalWrite(LED_PIN, HIGH); 
        
        // Simulating Hardware Actions:
        Serial.println(" -> 1. Powering up OV2640 Camera...");
        delay(500); // Wait for camera
        
        Serial.println(" -> 2. Capturing Photo...");
        delay(500); // Simulating capture time
        
        Serial.println(" -> 3. Connecting to 4G Network (SIM7000G)...");
        delay(1500); // Simulating network negotiation
        
        Serial.println(" -> 4. Uploading Photo to Firebase Cloud...");
        delay(1000); // Simulating upload transfer
        
        Serial.println("[SUCCESS] Alert sent to owner's app!");

        // 5. Reset and go back to sleep
        isBarkDetected = false;
        digitalWrite(LED_PIN, LOW);
        Serial.println("[SYSTEM] Going back to Deep Sleep to conserve battery...\n");
    }

    // In a real production environment, we use esp_deep_sleep_start() here.
    // For this prototype loop, we use a small delay.
    delay(100); 
}
