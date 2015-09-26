/*--------FYRE 5480 2014-2015 Incendia Code--------*/

//Includes libraries with various functions, constants, variables, etc. that we use in the program
#include "WPILib.h"
#include "string"
#include <cmath>
#include <iostream>


class FYRERobot: public IterativeRobot
{
	
	RobotDrive *m_robotDrive;	
	Joystick *m_driveStick;        
	Talon *m_robotLift;
	Joystick *m_liftStick;
	Encoder *m_liftEncoder;
	DoubleSolenoid *m_solenoid;
	DigitalInput *autoSwitch;
	Compressor *compressorA;
	DigitalInput *compressorSwitch;

	float rightXboxY;
	bool rightBumper;
	bool leftBumper;
	bool m_topLimit;
	bool m_bottomLimit;
	float driveStickX;
	float driveStickY;
	float driveThrottle;
	bool solenoidValue;
	int level;
	int counter;
	bool XboxA;
	bool XboxB;
	bool XboxStart;
	float newLiftEncoder;
	bool workplease;

public:

	FYRERobot(void)
	{

		// Create a RobotDrive object using PWMS 1, 2, 3, and 4
		m_robotDrive = new RobotDrive(0, 1);
		// Define joystick being used at USB port #1 on the Drivers Station
		m_driveStick = new Joystick(0);
		// Twist is on Axis 3 for the Extreme 3D Pro
		m_driveStick->SetAxisChannel(Joystick::kTwistAxis, 3);
		// Create a RobotDrive object using PWMS 5
		m_robotLift = new Talon(2);
		// encoders and pnuematics
		m_liftEncoder = new Encoder(0, 1, true);

		m_solenoid = new DoubleSolenoid (0,1);
		// Define joystick being used at USB port #2 on the Drivers Station
		m_liftStick = new Joystick(1);
		// Define two swtiches
		autoSwitch = new DigitalInput(6);

		compressorA = new Compressor();

		compressorSwitch = new DigitalInput(7);



	}

private:
	LiveWindow *lw;

	void RobotInit()
	{
		CameraServer::GetInstance()->SetQuality(50);
		CameraServer::GetInstance()->StartAutomaticCapture("cam0");
		lw = LiveWindow::GetInstance();
		compressorA -> ClearAllPCMStickyFaults();
		compressorA -> Start();
		compressorA -> SetClosedLoopControl(false);
	}

	void AutonomousInit()
	{
		m_liftEncoder->Reset();
		counter = 0;

		AutonomousBin();

	}

	void AutonomousBin()
	{
		while(m_liftEncoder->Get()<1000){
			m_solenoid -> Set(m_solenoid-> kForward);
			m_robotLift -> Set(-.5);
			std::cout << m_liftEncoder->Get() << std::endl;

		}

		m_robotLift -> Set(0);
		m_solenoid -> Set(m_solenoid-> kReverse);

		toCenterOfAutoZoneTime();

	}

	void AutonomousToteTime(){

		while(m_liftEncoder->Get()<480){
			m_solenoid -> Set(m_solenoid-> kForward);
			m_robotLift -> Set(-.5);

		}

		m_robotLift -> Set(0);
		m_solenoid -> Set(m_solenoid-> kReverse);

		while(counter<2700)
		{
			m_robotDrive -> Drive(-.5,-.5);
			counter ++;
		}
		m_robotDrive -> Drive(0,0);
		counter = 0;

		toCenterOfAutoZoneTime();

	}

	 void toCenterOfAutoZoneTime(){
		while(counter < 5250){
			m_robotDrive -> Drive(-.5,0);
			counter++;
		}
			m_robotDrive -> Drive(0,0);
			counter = 0;

		while(m_liftEncoder -> Get()> 100){
			m_solenoid -> Set(m_solenoid-> kForward);
			m_robotLift -> Set(.5);
		}

		m_robotLift -> Set(0);
		m_solenoid -> Set(m_solenoid -> kReverse);
		while(counter < 800){
			m_robotDrive -> Drive(.5,0);
			counter++;
		}
		m_robotDrive -> Drive(0,0);
	}

	void AutonomousPeriodic()
	{
		
	}

	void TeleopInit()
	{

		m_liftEncoder->Reset();
		solenoidValue = false;
		level = 0;
		newLiftEncoder = 0;

	}

	void TeleopPeriodic()
	{

		// Update Variables
		rightXboxY = m_liftStick->GetRawAxis(5);
		rightBumper = m_liftStick->GetRawButton(6);
		leftBumper = m_liftStick -> GetRawButton(5);
		compressorSwitch -> Get();

		XboxA = m_liftStick -> GetRawButton(1);
		XboxB = m_liftStick -> GetRawButton(2);
		XboxStart = m_liftStick -> GetRawButton(8);

		setDriveTrain();

		if(XboxA == true){

			if(level < 3){
				if (level == 0){
					newLiftEncoder = m_liftEncoder->Get() + 300;
					m_solenoid -> Set(m_solenoid-> kForward);
					printf("%i\n", m_liftEncoder->Get());
					while((m_liftEncoder->Get()<= newLiftEncoder) && (XboxStart == 0)){
						m_robotLift -> Set(-1);
						printf("%i\n", m_liftEncoder->Get());
						setDriveTrain();
						XboxStart = m_liftStick -> GetRawButton(8);
						// = m_liftEncoder -> Get();
						updatePnuematics();
					}
				}
				else{
					//m_robotDrive -> Drive(0,0);
					newLiftEncoder = m_liftEncoder->Get() + 670;
					m_solenoid -> Set(m_solenoid-> kForward);
					printf("%i\n", m_liftEncoder->Get());
					while((m_liftEncoder->Get()<= newLiftEncoder) && (XboxStart == 0)){
						m_robotLift -> Set(-1);
						printf("%i\n", m_liftEncoder->Get());
						setDriveTrain();
						XboxStart = m_liftStick -> GetRawButton(8);
						// = m_liftEncoder -> Get();
						updatePnuematics();
					}
				}

				level = level + 1;
				m_solenoid -> Set(m_solenoid-> kReverse);
			}



		}

		else if(XboxB == true){
			if(level > 0){
				m_robotDrive -> Drive(0,0);
				newLiftEncoder = m_liftEncoder -> Get() + 100;
				while((m_liftEncoder->Get() <= newLiftEncoder) && (XboxStart == 0)){
					m_robotLift -> Set(-1);
					setDriveTrain();
					XboxStart = m_liftStick -> GetRawButton(8);
					updatePnuematics();
				}
				 // make via encoder count
				newLiftEncoder = m_liftEncoder->Get() - 200;
				m_solenoid -> Set(m_solenoid-> kForward);
				while((m_liftEncoder->Get()>= newLiftEncoder) && (XboxStart == 0)){

					m_robotLift -> Set(.5);
					printf("%i\n", m_liftEncoder->Get());
					setDriveTrain();
					XboxStart = m_liftStick -> GetRawButton(8);
					updatePnuematics();

				}
               level = level - 1;
               m_solenoid -> Set(m_solenoid-> kReverse);
			}

		}
		else{
			m_robotLift->Set(rightXboxY);
		}

		if (rightBumper == true){
			m_solenoid -> Set(m_solenoid-> kForward);
		}

		else if(leftBumper == true){
			m_solenoid -> Set(m_solenoid -> kReverse);


		}
		updatePnuematics();
	
		std::cout << m_liftEncoder->Get() << ": encoder" << std::endl;
		std::cout << compressorSwitch -> Get() << ": switch" << std::endl;
	}

	void updatePnuematics()
	{
		if (compressorSwitch -> Get() == false){
			compressorA -> Start();
		}
		else{
			compressorA -> Stop();
		}
	}

	void setDriveTrain()
	{
		driveStickX = (m_driveStick->GetX())*-1;
		driveStickY = m_driveStick->GetY();
		driveThrottle = (((((m_driveStick->GetThrottle())*-1)+1)/4)+.5);
		driveStickX = driveStickX * driveThrottle;
		driveStickY = driveStickY * driveThrottle;
		m_robotDrive->ArcadeDrive(driveStickY, driveStickX);

	}
	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(FYRERobot);
