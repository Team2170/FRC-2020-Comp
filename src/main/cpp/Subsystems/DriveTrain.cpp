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

DriveTrain::DriveTrain() : 

    kWheelDiameter{frc::Preferences::GetInstance()->GetDouble("Wheel Diameter", 5.0)},
    kVoltageDrive{frc::Preferences::GetInstance()->GetBoolean("Voltage Driving", false)},
    kRampRate{frc::Preferences::GetInstance()->GetDouble("Voltage Ramp Time", 0.0)},
    kP{frc::Preferences::GetInstance()->GetDouble("Drive kP", 0.1)},
    kI{frc::Preferences::GetInstance()->GetDouble("Drive kI", 0.00)},
    kFF{frc::Preferences::GetInstance()->GetDouble("Drive kFF", 0.05)},
    kPvel{frc::Preferences::GetInstance()->GetDouble("Vel kP", 0.1)},
    kIvel{frc::Preferences::GetInstance()->GetDouble("Vel kI", 0.00)},
    kFFvel{frc::Preferences::GetInstance()->GetDouble("Vel kFF", 0.05)},
    kPpos{frc::Preferences::GetInstance()->GetDouble("Pos kP", 0.1)},
    kIpos{frc::Preferences::GetInstance()->GetDouble("Pos kI", 0.00)},
    kFFpos{frc::Preferences::GetInstance()->GetDouble("Pos kFF", 0.05)},
    maxFeetPerSec{frc::Preferences::GetInstance()->GetDouble("Drive FPS", 18.0)},
    maxAccelPerSec{frc::Preferences::GetInstance()->GetDouble("Drive Accel FPS^2", 10.0)} {
    
    compressor.reset(new frc::Compressor(0));
    powerDistributionPanel.reset(new frc::PowerDistributionPanel(0));

    // Set follow motors to leaders
    m_leftFollow.Follow(m_leftLead);
    m_rightFollow.Follow(m_rightLead);
    m_rightLead.SetInverted(true);

    m_leftLead.SetOpenLoopRampRate(kRampRate);
    m_rightLead.SetOpenLoopRampRate(kRampRate);

    m_leftLead.SetIdleMode(rev::CANSparkMax::IdleMode::kCoast);
    m_leftFollow.SetIdleMode(rev::CANSparkMax::IdleMode::kCoast);
    m_rightLead.SetIdleMode(rev::CANSparkMax::IdleMode::kCoast);
    m_rightFollow.SetIdleMode(rev::CANSparkMax::IdleMode::kCoast);

    // Set encoder converting factors Inches/Sec, Inches
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

    // Tuned for driver velocity mode
    m_pidControllerL.SetP(kP, GainSelect::kDriverVelocity);
    m_pidControllerR.SetP(kP, GainSelect::kDriverVelocity);
    m_pidControllerL.SetD(kD, GainSelect::kDriverVelocity);
    m_pidControllerR.SetD(kD, GainSelect::kDriverVelocity);
    m_pidControllerL.SetFF(kFF, GainSelect::kDriverVelocity);
    m_pidControllerR.SetFF(kFF, GainSelect::kDriverVelocity);  
    m_pidControllerL.SetI(kI, GainSelect::kDriverVelocity);
    m_pidControllerR.SetI(kI, GainSelect::kDriverVelocity);  

    // Tuned for autonomous velocity mode
    m_pidControllerL.SetP(kPvel, GainSelect::kAutoVelocity);
    m_pidControllerR.SetP(kPvel, GainSelect::kAutoVelocity);
    m_pidControllerL.SetD(kDvel, GainSelect::kAutoVelocity);
    m_pidControllerR.SetD(kDvel, GainSelect::kAutoVelocity);
    m_pidControllerL.SetFF(kFFvel, GainSelect::kAutoVelocity);
    m_pidControllerR.SetFF(kFFvel, GainSelect::kAutoVelocity);  
    m_pidControllerL.SetI(kIvel, GainSelect::kAutoVelocity);
    m_pidControllerR.SetI(kIvel, GainSelect::kAutoVelocity);  

    // Tuned for autonomous position mode
    m_pidControllerL.SetP(kPpos, GainSelect::kAutoPosition);
    m_pidControllerR.SetP(kPpos, GainSelect::kAutoPosition);
    m_pidControllerL.SetD(kDpos, GainSelect::kAutoPosition);
    m_pidControllerR.SetD(kDpos, GainSelect::kAutoPosition);
    m_pidControllerL.SetFF(kFFpos, GainSelect::kAutoPosition);
    m_pidControllerR.SetFF(kFFpos, GainSelect::kAutoPosition);  
    m_pidControllerL.SetI(kIpos, GainSelect::kAutoPosition);
    m_pidControllerR.SetI(kIpos, GainSelect::kAutoPosition);  

    m_leftEncoder.SetPosition(0.0);
    m_rightEncoder.SetPosition(0.0);

    m_Drive.SetExpiration(1.0);
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
                        units::foot_t(m_leftEncoder.GetPosition()),
                        units::foot_t(m_rightEncoder.GetPosition()));
    frc::SmartDashboard::PutNumber("Rotation", units::unit_cast<double>(m_odometry.GetPose().Rotation().Radians()));
    frc::SmartDashboard::PutNumber("Translation X", units::unit_cast<double>(m_odometry.GetPose().Translation().X()));
    frc::SmartDashboard::PutNumber("Translation Y", units::unit_cast<double>(m_odometry.GetPose().Translation().Y()));


    frc::SmartDashboard::PutNumber("Left Wheel Velocity", m_leftEncoder.GetVelocity());
    frc::SmartDashboard::PutNumber("Right Wheel Velocity", m_rightEncoder.GetVelocity());   

    frc::SmartDashboard::PutNumber("Left Wheel Position", m_leftEncoder.GetPosition());
    frc::SmartDashboard::PutNumber("Right Wheel Position", m_rightEncoder.GetPosition());  

    frc::SmartDashboard::PutNumber("Gyro Position", m_gyro.GetAngle());
}

void DriveTrain::ArcadeDrive(double xSpeed, double zRotation, bool squaredInputs)
{
    if (fabs(xSpeed) < 0.05) {
        xSpeed = 0.0;
    }
    if (fabs(zRotation) < 0.05) {
        zRotation = 0.0;
    }

    m_Drive.ArcadeDrive(zRotation, xSpeed, squaredInputs);
}
void DriveTrain::VelocityArcadeDrive(double xSpeed, double zRotation, bool squaredInputs)
{
    if (fabs(xSpeed) < 0.05) {
        xSpeed = 0.0;
    }

    if (fabs(zRotation) < 0.05) {
        zRotation = 0.0;
    }

    double moveValue = xSpeed;
    double rotateValue = zRotation;

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
    double rightMotorSpeed = rightMotorOutput * maxFeetPerSec;

    leftVelocityCommand = leftMotorSpeed;
    rightVelocityCommand = rightMotorSpeed;

    // Send setpoints to pid controllers
    m_pidControllerL.SetReference(leftMotorSpeed, rev::ControlType::kVelocity, GainSelect::kDriverVelocity);
    m_pidControllerR.SetReference(rightMotorSpeed, rev::ControlType::kVelocity, GainSelect::kDriverVelocity);
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

void DriveTrain::SetMaxOutput(double maxOutput) {
    m_Drive.SetMaxOutput(maxOutput);
}

/**
 * @brief Set wheel velocity of left and right wheels (FPS)
 * 
 * @param left wheel velocity in feet per second
 * @param right wheel velocity in feet per second
 */
void DriveTrain::SetWheelVelocity(double left, double right, int pidSlot) {
    m_pidControllerL.SetReference(left, rev::ControlType::kVelocity, pidSlot);
    m_pidControllerR.SetReference(right, rev::ControlType::kVelocity, pidSlot);
    leftVelocityCommand = left;
    rightVelocityCommand = right;
    m_Drive.FeedWatchdog();
}

/**
 * @brief Set wheel velocity of left and right wheels (FPS)
 * 
 * @param left wheel velocity in feet per second
 * @param right wheel velocity in feet per second
 */
void DriveTrain::SetWheelVelocity(units::meters_per_second_t left, units::meters_per_second_t right, int pidSlot) {
    units::feet_per_second_t leftFPS = left;
    units::feet_per_second_t rightFPS = right;
    SetWheelVelocity(units::unit_cast<double>(leftFPS), units::unit_cast<double>(rightFPS), pidSlot);
}

/**
 * @brief Set left and right to same velocity
 * 
 * @param velocity left and right velocity in feet per second
 */
void DriveTrain::SetWheelVelocity(double velocity, int pidSlot) {
    SetWheelVelocity(velocity, velocity, pidSlot);
    m_Drive.FeedWatchdog();
}

void DriveTrain::SetWheelPosition(double leftInches, double rightInches, int pidSlot) {
    m_pidControllerL.SetReference(leftInches, rev::ControlType::kPosition, pidSlot);
    m_pidControllerR.SetReference(rightInches, rev::ControlType::kPosition, pidSlot);
    m_Drive.FeedWatchdog();
}

void DriveTrain::SetWheelPosition(units::meter_t left, units::meter_t right, int pidSlot) {
    units::foot_t leftFt = left;
    units::foot_t rightFt = right;
    SetWheelPosition(units::unit_cast<double>(leftFt), units::unit_cast<double>(rightFt), pidSlot);
}