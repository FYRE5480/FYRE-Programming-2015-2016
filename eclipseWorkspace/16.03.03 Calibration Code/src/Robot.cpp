//Calibration code for distance sensor and camera

#include "WPILib.h"

class Robot: public IterativeRobot
{
private:
	// Smart dashboard setup
	LiveWindow *lw = LiveWindow::GetInstance();
	SendableChooser *chooser;
	const std::string camFront = "Front";
	const std::string camBack = "Back";
	std::string camSelected;

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

	Joystick *shootStick;
	float shooterY;

	void RobotInit()
	{
		chooser = new SendableChooser();
		chooser->AddDefault(camFront, (void*)&camDefault);
		chooser->AddObject(camBack, (void*)&camCustom);
		SmartDashboard::PutData("Which camera are you testing?", chooser);

//		Distance sensor declarations
		distanceSensor = new AnalogInput(3);
		getVcc = new AnalogInput(2);

		frontCamServo = new Servo(10); // WHAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAT
		backCamServo = new Servo(11); // WHAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAT

		shootStick = new Joystick(0);
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
//		SmartDashboard::Number("Front angle:", frontAngle); // if you're using the line below comment this out
//		// frontAngle = WHATEVERNUMBER; // if you want to hard-code the servo instead because the smart dashboard's being dumb
//		frontCamServo -> SetAngle(frontAngle);
//		SmartDashboard::GetNumber("Back angle:", backAngle); // if you're using the line below comment this out
//		// backAngle = WHATEVERNUMBER; // if you want to hard-code the servo instead because the smart dashboard's being dumb
//		backCamServo -> SetAngle(backAngle);

		// Get joystick Y axis
		shooterY = shootStick->GetY();
		// print joystick axis
		frontAngle += shooterY; // Scaling is most likely needed. I don't know what the servo angle goes to/from, or what the shooter goes to/from
		backAngle += shooterY;

		if(camSelected == camBack) {
			backCamServo -> SetAngle(backAngle);
			// GET BACK CAMERA IMAGE (or maybe just switch a var? who knows. will have to see how Matt implemented 2 cams)
		}
		else { // front is default
			frontCamServo -> SetAngle(frontAngle);
			// Change a thing. Or display image. Whatevvver.
		}

		// Get image however you're getting it. GOTTA ADD THIS CODE

		SmartDashboard::PutNumber("Front angle:", frontAngle);
		SmartDashboard::PutNumber("Back angle:", backAngle);
	}

	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot)



