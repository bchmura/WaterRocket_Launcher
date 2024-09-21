//
// Created by bill on 9/6/2024.
//

#ifndef TOGGLEDEVICE_HPP
#define TOGGLEDEVICE_HPP
#include <Arduino.h>
#include <ArduinoLog.h>

enum ToggleState {
    OFF = 0,
    ON = 1
};

class ToggleDevice {
private:
    ToggleState currentState = OFF;
    int pinNumber = -1;

public:
    void setState(ToggleState newState);
    void setup(int pinNumber, ToggleState startingState);
    ToggleState getState();
    ToggleState toggle();
};

#endif //TOGGLEDEVICE_HPP
