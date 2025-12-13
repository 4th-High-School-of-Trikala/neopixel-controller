#include "lights.h"

#define LED_BRIGHTNESS 127

LightsModule::LightsModule(int neopixel_pin, int neopixel_count)
    : neopixel_strip_{neopixel_count, neopixel_pin, NEO_GRB + NEO_KHZ800},
      program_{nullptr} {
  this->neopixel_strip_.setBrightness(LED_BRIGHTNESS);
  this->neopixel_strip_.begin();
  this->neopixel_strip_.show();
}

uint32_t LightsModule::ConvertColor_(Color color) {
  return this->neopixel_strip_.Color(color.r, color.g, color.b);
}

void LightsModule::Clear() {
  this->neopixel_strip_.clear();
  this->neopixel_strip_.show();
}

void LightsModule::Loop() {
  this->Clear();
  Serial.println(freeRam());
  this->neopixel_strip_.setBrightness(LED_BRIGHTNESS);

  if (this->program_ != nullptr) this->program_(this);
  this->neopixel_strip_.clear();
  this->StarFlash();
}

int freeRam() {

  extern int __heap_start,*__brkval;

  int v;

  return (int)&v - (__brkval == 0  

    ? (int)&__heap_start : (int) __brkval);  

}

void LightsModule::SetProgram(void (*program)()) {
  this->program_ = program;
}

void LightsModule::SetToColor(int led, uint32_t color) {
  this->neopixel_strip_.setPixelColor(led, color);
  this->neopixel_strip_.show();
}

void LightsModule::SetToColor(int led, Color color) {
  this->SetToColor(led, this->ConvertColor_(color));
}

void LightsModule::SetAllToColor(uint32_t color) {
  for (int i = 0; i < this->neopixel_strip_.numPixels(); ++i) {
    this->neopixel_strip_.setPixelColor(i, color);
  }

  this->neopixel_strip_.show();
}

void LightsModule::SetAllToColor(Color color) {
  this->SetAllToColor(this->ConvertColor_(color));
}

void LightsModule::SetHalfToColor(uint32_t color1, uint32_t color2) {
  for (int i = 0; i < this->neopixel_strip_.numPixels(); ++i) {
    switch (i % 2) {
      case 0:
        this->neopixel_strip_.setPixelColor(i, color1);
        break;
      case 1:
        this->neopixel_strip_.setPixelColor(i, color2);
        break;
    }
  }

  this->neopixel_strip_.show();
}

void LightsModule::SetHalfToColor(Color color1, Color color2) {
  this->SetHalfToColor(this->ConvertColor_(color1), this->ConvertColor_(color2));
}

void LightsModule::BlinkToColor(int led, uint32_t color, int duration) {
  uint32_t tmp = this->neopixel_strip_.getPixelColor(led);

  this->SetToColor(led, color);
  delay(duration);
  this->SetToColor(led, tmp);
}

void LightsModule::BlinkToColor(int led, Color color, int duration) {
  this->BlinkToColor(led, this->ConvertColor_(color), duration);
}

void LightsModule::BlinkAllToColor(uint32_t color, int duration) {
  uint32_t tmp = this->neopixel_strip_.getPixelColor(0);

  this->SetAllToColor(color);
  delay(duration);
  this->SetAllToColor(tmp);
}

void LightsModule::BlinkAllToColor(Color color, int duration) {
  this->BlinkAllToColor(this->ConvertColor_(color), duration);
}

void LightsModule::ColorWipeAll(uint32_t color, int duration) {
  for (int i = 0; i < this->neopixel_strip_.numPixels(); ++i) {
    this->SetToColor(i, color);
    delay(duration);
    ++i;
    this->SetToColor(i, Color {127, 127, 127});
    delay(duration); 
  }
}

void LightsModule::ColorWipeAll(Color color, int duration) {
  this->ColorWipeAll(this->ConvertColor_(color), duration);
}

void LightsModule::Snake(uint32_t color, int duration, int snake_length) {
  for (int i = 0; i < this->neopixel_strip_.numPixels()+snake_length; ++i) {
    this->SetToColor(i, color);
    delay(duration);
    ++i;
    this->SetToColor(i, Color {127, 127, 127});

     this->SetToColor(i-snake_length-1, Color {0, 0, 0});
    this->SetToColor(i-snake_length, Color {0, 0, 0});
    delay(duration);
  }
}

void LightsModule::Snake(Color color, int duration, int snake_length) {
  this->Snake(this->ConvertColor_(color), duration, snake_length);
}

void LightsModule::ChaseAll(uint32_t color, int duration) {
  for (int i = 0; i < 10; ++i) {
    for (int ii = 0; ii <= 2; ++ii) {
      this->neopixel_strip_.clear();

      for (int n = ii; n < this->neopixel_strip_.numPixels(); n += 2) {
        this->neopixel_strip_.setPixelColor(n, color);
      }

      this->neopixel_strip_.show();
      delay(duration);
    }
  }
}

void LightsModule::ChaseAll(Color color, int duration) {
  this->ChaseAll(this->ConvertColor_(color), duration);
}

#define SPRINKLE_LEN 12

void LightsModule::Sprinkle(uint32_t color) {
  int location = random(0, this->neopixel_strip_.numPixels() - SPRINKLE_LEN);
  int location2 = random(0, this->neopixel_strip_.numPixels() - SPRINKLE_LEN);
  for (int k = 0; k < 5; ++k) {
    for (int i = 0; i <= SPRINKLE_LEN; ++i)
    {
      this->neopixel_strip_.setPixelColor(location + i, color);
      this->neopixel_strip_.setPixelColor(location2 + i, color);
    }
  this->neopixel_strip_.show();
    
  delay(40);

  for (int i = 0; i <= SPRINKLE_LEN; ++i)
  {
    this->neopixel_strip_.setPixelColor(location + i, 0);
    this->neopixel_strip_.setPixelColor(location2 + i, 0);
  }  this->neopixel_strip_.show();
  delay(40);
}
}

void LightsModule::Sprinkle(Color color) {
  this->Sprinkle(this->ConvertColor_(color));
}

void LightsModule::GlowRandom(uint32_t color, int amount) {
  int leds[amount];
  for (int i = 0; i < amount; ++i)
    leds[i] = random(0, this->neopixel_strip_.numPixels());
  
  this->neopixel_strip_.setBrightness(1);
  for (int led : leds)
    this->neopixel_strip_.setPixelColor(led, color);
  this->neopixel_strip_.show();

  for (int i = 1; i <= 100; ++i) {
    this->neopixel_strip_.setBrightness(i);
    this->neopixel_strip_.show();
    delay(25);
  }

  for (int i = 1; i <= 100; ++i) {
    this->neopixel_strip_.setBrightness(100 - i);
    this->neopixel_strip_.show();
    delay(25);
  }
}

void LightsModule::GlowRandom(Color color, int amount) {
  this->GlowRandom(this->ConvertColor_(color), amount);
}

void LightsModule::Merge(uint32_t color_a, uint32_t color_b) {
  for (int i = 0; i <= this->neopixel_strip_.numPixels() / 2; ++i) {
    this->neopixel_strip_.setPixelColor(i, color_a);
    this->neopixel_strip_.setPixelColor(this->neopixel_strip_.numPixels() - i, color_b);
    this->neopixel_strip_.show();
    delay(25);
  }

  for (int i = 0; i <= this->neopixel_strip_.numPixels() / 2; i += 2) {
    this->neopixel_strip_.setPixelColor((this->neopixel_strip_.numPixels() / 2) + i, color_a);
    this->neopixel_strip_.setPixelColor((this->neopixel_strip_.numPixels() / 2) - i, color_b);
    this->neopixel_strip_.show();
    delay(25);
  }
}

void LightsModule::Merge(Color color_a, Color color_b) {
  this->Merge(this->ConvertColor_(color_a), this->ConvertColor_(color_b));
}

void LightsModule::MergeSnake(uint32_t color_a, uint32_t color_b, int amount) {
  for (int i = 0; i <= this->neopixel_strip_.numPixels() + amount; ++i) {
    this->neopixel_strip_.setPixelColor(i, color_a);
    this->neopixel_strip_.setPixelColor(i - amount, 0);

    this->neopixel_strip_.setPixelColor(this->neopixel_strip_.numPixels() - i, color_b);
    this->neopixel_strip_.setPixelColor(this->neopixel_strip_.numPixels() - i + amount, 0);

    this->neopixel_strip_.show();
    delay(25);
  }
}

void LightsModule::MergeSnake(Color color_a, Color color_b, int amount) {
  this->MergeSnake(this->ConvertColor_(color_a), this->ConvertColor_(color_b), amount);
}

void LightsModule::IncreaseBrightness() {
  this->neopixel_strip_.setBrightness(this->neopixel_strip_.getBrightness() + 1);
}

void LightsModule::DecreaseBrightness() {
  this->neopixel_strip_.setBrightness(this->neopixel_strip_.getBrightness() - 1);
}

#define STAR_LEDS 12
#define NUM_OF_FLASHES 10

void LightsModule::StarFlash() {
  for (int x = 0; x < NUM_OF_FLASHES; x++) {
    for (int i = this->neopixel_strip_.numPixels() - STAR_LEDS; i < this->neopixel_strip_.numPixels(); i++) {
      this->neopixel_strip_.setPixelColor(i, this->ConvertColor_(Color { 255, 255, 255 }));
    }
    this->neopixel_strip_.show();
    delay(15);
    for (int i = this->neopixel_strip_.numPixels() - STAR_LEDS; i < this->neopixel_strip_.numPixels(); i++) {
      this->neopixel_strip_.setPixelColor(i, this->ConvertColor_(Color { 0, 0, 0 }));
    }
    this->neopixel_strip_.show();
    delay(15);
  }
}

void LightsModule::Fade(Color color) {
  for (int i = 0; i < 256; i++) {
    this->SetAllToColor( Color {(color.r * i) / 255, (color.g * i) / 255, (color.b * i) / 255} );
    delay(3);
  }
  for (int i = 256 - 1; i >= 0; i--) {
    this->SetAllToColor( Color {(color.r * i) / 255, (color.g * i) / 255, (color.b * i) / 255} );
    delay(3);
  }
}

void LightsModule::DoubleFade(Color color1, Color color2) {
  for (int i = 0; i < 256; i++) {
    this->SetAllToColor( Color {(color1.r * i) / 255, (color1.g * i) / 255, (color1.b * i) / 255} );
    delay(5);
  }
  for (int i = 256 - 1; i >= 0; i--) {
    this->SetAllToColor( Color {(color1.r * i) / 255, (color1.g * i) / 255, (color1.b * i) / 255} );
    delay(5);
  }
}

#define RAINBOW_DELAY 2
#define RAINBOW_LOOPS 3

void LightsModule::Rainbow(int8_t reps = 1, bool rev = false) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes- through this loop:
  for(long firstPixelHue = 0; firstPixelHue < RAINBOW_LOOPS * 65536; firstPixelHue += 256 ) {
    // strip.rainbow() can take a single argument (first pixel hue) or
    // optionally a few extras: number of rainbow repetitions (default 1),
    // saturation and value (brightness) (both 0-255, similar to the
    // ColorHSV() function, default 255), and a true/false flag for whether
    // to apply gamma correction to provide 'truer' colors (default true).
    this->neopixel_strip_.rainbow(firstPixelHue, reps, 255, 255);
    // Above line is equivalent to:
    // strip.rainbow(firstPixelHue, 1, 255, 255, true);
    this->neopixel_strip_.show(); // Update strip with new contents
    delay(RAINBOW_DELAY);  // Pause for a moment
  }
}