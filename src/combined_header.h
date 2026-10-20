#include "Inputs.h"
#include "victory_music1.h"

#ifndef TOGGLE_LEDS_H
#define TOGGLE_LEDS_H

#include <Arduino.h>

// LED variables
extern const int RLED;      //red LED
extern const int GLED;      //green LED
extern const int PIR;       // PIR sensor input
const int startSwitch = 3;  // Switch to toggle game on/off
extern const int BUTTON;    // Button to indicate game win
extern bool gameStart;
extern bool red_light_on;
extern unsigned long LED_start_time;
const unsigned long timeDelay = 6000;  // 5 seconds
extern bool turn_green_light_on;
const long fast_blink_interval = 400;  // Blink interval of 1 second (1000ms)
bool final_LED_state = false;

//PIR variable
extern unsigned long detectionStartTime;       // Time when motion detection starts
extern unsigned long cooldownStartTime;        // Time when red light turns on
extern const unsigned long cooldownInterval;   // 1 second cooldown before detecting motion
extern const unsigned long detectionInterval;  // 5 second detection window
extern bool redLightOn;                        // Flag for red light state
extern bool detectingMotion;                   // Flag for whether we're in the motion detection window
extern bool motionDetected;                    // Flag for detecting motion
unsigned long prevMillis = 0;
unsigned long initialMillis = 0;
const long slow_blink_interval = 1000;  // 1-second interval for blinking
bool slow_RLED_on = false;
bool slow_GLED_on = false;
// Function declaration
void toggle_LEDS();
void setLEDS(const int, bool, const int, bool);
void blinkAlternatingLEDS(int RLED, int GLED);
void gameOverLEDS(int RLED, int GLED);

//PIR motion detection:  By Matthew
void detectMotionDuringRedLight(int SPEAKER, int PIR, bool DETECT) {
  if (DETECT) {
    unsigned long currentMillis = millis();

    if (redLightOn && !detectingMotion) {
      // Start the cooldown period after the red light turns on
      if (currentMillis - cooldownStartTime >= cooldownInterval) {
        detectingMotion = true;              // Enable motion detection after the cooldown
        detectionStartTime = currentMillis;  // Start the detection timer
        Serial.println("Motion detection started.");
      }
    }

    // When in motion detection mode, check for motion for 5 seconds
    if (detectingMotion) {
      if (currentMillis - detectionStartTime <= detectionInterval) {
        // Check if motion is detected
        if (digitalRead(PIR) == HIGH && !motionDetected) {
          motionDetected = true;
          tone(SPEAKER, 700, 2000);
          delay(2000);
          Serial.println("Motion detected! Player loses.");
          // Red light starts flashing (handled by another teammate)
        }
      } else {
        // No motion detected within 5 seconds, return to green light (handled by teammate)
        Serial.println("No motion detected. Turning green light on.");
        // delay(20);
        // tone(SPEAKER, 500, 1500);
        // delay(1500);
        detectingMotion = false;
        redLightOn = false;  // Reset for next cycle
        red_light_on = false;
        turn_green_light_on = true;
        LED_start_time = millis();
        setLEDS(RLED, false, GLED, true);
      }
    }
  }
}

//Called when red light is on, initializes some variables:   By Matthew
void startRedLight() {
  redLightOn = true;
  cooldownStartTime = millis();  // Start cooldown timer
  motionDetected = false;        // Reset motion detection
  detectingMotion = false;       // Ensure detection starts only after cooldown
  Serial.println("Red light on.");
}

// Sets LEDS:  By Vanessa
void setLEDS(int RLED, bool red, int GLED, bool green) {
  digitalWrite(RLED, red ? HIGH : LOW);
  digitalWrite(GLED, green ? HIGH : LOW);
}
//Flashes LEDS:  By Vanessa
void flashLEDS(int times, int delayTime) {
  for (int i = 0; i < times; i++) {
    setLEDS(RLED, true, GLED, true);  // Both LEDs on
    delay(delayTime);
    setLEDS(RLED, false, GLED, false);  // Both LEDs off
    delay(delayTime);
  }
}
//Handles LED changes during game: By group
void toggle_LEDS() {
  unsigned long currentMillis = millis();
  if (!red_light_on && currentMillis - LED_start_time >= timeDelay) {
    red_light_on = true;
    turn_green_light_on = false;
    setLEDS(RLED, true, GLED, false);  // Switch to red LED after delay
    startRedLight();
  }

  if (turn_green_light_on && currentMillis - LED_start_time >= timeDelay) {
    red_light_on = false;
    turn_green_light_on = true;

    setLEDS(RLED, false, GLED, true);
  }

  if (red_light_on) {
    if (digitalRead(PIR) == HIGH) {      // Motion detected
      setLEDS(RLED, GLED, true, false);  // Red LED blinks
      delay(200);
      setLEDS(RLED, GLED, false, false);
      delay(200);
    } else if (currentMillis - LED_start_time >= timeDelay + 5000) {
      red_light_on = false;              // Reset after timeout
      setLEDS(RLED, false, GLED, true);  // Green LED back on
      LED_start_time = currentMillis;
    }
  }
}
//Slowly blinks LEDS during game start: By Matthew and David
void blinkAlternatingLEDS(int RLED, int GLED) {
  unsigned long currMillis = millis();

  // Check if enough time has passed (non-blocking)
  if (currMillis - prevMillis >= slow_blink_interval) {
    prevMillis = currMillis;  // Update the last change time

    // Alternate the LEDs
    if (slow_RLED_on) {
      // Turn off red, turn on green
      setLEDS(RLED, false, GLED, true);
      slow_RLED_on = false;
      slow_GLED_on = true;
    } else {
      // Turn off green, turn on red
      setLEDS(RLED, true, GLED, false);
      slow_RLED_on = true;
      slow_GLED_on = false;
    }
  }
}
//Rapidly blinks lights during victory:  By Matthew and Vanessa
void gameOverLEDS(int RLED, int GLED) {
  unsigned long currMillis = millis();

  // Check if enough time has passed (non-blocking)
  if (currMillis - initialMillis >= fast_blink_interval) {
    initialMillis = currMillis;  // Update the last change time
    // Toggle LED state
    final_LED_state = !final_LED_state;
    // Set LEDs based on toggled state
    setLEDS(RLED, final_LED_state, GLED, final_LED_state);
  }
}

#endif
