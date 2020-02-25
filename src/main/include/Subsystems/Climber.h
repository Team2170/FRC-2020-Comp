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
#include "hwcfg.h"
#include "rev/CANSparkMax.h"
#include "frc/Solenoid.h"
#include "ctre/Phoenix.h"

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES
// END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES

#define WINCH_MOTOR_ID 7
#define DEFAULT_CLIMB_VOLTS 0.5
/**
 *
 *
 * @author ExampleAuthor
 */
class Climber: public frc2::SubsystemBase {
private:
	// It's desirable that everything possible is private except
	// for methods that implement subsystem capabilities
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
	// END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
	frc::Solenoid m_climberSol{PCMIDs::kClimberSol};
	WPI_TalonSRX m_climberMotor{CANIDs::kClimberMotorID};

	
	

public:
Climber();
	void Periodic() override;
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CMDPIDGETTERS
	// END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CMDPIDGETTERS
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTANTS
	// END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTANTS
	void ClimberRelease(){m_climberSol.Set(true);}		// extend climber up (to grab bar)
	void WinchExtend(double speed = DEFAULT_CLIMB_VOLTS){m_climberMotor.Set(speed);}	// extend winch (to let out slack ONLY in the pit)
	void WinchRetract(double speed = DEFAULT_CLIMB_VOLTS){m_climberMotor.Set(-speed);}	// retract winch (to pull up/climb)
	void WinchStop(void){m_climberMotor.Set(0.0);}		// stop winch

};

