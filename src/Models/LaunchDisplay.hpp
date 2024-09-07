#ifndef BRL_DISPLAY_HPP
#define BRL_DISPLAY_HPP
#include "Models/ApplicationState.hpp"

static const unsigned char image_EviWaiting1_bits[] = {0x30,0x03,0x00,0x60,0x01,0x80,0xe0,0x01,0xc0,0xf3,0xf3,0xc0,0xff,0xff,0xc0,0xff,0xff,0xc0,0x7f,0xff,0x80,0x7f,0xff,0x80,0x7f,0xff,0x80,0xff,0xff,0xc0,0xed,0xf5,0xc0,0xfd,0xfd,0xc0,0xf9,0xf9,0xc0,0xe3,0xe3,0xc0,0xff,0xff,0xc0,0x7f,0xff,0x80,0x7f,0xff,0x80,0x3f,0xff,0x00,0x1f,0xfe,0x00,0x0f,0xfc,0x00,0x03,0xf0,0x00};
static const unsigned char image_EviWaiting2_bits[] = {0x30,0x03,0x00,0x60,0x01,0x80,0xe0,0x01,0xc0,0xf3,0xf3,0xc0,0xff,0xff,0xc0,0xff,0xff,0xc0,0x7f,0xff,0x80,0x7f,0xff,0x80,0x7f,0xff,0x80,0xff,0xff,0xc0,0xeb,0xed,0xc0,0xef,0xef,0xc0,0xef,0xef,0xc0,0xf1,0xf1,0xc0,0xff,0xff,0xc0,0x7f,0xff,0x80,0x7f,0xff,0x80,0x3f,0xff,0x00,0x1f,0xfe,0x00,0x0f,0xfc,0x00,0x03,0xf0,0x00};
static const unsigned char image_Ble_connected_bits[] = {0x07,0xc0,0x1f,0xf0,0x3e,0xf8,0x7e,0x7c,0x76,0xbc,0xfa,0xde,0xfc,0xbe,0xfe,0x7e,0xfc,0xbe,0xfa,0xde,0x76,0xbc,0x7e,0x7c,0x3e,0xf8,0x1f,0xf0,0x07,0xc0};
static const unsigned char image_Ble_disconnected_bits[] = {0x07,0xc0,0x18,0x30,0x21,0x08,0x41,0x84,0x49,0x44,0x85,0x22,0x83,0x42,0x81,0x82,0x83,0x42,0x85,0x22,0x49,0x40,0x41,0x84,0x21,0x08,0x18,0x30,0x07,0xc0};
static const unsigned char image_ArrowUpFilled_bits[] = {0x03,0x00,0x04,0x80,0x0b,0x40,0x17,0xa0,0x2f,0xd0,0x5f,0xe8,0x86,0x84,0xf5,0xbc,0x16,0xa0,0x15,0xa0,0x16,0xa0,0x15,0xa0,0x17,0xa0,0x10,0x20,0x1f,0xe0};
static const unsigned char image_Pressed_Button_bits[] = {0x0f,0x80,0x3f,0xe0,0x7f,0xf0,0x7f,0xf0,0xff,0xf8,0xff,0xf8,0xff,0xf8,0xff,0xf8,0xff,0xf8,0x7f,0xf0,0x7f,0xf0,0x3f,0xe0,0x0f,0x80};
static const unsigned char image_ArrowUpEmpty_bits[] = {0x03,0x00,0x05,0x00,0x09,0x00,0x11,0xfe,0x20,0x02,0x40,0x02,0x80,0x02,0x80,0x02,0x40,0x02,0x20,0x02,0x11,0xfe,0x09,0x00,0x05,0x00,0x03,0x00};
  
enum DisplayState {
    Initializing,
    Running,
    Error
};

class LaunchDisplay
{
    private:
    public:
        DisplayState currentState = Initializing;

        LaunchDisplay();
        void Setup();
        void SetupLights();
        void SetupOled();
        void modeInitialization();
        void modeRunTime();
        void modeErrorScreen();
        void RunUpdate();
        bool RunUpdate(ApplicationState newAppState);
        int ConvertDigitalValueToPressure(int current_voltage, int max_voltage, int max_pressure);
};

#endif