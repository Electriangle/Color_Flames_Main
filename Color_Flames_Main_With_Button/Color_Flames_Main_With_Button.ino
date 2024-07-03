/*
 * Title: Color_Flames_Main_With_Button
 * 
 * Description:
 *      - This sketch was created for controling a WS2812B LED light strip (or similar) using the FastLED library. 
 *      - Special functionality added: An input button can be used to switch between each of the different flame colors. 
 *        NOTE: Button pin has been set to use internal pull-up resistor. (If selected pin does not have an internal pull-up available,
 *              connect an external pull-up resistor). The button should be connected across the input pin to ground. 
 *      
 * Author: Electriangle
 *      - Channel: https://www.youtube.com/@Electriangle
 *      - YouTube Video Demonstration: https://www.youtube.com/watch?v=umW_gA3FTrY
 *      
 * License: MIT License
 *      - Copyright (c) 2024 Electriangle
 *
 * Date Created: 11/26/2023
 * Last Updated: 7/3/2024
*/

#include "FastLED.h"

#define NUM_LEDS          150    // Enter the total number of LEDs on the strip
#define LED_PIN           4      // The pin connected to DATA line to control the LEDs
#define INPUT_BUTTON_PIN  2      // The pin of the input button

int input_state = HIGH;          // The current state of the output pin
int color_state;                 // The current state of color
int button_state;                // The current reading from the input pin
int last_button_state = LOW;     // The previous reading from the input pin
unsigned long last_debounce_time = 0;  // The last time the output pin was toggled
unsigned long debounce_delay = 50;     // The debounce time; increase if the output flickers

CRGB leds[NUM_LEDS];

void setup() {
  pinMode(INPUT_BUTTON_PIN, INPUT_PULLUP);
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 1500);    // Set power limit of LED strip to 5V, 1500mA
  FastLED.clear();                                    // Initialize all LEDs to "OFF"
}

void loop() {
  // Read the state of the switch into a local variable:
  int input_reading = digitalRead(INPUT_BUTTON_PIN);

  // If the switch changed, due to noise or pressing:
  if (input_reading != last_button_state) {
    // reset the debouncing timer
    last_debounce_time = millis();
  }

  if ((millis() - last_debounce_time) > debounce_delay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (input_reading != button_state) {
      button_state = input_reading;

      // Only toggle the color state if the new button state is LOW:
      if (input_reading == LOW) {
        color_state = (color_state + 1) % 9;
      }
    }
  }

  // Save the reading. Next time through the loop, it'll be the last_button_state:
  last_button_state = input_reading;

  if (color_state <= 7) {
    // Set LEDs to a color based on state from input button
    Fire(color_state, 50, 100, 10, false);
  }
  else {
    // Turn OFF all the LEDs
    fadeToBlackBy(leds, NUM_LEDS, 3);
    FastLED.show();
  }
}