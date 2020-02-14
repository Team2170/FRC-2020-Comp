/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "RobotContainer.h"
#include "frc2/command/button/JoystickButton.h"

RobotContainer::RobotContainer() {
  m_driveTrain.SetDefaultCommand(TeleopDrive(&m_driveTrain));

  // Initialize all of your commands and subsystems here

  // Configure the button bindings
  ConfigureButtonBindings();
}

void RobotContainer::ConfigureButtonBindings() {
  // Configure your button bindings here6
  frc2::JoystickButton(&m_operator,1).WhileHeld(new IntakeOn(&m_intake));
  frc2::JoystickButton(&m_operator,2).WhileHeld(new IntakeReverse(&m_intake));
  frc2::JoystickButton(&m_operator,3).WhileHeld(new SpinStorage(&m_feeder));
  frc2::JoystickButton(&m_operator,4).WhileHeld(new SpinStorageCCW(&m_feeder));

  frc2::JoystickButton(&m_driver, 3).WhileHeld(new VisionDrive(&m_vision, &m_driveTrain));
}

frc2::Command* RobotContainer::GetAutonomousCommand() {
  // An example command will be run in autonomous
  return &m_autonomousCommand;
}

void RobotContainer::StartDataLogging() {
  m_vision.EnableLogging();
}

void RobotContainer::EndDataLogging() {
  m_vision.DisableLogging();
}