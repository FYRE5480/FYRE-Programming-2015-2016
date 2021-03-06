#include "WPILib.h"
float convertedDist;
float scalingFactor;

class Robot: public IterativeRobot
{
private:
	LiveWindow *lw = LiveWindow::GetInstance();
	AnalogInput *distanceSensor;
	AnalogInput *getVcc;

	void RobotInit()
	{
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
		ReadDistance();
	}

	void ReadDistance() {
//		SmartDashboard::PutNumber("the raw distance from the distance sensor is", distanceSensor->GetValue());
//		SmartDashboard::PutNumber("the voltage is", distanceSensor->GetAverageVoltage());
//		SmartDashboard::PutNumber("the vcc is", getVcc->GetAverageVoltage());
//		scalingFactor = getVcc->GetAverageVoltage()/float(512);
//		SmartDashboard::PutNumber("using scaling factor", scalingFactor * distanceSensor->GetValue());
//		convertedDist = float(512) * distanceSensor->GetValue() / getVcc->GetAverageVoltage();
//		SmartDashboard::PutNumber("the converted distance is", convertedDist/1000);

		// y = 0.1319x-1.3456
		// y is actual distance, x is raw value
		Smart Dashboard::PutNumber("Actual distance:", 0.1319(distanceSensor->GetValue())-1.3456);
	}

	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot)
