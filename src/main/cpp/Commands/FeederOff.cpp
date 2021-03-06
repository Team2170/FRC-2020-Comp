/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "Commands/FeederOff.h"

FeederOff::FeederOff(Feeder* subsystem): m_feeder{subsystem} {
  // Use addRequirements() here to declare subsystem dependencies.
  AddRequirements({subsystem});
}

// Called when the command is initially scheduled.
void FeederOff::Initialize() {}

// Called repeatedly when this Command is scheduled to run
void FeederOff::Execute() {
  m_feeder->FeedStop();
}

// Called once the command ends or is interrupted.
void FeederOff::End(bool interrupted) {}

// Returns true when the command should end.
bool FeederOff::IsFinished() { return true; }
