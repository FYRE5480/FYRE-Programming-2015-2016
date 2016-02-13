#include "WPILib.h"

class Robot: public IterativeRobot
{
private:
	LiveWindow *lw = LiveWindow::GetInstance();
	AnalogInput *distanceSensor;

	void RobotInit()
	{
		distanceSensor = new AnalogInput(3);
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
		SmartDashboard::GetNumber("distance",distanceSensor->GetValue())
	}

	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot)
