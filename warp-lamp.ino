#include <Adafruit_NeoPixel.h>

#define PIN 6

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(18, PIN, NEO_GRB + NEO_KHZ800);

uint32_t warpColor = strip.Color(0,255,240);
uint32_t alertColor = strip.Color(255,0,0);

uint32_t mainColor = warpColor;
uint32_t injectorColor = strip.Color(255,255,255);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {


  strip.setPixelColor(5, injectorColor);
  strip.setPixelColor(6, injectorColor);
  strip.setPixelColor(7, injectorColor);
  warp();
  //allOn();
}

void allOn()
{
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, mainColor);
  }

  strip.show();
}

void warp()
{
    for(uint16_t i=0; i<6; i++) {
       
        if(i != 5){
            strip.setPixelColor(i, mainColor);
            strip.setPixelColor(17 - i, mainColor);
            strip.setPixelColor(12 - i, mainColor);
        }
        for(uint16_t j=0; j<5; j++) {
            if(i != j){
                strip.setPixelColor(j, 0);
            }
        }

        for(uint16_t j=0; j<5; j++) {
            if(17 - i != 17-j){
                strip.setPixelColor(17-j, 0);
            }
        }

        for(uint16_t j=0; j<5; j++) {
            if(12 - i != 12-j){
                strip.setPixelColor(12-j, 0);
            }
        }

        strip.show();
        delay(300);
    }
}
