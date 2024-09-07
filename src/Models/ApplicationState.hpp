//
// Created by bill on 9/7/2024.
//

#ifndef APPLICATIONSTATE_HPP
#define APPLICATIONSTATE_HPP

struct ApplicationState {
  bool isDirty = true;
  bool isRocketDetected = false;
  bool isLauncherVertical = false;
  bool isNetworkConnected = false;
  bool isPressurizeButtonPressed = false;
  bool isLaunchButtonPressed = false;
  bool isSolendoidOpen = false;
  int resevoirPressureVoltage = 0;
  int resevoirPressurePsi = 0;
  int rocketPressureVoltage = 0;
  int rocketPressurePsi = 0;
};

#endif //APPLICATIONSTATE_HPP
