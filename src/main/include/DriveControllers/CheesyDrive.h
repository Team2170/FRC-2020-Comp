/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once
#include "DriveControllers/DriveController.h"

class CheesyDrive: public DriveController {
 public:
  CheesyDrive();
  std::pair<double, double> GetLeftRightMotorCommands(double throttle, double rotation) override;
  
};
