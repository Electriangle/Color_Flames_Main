/*
 * Title: Color_Flames_Main
 * 
 * Description:
 *      - This sketch was created for controling a WS2812B LED light strip (or similar) using the FastLED library. 
 *      
 * Author: Electriangle
 *      - Channel: https://www.youtube.com/@Electriangle
 *      - YouTube Video Demonstration: https://www.youtube.com/watch?v=umW_gA3FTrY
 *      
 * License: MIT License
 *      - Copyright (c) 2023 Electriangle
 *
 * Date Created: 11/26/2023
 * Last Updated: 11/26/2023
*/

#include "FastLED.h"

#define NUM_LEDS  150    // Enter the total number of LEDs on the strip
#define PIN       7      // The pin connected to DATA line to control the LEDs

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<WS2812B, PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 1500);    // Set power limit of LED strip to 5V, 1500mA
  FastLED.clear();                                    // Initialize all LEDs to "OFF"
}

void loop() {
  // A white flame. Change parameters according to description in "Color_Flames_Animation" tab.
  Fire(0, 50, 100, 10, false);
}