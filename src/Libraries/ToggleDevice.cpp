//
// Created by bill on 9/6/2024.
//
#include "ToggleDevice.hpp"


void ToggleDevice::setup(int _pinNumber, ToggleState startingState) {
  Log.infoln("Device set for pin %d and state %d", _pinNumber, startingState);
  pinNumber = _pinNumber;
  pinMode(pinNumber, OUTPUT);
  currentState = startingState;
  Log.infoln("Currentstate is %d", currentState);
}



void ToggleDevice::setState(ToggleState newState)
{
  Log.infoln("device set state requested: %d on pin %d", newState, pinNumber);
  if (newState == currentState) return;
  digitalWrite(pinNumber, newState);
  currentState = newState;
  Log.infoln("device set to: %d", currentState);
};

ToggleState ToggleDevice::getState()
{
  return currentState;
};

ToggleState ToggleDevice::toggle() {
  Log.infoln("device toggle requested from current state of %d", currentState);

  if (currentState == ON) {
    setState(OFF);
  } else if (currentState == OFF) {
    setState(ON);
  } else {
    setState(ON);
  }

  return currentState;
};

