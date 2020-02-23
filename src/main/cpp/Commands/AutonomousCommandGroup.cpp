/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/
#include "Commands/AutonomousCommandGroup.h"
#include "Commands/AutonomousMotionProfile.h"
#include "Automoves/To_PwrPort.h"

// NOTE:  Consider using this command inline, rather than writing a subclass.
// For more information, see:
// https://docs.wpilib.org/en/latest/docs/software/commandbased/convenience-features.html
AutonomousCommandGroup::AutonomousCommandGroup(DriveTrain* m_driveTrain) {
  // Add your commands here, e.g.
  // AddCommands(FooCommand(), BarCommand());
  AddCommands(AutonomousMotionProfile(m_driveTrain, &AutoMove_To_PwrPort_L, &AutoMove_To_PwrPort_R));
}