#pragma once

enum CANIDs {
	kRightLeadID = 1,
	kRightFollowID = 2,
	kLeftFollowID = 3,
	kLeftLeadID = 4,
	kShooterMotorID = 5,
	kIntakeMotorID = 6,
	kFeederMotorID = 7,
	kLoaderMotorID = 8,
	kShooterMotorID2 = 9,
	kClimberMotorID = 10
};

enum PCMIDs {
  kHoodSolFwd = 0,
  kHoodSolRev = 1,
	kIntakeSolFwd = 2,
	kIntakeSolRev = 3,
	kClimberSol = 4
};

enum PWMIDs {

};

enum DIOIDs {

};

enum HIDIDs {
	kDriver = 0,
	kOperator = 1
};

enum LEDCodes 
{
	Off = 0,
	Shooter_SpoolUp = 1,
	Shooter_AtSpeed = 2,
	Shoooter_Shooting = 3,
	VLock = 4,
	VDrive = 5,
	IntakeBalls = 6
};
