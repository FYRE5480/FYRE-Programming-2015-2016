// Recording code as of 16.02.23

#include "WPILib.h"
#include <iostream>
#include <fstream>
#include <istream>

class Robot: public IterativeRobot
{
private:
	LiveWindow *lw = LiveWindow::GetInstance();
	SendableChooser *chooser;
	const std::string autoNameDefault = "Default";
	const std::string autoNameCustom = "My Auto";
	std::string autoSelected;

	float xValues[800];
	float yValues[800];
	float zValues[800];

	Joystick *flightStick;
	Joystick *shootStick;
	RobotDrive *robotDrive;
	CANTalon *frontLeftTalon, *rearLeftTalon, *rearRightTalon;
	Victor *frontRightTalon;



	int frontLeftChannel	 = 55;	// Channel Declarations
	int rearLeftChannel	     = 1;
	int frontRightChannel	 = 0;
	int rearRightChannel	 = 2;

	float deadZone			 = .2;

	float flightX = 0;
	float flightY = 0;
	float flightZ = 0;
	float flightThrottle = 0;
	int count = 0;
	int counter = 0;
	int arraynum = 800;
	float arrayx = 0;
	int arraygetter =0;
	void RobotInit()
	{
		frontLeftTalon = new CANTalon(frontLeftChannel);
		rearLeftTalon = new CANTalon(rearLeftChannel);
		frontRightTalon = new Victor(frontRightChannel);
		rearRightTalon = new CANTalon(rearRightChannel);
		chooser = new SendableChooser();
		chooser->AddDefault(autoNameDefault, (void*)&autoNameDefault);
		chooser->AddObject(autoNameCustom, (void*)&autoNameCustom);
		SmartDashboard::PutData("Auto Modes", chooser);

		flightStick = new Joystick(1);
		shootStick = new Joystick(0);
		robotDrive = new RobotDrive(frontLeftTalon, rearLeftTalon, frontRightTalon, rearRightTalon);



	}


	/**
	 * This autonomous (along with the chooser code above) shows how to select between different autonomous modes
	 * using the dashboard. The sendable chooser code works with the Java SmartDashboard. If you prefer the LabVIEW
	 * Dashboard, remove all of the chooser code and uncomment the GetString line to get the auto name from the text box
	 * below the Gyro
	 *
	 * You can add additional auto modes by adding additional comparisons to the if-else structure below with additional strings.
	 * If using the SendableChooser make sure to add them to the chooser code above as well.
	 */
	void AutonomousInit()
	{
		autoSelected = *((std::string*)chooser->GetSelected());
		//std::string autoSelected = SmartDashboard::GetString("Auto Selector", autoNameDefault);
		if(arraygetter == 0){
			while(counter < arraynum){
				arrayx = xValues[counter];
				std::cout << arrayx << "," ;
				counter ++;
				robotDrive -> MecanumDrive_Cartesian(0,0,0);
			}
		}
		else if(arraygetter == 1){
			while(counter < arraynum){
				arrayx = yValues[counter];
				std::cout << arrayx << ",";
				counter ++;
				robotDrive -> MecanumDrive_Cartesian(0,0,0);
			}
		}
		else if(arraygetter == 2){
			while(counter < arraynum){
				arrayx = zValues[counter];
				std::cout << arrayx << "," ;
				robotDrive -> MecanumDrive_Cartesian(0,0,0);
				counter++;
			}
		}
		else{
			std::cout << "done" << std::endl;
			std::cout << "resetting" << std::endl;
			arraygetter = -1;
		}
		std::cout << std::endl;
		counter = 0;
		arraygetter++;
		robotDrive -> MecanumDrive_Cartesian(0,0,0);

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
		robotDrive -> MecanumDrive_Cartesian(0,0,0);
	}

	void TeleopInit()
	{

	}

	void TeleopPeriodic()
	{
		flightX = flightStick->GetRawAxis(0); //Pull joystick side motion for later use
		flightY = flightStick->GetRawAxis(1); //Pull joystick forward motion for later use (forward is -, backwards is +)
		flightZ = flightStick->GetRawAxis(4); //Pull joystick twist motion for later use
		flightThrottle = ((((shootStick->GetThrottle() - 1)*-1)/2) * .8 + .2); //Pull throttle to modify drive variables
																		//Throttle value is between .2 and 1.0

		if (fabs(flightX) < deadZone) { //Deaden x
			flightX = 0;
		}
		if (fabs(flightY) < deadZone) { //Deaden y
			flightY = 0;
		}
		if (fabs(flightZ) < deadZone) { //Deaden z
			flightZ = 0;
		}
		flightX = flightX * flightThrottle;
		flightY = flightY * flightThrottle;
		flightZ = flightZ * flightThrottle;

		if(count < 800){
			xValues[count] = flightX;
			yValues[count] = flightY;
			zValues[count] = flightZ;
			std::cout << count << std::endl;
			count++;
		} else {
		}


		robotDrive->MecanumDrive_Cartesian(flightZ, flightY, flightX);



	}

	void TestPeriodic()
	{
		lw->Run();
	}

	void DisabledInit(){

	}
};

START_ROBOT_CLASS(Robot)



