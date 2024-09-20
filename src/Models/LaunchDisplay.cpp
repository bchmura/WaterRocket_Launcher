#include <Arduino.h>
#include <ArduinoLog.h>
#include <Adafruit_SSD1306.h>
#include <FastLED.h>
#include <Wire.h>
#include <config.hpp>
#include <Models/LaunchDisplay.hpp>

#include "ApplicationState.hpp"

CRGB leds[LED_TOTAL_LEDS];
Adafruit_SSD1306 display(SCREEN_OLED_WIDTH, SCREEN_OLED_HEIGHT, &Wire, SCREEN_OLED_RESET_PIN);
DisplayState displayState;
ApplicationState lastAppState;

LaunchDisplay::LaunchDisplay() {

}

void LaunchDisplay::Setup() {
    displayState = Initializing;
    SetupOled();
    SetupLights();
    RunUpdate(lastAppState);
    RunUpdate(lastAppState);
    displayState = Running;
    Log.noticeln("Display setup complete");
}

void LaunchDisplay::SetupOled() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_OLED_ADDRESS)) {
    Log.errorln("SSD1306 allocation failed");
    for(;;); // Don't proceed, loop forever @TODO: Fix this
  }
  Log.notice("Setup OLED completed");
}


void LaunchDisplay::SetupLights() {
    FastLED.addLeds<WS2812B, LED_DATA_PIN, GRB>(leds, LED_TOTAL_LEDS);
    for(int i = 0; i < LED_TOTAL_LEDS; i++) {
      leds[i] = CRGB::Red;
      FastLED.show();
      delay(50);
    }

  delay(10000);



    for(int i = LED_TOTAL_LEDS - 1; i > -1; i--) {
      leds[i] = CRGB::Green;
      FastLED.show(); 
      delay(50);
    }
    FastLED.clear();
    FastLED.show();

    Log.noticeln("setupLights compelte");
}

bool LaunchDisplay::RunUpdate(ApplicationState newAppState) {

  if (newAppState.isDirty) {
    Log.infoln("RunUpdate has a new app status to process");
    lastAppState = newAppState;
  }

  Log.verboseln("Current display state is %d", displayState);

  switch (displayState) {
    case Initializing:
      modeInitialization();
    break;
    case Running:
      modeRunTime();
    break;
    case Error:
      modeErrorScreen();
    break;
  }

  return lastAppState.isDirty;

}

void LaunchDisplay::modeErrorScreen(void) {
    display.clearDisplay();
    display.setTextWrap(false);
    display.setCursor(2, 4);
    display.print("Error");
    display.display();

};

void LaunchDisplay::modeInitialization(void) {
    if (! lastAppState.isDirty) return;

    display.clearDisplay();
    display.drawBitmap(106, 36, image_EviWaiting1_bits, 18, 21, 1);
    display.setTextWrap(false);
    display.setCursor(2, 2);
    display.print("Title_Here");
    display.setCursor(2, 27);
    display.print("Line2");
    display.setCursor(2, 47);
    display.print("Line4");
    display.setCursor(2, 37);
    display.print("Line3");
    display.setCursor(2, 17);
    display.print("Line1");
    display.display();

    lastAppState.isDirty = false;
}


void LaunchDisplay::modeRunTime(void) {
    if (! lastAppState.isDirty) return;

    int cx;
    char line[20] = "...";
    display.clearDisplay();
    display.setTextWrap(false);
    display.setTextColor(WHITE);
    display.setCursor(2, 4);
    display.print("Running...");
    
    display.setCursor(2, 18);display.print("Res:");
    display.setCursor(2, 28);display.print("Tube:");
    display.setCursor(2, 38);display.print("Sol:");
    display.setCursor(2, 49);display.print("Botl:");
    
    cx = snprintf(line, 20, "%d/%d",lastAppState.resevoirPressurePsi, lastAppState.resevoirPressureVoltage);
    display.setCursor(33, 18);
    display.print(line);
    
    cx = snprintf(line, 20, "%d/%d",lastAppState.rocketPressurePsi, lastAppState.rocketPressureVoltage);
    display.setCursor(33, 29);
    display.print(line);
 
    display.setCursor(33, 39);
    if (lastAppState.isSolendoidOpen) {
      display.print("open");
    } else { 
      display.print("closed");
    }
    
    display.setCursor(33, 49);
    if (lastAppState.isRocketDetected) {
      display.print("detected");
    } else {
      display.print("no rocket");
    }
    
    if (lastAppState.isNetworkConnected) {
      display.drawBitmap(110, 44, image_Ble_connected_bits, 15, 15, 1);
    } else {
      display.drawBitmap(110, 44, image_Ble_disconnected_bits, 15, 15, 1);
    }
    
    if (lastAppState.isLauncherVertical) {
      display.drawBitmap(110, 24, image_ArrowUpFilled_bits, 14, 15, 1);
    } else {
      display.drawBitmap(110, 24, image_ArrowUpEmpty_bits, 15, 14, 1);
    }
    
    if (lastAppState.isSolendoidOpen) {
      display.drawBitmap(110, 1, image_Pressed_Button_bits, 13, 13, 1);  
    }    
    
    display.display();

  if (lastAppState.isSolendoidOpen) {
    leds[LED_FOR_SOLENOID_OPEN] = CRGB::Green;
  } else {
    leds[LED_FOR_SOLENOID_OPEN] = CRGB::Red;
  }

  if (lastAppState.isLauncherVertical) {
    leds[LED_FOR_LAUNCHER_VERTICAL] = CRGB::Green;
  } else {
    leds[LED_FOR_LAUNCHER_VERTICAL] = CRGB::Red;
  }

  SetLedGauge(lastAppState.resevoirPressurePsi, RES_PRESSURE_LED_MAX_VALUE, RES_PRESSURE_LED_START, RES_PRESSURE_LED_COUNT);

  SetLedGauge(lastAppState.rocketPressurePsi, ROCKET_PRESSURE_LED_MAX_VALUE, ROCKET_PRESSURE_LED_START, ROCKET_PRESSURE_LED_COUNT);

  FastLED.show();

  lastAppState.isDirty = false;
}


int LaunchDisplay::SetLedGauge(int current_pressure_psi, unsigned int pressure_max_value, unsigned int led_start, unsigned int led_count) {

  double levelSize = pressure_max_value / led_count;

  int numberOfLedsToLight = (int)(current_pressure_psi / levelSize + 0.5);

  Log.infoln("Turning on %d out of %d LEDs due to pressure at %d out of %d (Level is %D)", numberOfLedsToLight, led_count, current_pressure_psi, pressure_max_value, levelSize);

  for (int i = led_start; i < led_start + led_count; i++) {
    if (i < numberOfLedsToLight + led_start) {
      leds[i] = CRGB::HotPink;
    } else {
      leds[i] = CRGB::Black;
    }
  }

  return numberOfLedsToLight;

}


int LaunchDisplay::ConvertPressureToLedIndicators(int res_pressure_psi, unsigned int res_pressure_led_max_value, unsigned int res_pressure_led_count) {
  double levelSize = res_pressure_led_max_value / res_pressure_led_count;
  return (int)(res_pressure_psi / levelSize + 0.5);
}
