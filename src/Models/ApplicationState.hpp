//
// Created by bill on 9/7/2024.
//

#ifndef APPLICATIONSTATE_HPP
#define APPLICATIONSTATE_HPP

typedef struct{
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
}  ApplicationState ;


// Overload the == operator for the struct
inline bool operator==(const ApplicationState& lhs, const ApplicationState& rhs) {
  return
      rhs.isDirty == lhs.isDirty &&
      rhs.isRocketDetected == rhs.isRocketDetected &&
      rhs.isLauncherVertical == rhs.isLauncherVertical  &&
      rhs.isNetworkConnected == rhs.isNetworkConnected  &&
      rhs.isPressurizeButtonPressed == rhs.isPressurizeButtonPressed  &&
      rhs.isLaunchButtonPressed == rhs.isLaunchButtonPressed  &&
      rhs.isSolendoidOpen == rhs.isSolendoidOpen  &&
      rhs.resevoirPressureVoltage == rhs.resevoirPressureVoltage  &&
      rhs.resevoirPressurePsi == rhs.resevoirPressurePsi  &&
      rhs.rocketPressureVoltage == rhs.rocketPressureVoltage  &&
      rhs.rocketPressurePsi == rhs.rocketPressurePsi;
}

// Overload the != operator for the struct
inline bool operator!=(const ApplicationState& lhs, const ApplicationState& rhs) {
  return !(lhs == rhs);
}



#endif //APPLICATIONSTATE_HPP
