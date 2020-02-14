// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.

#pragma once

#include "frc2/command/SubsystemBase.h"
#include "networktables/NetworkTable.h"
#include "networktables/NetworkTableEntry.h"
#include "networktables/NetworkTableInstance.h"
#include "networktables/EntryListenerFlags.h"
#include "frc/Preferences.h"

#include "DataLogger.h"

#include <math.h>
#include <cmath>
#include <tuple>

#define  Deg2Rad 3.14159/180

/**
 *
 *
 * @author ExampleAuthor
 */
class Vision: public frc2::SubsystemBase {
private:
	// It's desirable that everything possible is private except
	// for methods that implement subsystem capabilities
	
	// network table entries for limelight communication 
	nt::NetworkTableEntry tx;
	nt::NetworkTableEntry ty;
	nt::NetworkTableEntry tv;
	nt::NetworkTableEntry tl;
	nt::NetworkTableEntry getPipe;
	nt::NetworkTableEntry ledMode;
	nt::NetworkTableEntry camMode;
	nt::NetworkTableEntry setPipe;
	nt::NetworkTableEntry snapshot;

	// network table entries for vision drive
	nt::NetworkTableEntry nt_visionDrive;
	nt::NetworkTableEntry nt_distance;
	nt::NetworkTableEntry nt_kP_Omega;
	nt::NetworkTableEntry nt_kI_Omega;
	nt::NetworkTableEntry nt_kP_Distance;
	
	// for distance calculation
	const double powerportVisionTargetHeight = (6 + 9.25/12) + (1 + 5.0/12)/2; // height of the center of the vision target (ft)
	const double cameraHeight = 21.0/12; // height that the camera is mounted at (ft)
	const double cameraAngle = 35; // angle camera is mounted at from horizontal (degrees)
	const double cameraDistanceFromFrontBumper = 15.0/12; // ft
	double optimalShootingDistance = 10; // optimal distance from powerport to shoot from (ft)

	// For VisionDrive PID controller
	double angleErrorDeadband = 2;	// degrees
	double kP_Omega = -0.025; 
	double kI_Omega = -0.02;
	double kP_Distance = 0.0;
	double omegaLimiter = 0.5;
	double omegaIntegrator = 0;
	double speedLimiter = 0.5;
	const double deltaTime = 0.02;

	// data logging
	DataLogger visionLogger;
	bool visionDriveActive = false;
	double distance = 0;	
	double distanceError = 0;
	double angleError = 0;
	double angleError_DB = 0;
	double speed = 0;
	double omega = 0;

public:
  Vision();
    enum Pipeline {powerport = 0};
	enum LEDMode {currentPipelineMode = 0, forceOff = 1, forceBlink = 2, forceOn = 3};

	void Periodic() override;

	bool TargetIsLocked();
	double GetXAngleToTarget();
	double GetDistanceToPowerport();
	double GetLatency();
	void SetLEDMode(LEDMode ledModeToSet);
	void SetCamMode(bool visionProcessingEnabled);
	void ToggleCamMode();
	void SetPipeline(Pipeline pipeline);
	void TakeSnapshot();
	void VisionSteerInit();
	std::pair<double, double> SteerToLockedTarget(void);
	void VisionSteerEnd();
	void EnableLogging();
	void DisableLogging();
};
