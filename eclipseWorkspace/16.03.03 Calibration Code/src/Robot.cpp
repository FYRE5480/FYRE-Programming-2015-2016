//Calibration code for distance sensor and camera

#include "WPILib.h"

class Robot: public IterativeRobot
{
private:
	// Smart dashboard setup
	LiveWindow *lw = LiveWindow::GetInstance();
	SendableChooser *chooser;
	const std::string autoNameDefault = "Default";
	const std::string autoNameCustom = "My Auto";
	std::string autoSelected;

	// Camera setup
	IMAQdxSession session;
	Image *frame;
	IMAQdxError imaqError;

//	Distance sensor
	AnalogInput *distanceSensor;
	AnalogInput *getVcc;

//	Distance variable
	float convertedDist = 0;

//  Servo
	Servo *frontCamServo;
	Servo *backCamServo;
	float frontAngle;
	float backAngle;

	void RobotInit()
	{

//		Distance sensor declarations
		distanceSensor = new AnalogInput(3);
		getVcc = new AnalogInput(2);

	}

	void AutonomousInit()
	{

	}

	void AutonomousPeriodic()
	{

	}

	void TeleopInit()
	{

	}

	void TeleopPeriodic()
	{
		DistanceSensor();
		CameraAngle();
	}

	void DistanceSensor() {
		SmartDashboard::PutNumber("Distance input:", distanceSensor->GetValue());
	}

	void CameraAngle() {
		SmartDashboard::GetNumber("Front angle:", frontAngle); // if you're using the line below comment this out
		// frontAngle = WHATEVERNUMBER; // if you want to hard-code the servo instead because the smart dashboard's being dumb
		frontCamServo -> SetAngle(frontAngle);
		SmartDashboard::GetNumber("Back angle:", backAngle); // if you're using the line below comment this out
		// backAngle = WHATEVERNUMBER; // if you want to hard-code the servo instead because the smart dashboard's being dumb
		backCamServo -> SetAngle(backAngle);

		// Get image however you're getting it.
	}

	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot)



