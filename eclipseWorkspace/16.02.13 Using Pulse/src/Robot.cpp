#include "WPILib.h"

class Robot: public IterativeRobot
{
private:
	LiveWindow *lw = LiveWindow::GetInstance();
	SendableChooser *chooser;
	const std::string autoNameDefault = "Default";
	const std::string autoNameCustom = "My Auto";
	std::string autoSelected;
	DigitalOutput *triggerPin;
	DigitalInput *echoPin;
	int distTimer = 0;
	int maximumRange = 200; // Maximum range needed
	int minimumRange = 0; // Minimum range needed
	long duration = 0, distance = -1; // Duration used to calculate distance
	long val;

	void RobotInit()
	{
		chooser = new SendableChooser();
		chooser->AddDefault(autoNameDefault, (void*)&autoNameDefault);
		chooser->AddObject(autoNameCustom, (void*)&autoNameCustom);
		SmartDashboard::PutData("Auto Modes", chooser);
		triggerPin = new DigitalOutput(8);
		echoPin = new DigitalInput(9);
	}

	void AutonomousInit()
	{
		autoSelected = *((std::string*)chooser->GetSelected());
		//std::string autoSelected = SmartDashboard::GetString("Auto Selector", autoNameDefault);
		std::cout << "Auto selected: " << autoSelected << std::endl;

		if(autoSelected == autoNameCustom){
			//Custom Auto goes here
		} else {
			//Default Auto goes here
		}
	}

	void AutonomousPeriodic()
	{
		if(autoSelected == autoNameCustom){
			//Custom Auto goes here
		} else {
			//Default Auto goes here
		}
	}

	void TeleopInit()
	{
		SmartDashboard::PutNumber("In TeleopInit:", 1);
		Wait(5);
		SmartDashboard::PutNumber("Fine, whatever:", 1);
	}

	void TeleopPeriodic()
	{
		ReadDistance();

	}

	void ReadDistance() {
////		digitalWrite(trigPin, LOW); Arduino code.
//		triggerPin->Set(0);

		// Code to pulse. We are using pulse instead.
//		distTimer->Reset(); // delay of 2 microseconds
//		while(distTimer->HasPeriodPassed(0.000002) == 0) {
//			;
//		}
//
////		digitalWrite(trigPin, HIGH);
//		triggerPin->Set(1);
//
//		distTimer->Reset(); // delay of 10 microseconds
//		while(distTimer->HasPeriodPassed(0.000010) == 0) {
//			;
//		}

		triggerPin->Pulse(0.000010);

////		digitalWrite(trigPin, LOW); Arduino code.
//		triggerPin->Set(0);

//		duration = pulseIn(echoPin, HIGH); Arduino code.
		duration = 0;
		distTimer = 0;
		while(echoPin->Get() == 0 && distTimer<1000) {
//			duration = distTimer->Get();
			distTimer++;
			duration++;
		}

		//Calculate the distance (in cm) based on the speed of sound.
		distance = duration/58.2;

		if (distance >= maximumRange || distance <= minimumRange){
		/* Send a negative number to computer to indicate "out of range" */
		val = -1;
		}
		else {
		/* Send the distance to the computer to indicate successful reading. */
		val = distance;
		}
		SmartDashboard::PutNumber("Distance is:", val);
	}

	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot)
