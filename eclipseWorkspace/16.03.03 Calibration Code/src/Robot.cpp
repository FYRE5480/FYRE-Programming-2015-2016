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
		// Get image + show image
	}

	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot)


