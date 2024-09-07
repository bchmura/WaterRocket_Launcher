#ifndef CONFIG_HPP
#define CONFIG_HPP

constexpr unsigned int I2C_1_SDA = 21;
constexpr unsigned int I2C_1_SDL = 22;

constexpr unsigned int SCREEN_OLED_WIDTH = 128;
constexpr unsigned int SCREEN_OLED_HEIGHT = 64;
constexpr int SCREEN_OLED_RESET_PIN = -1; // Reset pin # (or -1 if sharing Arduino reset pin)
constexpr unsigned int SCREEN_OLED_ADDRESS  = 0x3C; ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

constexpr unsigned int BUTTON_TOGGLE_SOLENOID = 18;

constexpr unsigned int LED_DATA_PIN = 19;
constexpr unsigned int LED_TOTAL_LEDS = 12;

constexpr unsigned int LED_FOR_SOLENOID_OPEN = 1;
constexpr unsigned int SOLENOID_DATA_PIN = 25;

constexpr unsigned int LAUNCHER_VERTICAL_PIN = 23;
constexpr unsigned int LED_FOR_LAUNCHER_VERTICAL = 2;

constexpr unsigned int RES_PRESSURE_LED_START = 3;
constexpr unsigned int RES_PRESSURE_LED_COUNT = 4;
constexpr unsigned int RES_PRESSURE_LED_MAX_VALUE = 100;

constexpr unsigned int ROCKET_PRESSURE_LED_START = 7;
constexpr unsigned int ROCKET_PRESSURE_LED_COUNT = 4;
constexpr unsigned int ROCKET_PRESSURE_LED_MAX_VALUE = 100;

#endif //CONFIG_HPP
