// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.

#include "Subsystems/Vision.h"

Vision::Vision() {
    auto table = nt::NetworkTableInstance::GetDefault().GetTable("limelight");
    tx = table->GetEntry("tx");
	ty = table->GetEntry("ty");
    tv = table->GetEntry("tv");
    tl = table->GetEntry("tl");
    getPipe = table->GetEntry("getpipe");
	ledMode = table->GetEntry("ledMode");
	camMode = table->GetEntry("camMode");
	setPipe = table->GetEntry("pipeline");
	snapshot = table->GetEntry("snapshot");
    nt_distance = table->GetEntry("distance");
    nt_distance.SetDouble(0);
    nt_visionDrive = table->GetEntry("Vision Drive");
    nt_visionDrive.SetBoolean(false);

    nt_kP_Omega = table->GetEntry("Vision kP Omega");
    nt_kI_Omega = table->GetEntry("Vision kI Omega");
    nt_kP_Distance = table->GetEntry("Vision kP Distance");
    nt_kP_Omega.SetDouble(kP_Omega);
    nt_kI_Omega.SetDouble(kI_Omega);
    nt_kP_Distance.SetDouble(kP_Distance);

    visionLogger.VisionLogger("/home/lvuser/VisionLogs/VisionLog_" + DataLogger::GetTimestamp() + ".csv");
}

void Vision::Periodic() {
    bool targetLocked = TargetIsLocked();
    if (!targetLocked) {
        visionDriveActive = false;
        distance = 0;
        distanceError = 0;
        distanceError_DB = 0;
        angleError = 0;
        angleError_DB = 0;
        speed = 0;
        omega = 0;        
    }
    else if (!visionDriveActive) {
        frc::SmartDashboard::PutNumber("LED Code",LEDCodes::VLock);
        distance = GetDistanceToPowerport();
        nt_distance.SetDouble(distance);
        optimalShootingDistance = frc::Preferences::GetInstance()->GetDouble("Optimal Shooting Distance", optimalShootingDistance);
        distanceError =  optimalShootingDistance - distance;
        distanceError_DB = distanceError;
        angleError = GetXAngleToTarget();
        angleError_DB = angleError;
        speed = 0;
        omega = 0;        
    }
    visionLogger.WriteVisionData(targetLocked, visionDriveActive, distance,
                                 distanceError, distanceError_DB, angleError, angleError_DB, speed, omega);
}

/**
 * @brief Checks if a target is locked. 
 * 
 * @return true when target locked
 * @return false when target not found
 */
bool Vision::TargetIsLocked() {
    return (tv.GetDouble(0) == 1);
}

/**
 * @brief Returns tx from limelight network tables
 * Should only be called if a target is locked
 * 
 * @return x-angle from target in degrees
 */
double Vision::GetXAngleToTarget() {
   return tx.GetDouble(0);
}

/**
 * @brief Calculates distance from the front bumper of robot to the powerport in ft
 * Should only be called when a powerport target is locked
 *
 * @return distance from powerport 
 */
double Vision::GetDistanceToPowerport() {
    double angleToTarget = (cameraAngle + ty.GetDouble(0)) * Deg2Rad;
    double camToPowerPortDistance  = (powerportVisionTargetHeight - cameraHeight) / tan(angleToTarget);
    return camToPowerPortDistance - cameraDistanceFromFrontBumper;
}

/**
 * @brief gets combined latency of the pipeline and image capture in ms
 * 
 * @return combined latency of the pipeline and image capture in milliseconds
 */
double Vision::GetLatency() {
    return tl.GetDouble(0) + 11;
}

/**
 * @brief Sets the LED Mode of the camera
 * 
 * @param ledModeToSet LEDMode enum value
 *
 */
void Vision::SetLEDMode(LEDMode ledModeToSet) {
    int ledModeValue = ledModeToSet;
    ledMode.SetDouble((double)ledModeValue);
}

/**
 * @brief Switches between vision processing and drive mode
 * 
 * @param visionProcessingEnabled true enables vision processing, false stops vision processing and increases exposure
 */
void Vision::SetCamMode(bool visionProcessingEnabled) {
    if (visionProcessingEnabled)
    {
        camMode.SetDouble(0);
    }
    else
    {
        camMode.SetDouble(1);
    }
}

/**
 * @brief Toggles between vision processing and drive mode
 */
void Vision::ToggleCamMode() {
    if (camMode.GetDouble(1))
    {
        camMode.SetDouble(0);
    }
    else
    {
        camMode.SetDouble(1);
    }
}

/**
 * @brief Sets the current vision processing pipeline
 * 
 * @param pipeline the pipeline to use
 */
void Vision::SetPipeline(Pipeline pipeline) {
    int pipelineIndex = pipeline;
    setPipe.SetDouble((double)pipelineIndex);
}

/**
 * @brief Saves a snapshot to the limelight.
 */
 void Vision::TakeSnapshot() {
     snapshot.SetDouble(1);
 }

/**
 * @brief prepares for vision steer
 */
void Vision::VisionSteerInit() {
    visionDriveActive = true;
    nt_visionDrive.SetBoolean(true);
    omegaIntegrator = 0.0;
}

/**
 * @brief calcuates speed and omega to steer to face the target and be the optimal distance away
 * 
 * @returns a pair of speed and omega to be passed to VelocityArcadeDrive method in drivetrain
 */
std::pair<double, double> Vision::SteerToLockedTarget() {
 
    frc::SmartDashboard::PutNumber("LED Code",LEDCodes::VDrive);

    // calulate distance error
    optimalShootingDistance = frc::Preferences::GetInstance()->GetDouble("Optimal Shooting Distance", optimalShootingDistance);
    distance = GetDistanceToPowerport();
    distanceError = optimalShootingDistance - distance;
    nt_distance.SetDouble(distance);

    // get angle error
    angleError = GetXAngleToTarget();
    angleError_DB = angleError;

    // deadband angle error
    if (angleError < angleErrorDeadband && angleError > -angleErrorDeadband)
    {
        angleError_DB = 0;
    }
    else if (angleError > angleErrorDeadband)
    {
        angleError_DB = angleError - angleErrorDeadband;
    }
    else
    {
        angleError_DB = angleError + angleErrorDeadband;
    }

    // fetch gains from network tables
    kP_Omega = nt_kP_Omega.GetDouble(kP_Omega);
    kI_Omega = nt_kI_Omega.GetDouble(kI_Omega);
    kP_Distance = nt_kP_Distance.GetDouble(kP_Distance);
    omega = 0.0;
    speed = 0.0;

    // omega PID calculations
    omegaIntegrator += angleError_DB * deltaTime;
    omega = kP_Omega * angleError_DB;
    omega += omegaIntegrator * kI_Omega;

    // limit omega
    if (omega > omegaLimiter)
    {
        omega = omegaLimiter;
    }
    else if (omega < -omegaLimiter)
    {
        omega = -omegaLimiter;
    }

    if (angleError >  -5 && angleError < 5)
    {

        // deadband angle error
        if (distanceError < distanceErrorDeadband && distanceError > -distanceErrorDeadband)
        {
            distanceError_DB = 0;
        }
        else if (angleError > angleErrorDeadband)
        {
            distanceError_DB = distanceError - distanceErrorDeadband;
        }
        else
        {
            distanceError_DB = distanceError + distanceErrorDeadband;
        }


        // speed PID calculations
        speed = kP_Distance * distanceError_DB;

        // limit speed
        if (speed > speedLimiter)
        {
            speed = speedLimiter;
        }
        else if (speed < -speedLimiter)
        {
            speed = -speedLimiter;
        }

    }

 
    return std::make_pair(speed, omega);
}

/**
 * @brief ends the vision steer
 */
void Vision::VisionSteerEnd() {
    visionDriveActive = false;
    nt_visionDrive.SetBoolean(false);
    visionDriveActive = false;
}

/**
 * @brief starts or resumes vision data logging
 */
void Vision::EnableLogging() {
    visionLogger.StartSession();
}

/**
 * @brief ends or pauses vision data logging
 */
void Vision::DisableLogging() {
    visionLogger.EndSession();
}