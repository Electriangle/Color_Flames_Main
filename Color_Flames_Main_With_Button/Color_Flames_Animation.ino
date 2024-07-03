/* 
 *  ========== Color Flame Animation ==========
 *  FUNCTION CALL:   Fire(Color, Cooling, Sparks, DelayDuration, ReverseDirection);
 *  
 *  PARAMETERS:
 *  Color - select the color of the flame with one of the following numbers:
    0 = white
    1 = red
    2 = yellow
    3 = green
    4 = cyan
    5 = blue
    6 = purple
    7 = pink
 *  Cooling - Use larger value for shorter flames; default=50; suggested range 20-100.
 *  Sparks - Use larger value for more ignitions and a more active fire (between 0 to 255); default=100; suggested range 50-200.
 *  DelayDuration - Use larger value for slower flame speed; default=10.
 *  ReverseDirection - change the origin of the flame from the bottom of the strip to the top; default=false.
*/

void Fire(int Color, int Cooling, int Sparks, int DelayDuration, bool ReverseDirection) {
  static byte heat[NUM_LEDS];   // Array of temperature readings at each simulation cell
  int cooldown;
  
  // 1) Slight cool down for each cell
  for (int i = 0; i < NUM_LEDS; i++) {
    cooldown = random(0, ((Cooling * 10) / NUM_LEDS) + 2);
    
    if(cooldown > heat[i]) {
      heat[i] = 0;
    }
    else {
      heat[i] = heat[i] - cooldown;
    }
  }
  
  // 2) Heat from each cell drifts up and diffuses slightly
  for (int k = (NUM_LEDS - 1); k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }
  
  // 3) Randomly ignite new Sparks near bottom of the flame
  if (random(255) < Sparks) {
    int y = random(7);
    heat[y] = heat[y] + random(160, 255);
  }
  
  // 4) Convert heat cells to LED colors
  for (int n = 0; n < NUM_LEDS; n++) {
    // Temperature ranges from 0 (black/cold) to 255 (white/hot)
    // Rescale heat from 0-255 to 0-191
    byte temperature = heat[n];
    byte t192 = round((temperature / 255.0) * 191);
    
    // Calculate ramp up from
    byte heatramp = t192 & 0x3F; // 0...63
    heatramp <<= 2; // scale up to 0...252

    int BottomColor[8][3] = {   // Array of Bottom flame colors (for spark)
      {heatramp/2, heatramp/2, 255},      // 0 - blue sparks on white flame
      {255, 255, heatramp},               // 1 - white/yellow sparks on red flame
      {255, heatramp, heatramp},          // 2 - white/red sparks on yellow flame
      {heatramp, heatramp, 255},          // 3 - white/blue sparks on green flame
      {heatramp, heatramp, 255},          // 4 - white/blue sparks on cyan flame
      {255, 255, heatramp},               // 5 - white/yellow sparks on blue flame
      {255, heatramp, heatramp},          // 6 - white/red sparks on purple flame
      {255, heatramp, heatramp},          // 7 - white/red sparks on pink flame
    };
    int MiddleColor[8][3] = {   // Array of Middle flame colors
      {heatramp/2, heatramp/2, heatramp},   // 0 - white/blue
      {255, heatramp, 0},                   // 1 - red/yellow
      {heatramp, heatramp, 0},              // 2 - yellow
      {0, 255, heatramp/2},                 // 3 - green/blue
      {0, heatramp, heatramp},              // 4 - cyan
      {0, heatramp, 255},                   // 5 - blue/green
      {heatramp/3, 0, heatramp/2},          // 6 - purple
      {heatramp, heatramp/4, heatramp*2/3}, // 7 - pink
    };
    int TopColor[8][3] = {      // Array of Top flame colors
      {heatramp, heatramp, heatramp},       // 0 - white
      {heatramp, 0, 0},                     // 1 - red
      {heatramp, heatramp, 0},              // 2 - yellow
      {0, heatramp, 0},                     // 3 - green
      {0, heatramp, heatramp},              // 4 - cyan
      {0, 0, heatramp},                     // 5 - blue
      {heatramp/3, 0, heatramp/2},          // 6 - purple
      {heatramp, heatramp/4, heatramp*2/3}, // 7 - pink
    };

    int Pixel = n;
    if (ReverseDirection) {Pixel = (NUM_LEDS - 1) - n;}

    // Set Pixels according to the three regions of the flame:
    if (t192 > 0x80) {                    // hottest (bottom of flame, heatramp between yellow and white)
      leds[Pixel].setRGB(round(BottomColor[Color][0]), round(BottomColor[Color][1]), round(BottomColor[Color][2]));
    }
    else if (t192 > 0x40) {               // middle (midde of flame, heatramp with analogous Color)
      leds[Pixel].setRGB(round(MiddleColor[Color][0]), round(MiddleColor[Color][1]), round(MiddleColor[Color][2]));
    }
    else {                                // coolest (top of flame, heatramp heatramp with monochromatic Color)
      leds[Pixel].setRGB(round(TopColor[Color][0]), round(TopColor[Color][1]), round(TopColor[Color][2]));
    }

  }
  
  FastLED.show();
  delay(DelayDuration);
}
