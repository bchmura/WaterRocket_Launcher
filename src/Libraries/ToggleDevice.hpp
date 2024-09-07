//
// Created by bill on 9/6/2024.
//

#ifndef TOGGLEDEVICE_HPP
#define TOGGLEDEVICE_HPP
#include <Arduino.h>
#include <ArduinoLog.h>

enum State {
    OFF = 0,
    ON = 1
};

class ToggleDevice {
private:
    State currentState = OFF;
    int pinNumber = -1;

public:
    void setState(State newState);
    void setup(int pinNumber, State startingState);
    State getState();
    State toggle();
};

#endif //TOGGLEDEVICE_HPP
