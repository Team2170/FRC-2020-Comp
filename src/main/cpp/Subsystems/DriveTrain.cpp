// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


#include "Subsystems/DriveTrain.h"
#include "Commands/TeleopDrive.h"

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES
// END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTANTS
// END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTANTS

DriveTrain::DriveTrain() : 

    kWheelDiameter(frc::Preferences::GetInstance()->GetDouble("Wheel Diameter", 6.0)),
    kP(frc::Preferences::GetInstance()->GetDouble("Drive kP", 0.1)),
    kFF(frc::Preferences::GetInstance()->GetDouble("Drive kFF", 0.05)),
    maxFeetPerSec(frc::Preferences::GetInstance()->GetDouble("Drive FPS", 18.0)),
    maxAccelPerSec(frc::Preferences::GetInstance()->GetDouble("Drive Accel FPS^2", 10.0)) {
    


    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
compressor.reset(new frc::Compressor(0));

powerDistributionPanel.reset(new frc::PowerDistributionPanel(0));

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS

    // Set follow motors to leaders
    m_leftFollow.Follow(m_leftLead);
    m_rightFollow.Follow(m_rightLead);

    // Set encoder converting factors 
    m_leftEncoder.SetVelocityConversionFactor((M_PI * kWheelDiameter / (kGearRatio * 60.0 * 12.0)));
    m_rightEncoder.SetVelocityConversionFactor((M_PI * kWheelDiameter / (kGearRatio * 60.0 * 12.0)));
    m_leftEncoder.SetPositionConversionFactor((M_PI * kWheelDiameter) / (kGearRatio * 12.0));
    m_rightEncoder.SetPositionConversionFactor((M_PI * kWheelDiameter) / (kGearRatio * 12.0));

    // Set min/max power
    m_pidControllerL.SetOutputRange(kMinOutput, kMaxOutput);
    m_pidControllerR.SetOutputRange(kMinOutput, kMaxOutput);
    m_pidControllerL.SetSmartMotionMaxVelocity(maxFeetPerSec);
    m_pidControllerR.SetSmartMotionMaxVelocity(maxFeetPerSec);
    m_pidControllerL.SetSmartMotionMaxAccel(maxAccelPerSec);
    m_pidControllerR.SetSmartMotionMaxAccel(maxAccelPerSec);
    
    // Set kFF and kP ( after tuning, since gains have been already been determined )
    m_pidControllerL.SetP(kP);
    m_pidControllerR.SetP(kP);

    m_pidControllerL.SetFF(kFF);
    m_pidControllerR.SetFF(kFF);    

    m_leftLead.SetClosedLoopRampRate(100);
    m_rightLead.SetClosedLoopRampRate(100);
    m_leftFollow.SetClosedLoopRampRate(100);
    m_rightFollow.SetClosedLoopRampRate(100);
    //SetDefaultCommand(new TeleopDrive());
    
    driveTrainLogger.DriveTrainLogger("/home/lvuser/DriveTrainLogs/DriveTrainLog_" + DataLogger::GetTimestamp() + ".csv");
}

void DriveTrain::InitDefaultCommand() {
    // Set the default command for a subsystem here.
    // SetDefaultCommand(new MySpecialCommand());
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DEFAULT_COMMAND

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DEFAULT_COMMAND
   // SetDefaultCommand(new TeleopDrive());
}

void DriveTrain::Periodic() {
    // Put code here to be run every loop
    m_odometry.Update(frc::Rotation2d(units::degree_t(GetHeading())),
                        units::meter_t(m_leftEncoder.GetPosition()),
                        units::meter_t(m_rightEncoder.GetPosition()));

    double leftVelocity = m_leftEncoder.GetVelocity(); 
    double rightVelocity = m_rightEncoder.GetVelocity(); 
    double leftLeadAppliedOutput = m_leftLead.GetAppliedOutput();
    double leftFollowAppliedOutput = m_leftFollow.GetAppliedOutput();
    double rightLeadAppliedOutput = m_rightLead.GetAppliedOutput();
    double rightFollowAppliedOutput = m_rightFollow.GetAppliedOutput();

    double leftLeadVoltage = m_leftLead.GetBusVoltage();
    double leftFollowVoltage = m_leftFollow.GetBusVoltage();
    double rightLeadVoltage = m_rightLead.GetBusVoltage();
    double rightFollowVoltage = m_rightFollow.GetBusVoltage();
    double leftLeadCurrent = m_leftLead.GetOutputCurrent();
    double leftFollowCurrent = m_leftFollow.GetOutputCurrent();
    double rightLeadCurrent = m_rightLead.GetOutputCurrent();
    double rightFollowCurrent = m_rightFollow.GetOutputCurrent();
    driveTrainLogger.WriteDriveTrainData(leftVelocityCommand, rightVelocityCommand, leftVelocity, rightVelocity, leftLeadAppliedOutput, leftFollowAppliedOutput, rightLeadAppliedOutput, rightFollowAppliedOutput, leftLeadVoltage, leftFollowVoltage, rightLeadVoltage, rightFollowVoltage, leftLeadCurrent, leftFollowCurrent, rightLeadCurrent, rightFollowCurrent);

}

void DriveTrain::VelocityArcadeDrive(double xSpeed, double zRotation, bool squaredInputs)
{
    double moveValue = xSpeed;
    double rotateValue = zRotation;

    if(moveValue > 0.0 && moveValue < 0.05){
        moveValue = 0.0;
    } else if(moveValue < 0.0 && moveValue > -0.05){
        moveValue = 0.0;
    } 

    if (fabs(rotateValue) < 0.01) {
        rotateValue = 0.0;
    }

    static bool reported = false;
    if (!reported)
    {
        HAL_Report(HALUsageReporting::kResourceType_RobotDrive, 4,
                   HALUsageReporting::kRobotDrive_ArcadeStandard);
        reported = true;
    }

    // local variables to hold the computed PWM values for the motors
    double leftMotorOutput;
    double rightMotorOutput;

    // LeftMove and leftRotate limits to +-1.0
    if (moveValue > 1.0)
    {
        moveValue = 1.0;
    }
    if (moveValue < -1.0)
    {
        moveValue = -1.0;
    }
    if (rotateValue > 1.0)
    {
        rotateValue = 1.0;
    }
    if (rotateValue < -1.0)
    {
        rotateValue = -1.0;
    }

    // square the inputs (while preserving the sign) to increase fine control
    // while permitting full power
    if (squaredInputs)
    {
        moveValue = std::copysign(moveValue * moveValue, moveValue);
        rotateValue = std::copysign(rotateValue * rotateValue, rotateValue);
    }

    if (moveValue > 0.0)
    {
        if (rotateValue > 0.0)
        {
            leftMotorOutput = moveValue - rotateValue;
            rightMotorOutput = std::max(moveValue, rotateValue);
        }
        else
        {
            leftMotorOutput = std::max(moveValue, -rotateValue);
            rightMotorOutput = moveValue + rotateValue;
        }
    }
    else
    {
        if (rotateValue > 0.0)
        {
            leftMotorOutput = -std::max(-moveValue, rotateValue);
            rightMotorOutput = moveValue + rotateValue;
        }
        else
        {
            leftMotorOutput = moveValue - rotateValue;
            rightMotorOutput = -std::max(-moveValue, -rotateValue);
        }
    }

    double leftMotorSpeed = leftMotorOutput * maxFeetPerSec;
    double rightMotorSpeed = rightMotorOutput * -maxFeetPerSec;

    // Send proper LED code to LED strip 
    // i.e make sure forward LED code and vision lock code are not sent at the same time
    sendProperLEDCode(leftMotorSpeed, rightMotorSpeed, rotateValue);

    // Send setpoints to pid controllers
    m_pidControllerL.SetReference(leftMotorSpeed, rev::ControlType::kSmartVelocity);
    m_pidControllerR.SetReference(rightMotorSpeed, rev::ControlType::kSmartVelocity);
    m_Drive.FeedWatchdog();

}

/**
 * @brief Set voltage for left and right wheel motors (open loop)
 * 
 * @param leftVoltage voltage command for left wheel
 * @param rightVoltage voltage command for right wheel
 */
void DriveTrain::SetWheelVolts(double leftVoltage, double rightVoltage) {
    m_pidControllerL.SetReference(leftVoltage, rev::ControlType::kVoltage);
    m_pidControllerR.SetReference(rightVoltage, rev::ControlType::kVoltage);
    m_Drive.FeedWatchdog();
}

void DriveTrain::SetWheelVolts(double voltage) {
    SetWheelVolts(voltage, voltage);
}

void DriveTrain::ResetEncoders() {
    m_leftEncoder.SetPosition(0.0);
    m_rightEncoder.SetPosition(0.0);
}

void DriveTrain::ResetOdometry(frc::Pose2d pose) {
    ResetEncoders();
    m_odometry.ResetPosition(pose,
                frc::Rotation2d(units::degree_t(GetHeading())));
}
void DriveTrain::SetMaxOutput(double maxOutput) {
    m_Drive.SetMaxOutput(maxOutput);
}

double DriveTrain::GetHeading() {
    return std::remainder(m_gyro.GetAngle(), 360) * (kGyroReverse ? -1.0 : 1.0);
}

double DriveTrain::GetTurnRate() {
    return m_gyro.GetRate() * (kGyroReverse ? -1.0 : 1.0);
}

frc::Pose2d DriveTrain::GetPose() {return m_odometry.GetPose();}

frc::DifferentialDriveWheelSpeeds DriveTrain::GetWheelSpeeds(){
    return {units::feet_per_second_t(m_leftEncoder.GetVelocity(),
    units::feet_per_second_t(m_rightEncoder.GetVelocity()))};
}

double DriveTrain::GetAverageEncoderDistance( ) {
    return(m_leftEncoder.GetPosition() + m_rightEncoder.GetPosition()) /2.0;
}

/**
 * @brief Set wheel velocity of left and right wheels (FPS)
 * 
 * @param left wheel velocity in feet per second
 * @param right wheel velocity in feet per second
 */
void DriveTrain::SetWheelVelocity(double left, double right) {
    m_pidControllerL.SetReference(left, rev::ControlType::kVelocity);
    m_pidControllerR.SetReference(right, rev::ControlType::kVelocity);
    leftVelocityCommand = left;
    rightVelocityCommand = right;
    m_Drive.FeedWatchdog();
}

/**
 * @brief Set left and right to same velocity
 * 
 * @param velocity left and right velocity in feet per second
 */
void DriveTrain::SetWheelVelocity(double velocity) {
    SetWheelVelocity(velocity, velocity);
}

/**
 * @brief starts or resumes vision data logging
 */
void DriveTrain::EnableLogging() {
    driveTrainLogger.StartSession();
}

/**
 * @brief ends or pauses vision data logging
 */
void DriveTrain::DisableLogging() {
    driveTrainLogger.EndSession();
}

/**
 * @brief Send correct LED code to Arduino
 */
void DriveTrain::sendProperLEDCode(double leftSpeed, double rightSpeed, double rotateValue) {
    bool turning = rotateValue != 0.0;
    auto table = nt::NetworkTableInstance::GetDefault().GetTable("limelight");
    bool visionDriveActive = table->GetEntry("Vision Drive").GetBoolean(false);
    bool targetLocked = table->GetEntry("tv").GetDouble(0) == 1;

    if (!turning && !visionDriveActive && !targetLocked && leftSpeed > 0.0 && rightSpeed > 0.0) {
        // Driving Forward
        frc::SmartDashboard::PutNumber("LED Code", LEDCodes::Fwd);
    }
    if (!turning && !visionDriveActive && !targetLocked && leftSpeed < 0.0 && rightSpeed < 0.0) {
        // Driving Backward
        frc::SmartDashboard::PutNumber("LED Code", LEDCodes::Bwd);
    }
}