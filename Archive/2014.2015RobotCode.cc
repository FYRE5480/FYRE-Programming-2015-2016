/*--------FYRE 5480 2014-2015 Incendia Code--------*/

//Includes libraries with various functions, constants, variables, etc. that we use in the program
#include "WPILib.h"
#include "string"
#include <cmath>
#include <iostream>

//Declares FYRERobot as a Robot that uses the Iterative Command Structure (See "Choosing a Base Class" in WPIlib)
class FYRERobot: public IterativeRobot
{
	//Declares addresses for variables w/ numerical values; Doesn't declare variable
	RobotDrive *m_robotDrive;	
	Joystick *m_driveStick;        
	Talon *m_robotLift;
	Joystick *m_liftStick;
	Encoder *m_liftEncoder;
	DoubleSolenoid *m_solenoid;
	DigitalInput *autoSwitch;
	Compressor *compressorA;
	DigitalInput *compressorSwitch;

	//Declares variables changed by code, not robot; Doesn't assign variables
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

//Contains functions for declaring robot parts
public:
	//Function defining robot
	FYRERobot(void)
	{

		// Create a RobotDrive object using PWMS 0 & 1
		m_robotDrive = new RobotDrive(0, 1);
		// Define joystick being used at USB port #0 on the Drivers Station
		m_driveStick = new Joystick(0);
		//Sets joystick axis channel for Twist to channel 3; Possibly unused in current program
		m_driveStick->SetAxisChannel(Joystick::kTwistAxis, 3);
		// Create a RobotLift object using PWMS 2 (Motor Controllor for Lift Mechanism)
		m_robotLift = new Talon(2);
		// encoders and pnuematics
		m_liftEncoder = new Encoder(0, 1, true);
		m_solenoid = new DoubleSolenoid (0,1);
		compressorA = new Compressor();
		compressorSwitch = new DigitalInput(7);
		// Define joystick being used at USB port #1 on the Drivers Station; XBox 360 controller
		m_liftStick = new Joystick(1);
		// Define switch
		autoSwitch = new DigitalInput(6);

	}

//Contains functions for running robot
private:
	//Creates a debugging window for testing mechanics of robot; doesn't (currently) work
	LiveWindow *lw;

	//Function defining robot boot-up
	void RobotInit()
	{
		//Camera settings
		CameraServer::GetInstance()->SetQuality(50);
		CameraServer::GetInstance()->StartAutomaticCapture("cam0");
		//Livewindow again (update)
		lw = LiveWindow::GetInstance();
		//Starts Compressor; ASK MATT
		compressorA -> ClearAllPCMStickyFaults();
		compressorA -> Start();
		compressorA -> SetClosedLoopControl(false);
	}
	//Function for autonomous portion
	void AutonomousInit()
	{
		//Reset Lift Encoder and time counter
		m_liftEncoder->Reset();
		counter = 0;

		AutonomousBin();

	}
	// function for auto-bin
	void AutonomousBin()
	{
		while(m_liftEncoder->Get()<1000){
			//Solenoid is flipped; Forward = Reverse, <1 = >1
			m_solenoid -> Set(m_solenoid-> kForward);
			m_robotLift -> Set(-.5);
			//Displays lift encoder for debugging purposes
			std::cout << m_liftEncoder->Get() << std::endl;

		}
		
		// Stops and locks lift
		m_robotLift -> Set(0);
		m_solenoid -> Set(m_solenoid-> kReverse);

		toCenterOfAutoZoneTime();

	}

	//function for auto-zone move
	 void toCenterOfAutoZoneTime(){
	 	//time counter
		while(counter < 5250){
			m_robotDrive -> Drive(-.5,0);
			counter++;
		}
			//Stops robot, resets time counter
			m_robotDrive -> Drive(0,0);
			counter = 0;

		//Lowering Lift
		while(m_liftEncoder -> Get()> 100){
			m_solenoid -> Set(m_solenoid-> kForward);
			m_robotLift -> Set(.5);
		}

		//Stop and lock lift
		m_robotLift -> Set(0);
		m_solenoid -> Set(m_solenoid -> kReverse);
		
		//Drive backwards
		while(counter < 800){
			m_robotDrive -> Drive(.5,0);
			counter++;
		}
		m_robotDrive -> Drive(0,0);
	}

	//This is literally dumb; ASK MATT
	void AutonomousPeriodic()
	{
		
	}

	//function initializing Teleop
	void TeleopInit()
	{

		//reset variables to 0
		m_liftEncoder->Reset();
		solenoidValue = false;
		level = 0;
		newLiftEncoder = 0;

	}

	//Iterative function for teleop; runs over and over and over and over and over and over and over and over and over and over and over and you really think I'm going to do this forever? You're wrong. It's only 12:33:30:19. I have better things to do tonight.
	void TeleopPeriodic()
	{

		// Set XBox + Controller variables; look up online which raw number matches which axis/button
		rightXboxY = m_liftStick->GetRawAxis(5);
		rightBumper = m_liftStick->GetRawButton(6);
		leftBumper = m_liftStick -> GetRawButton(5);
		XboxA = m_liftStick -> GetRawButton(1);
		XboxB = m_liftStick -> GetRawButton(2);
		XboxStart = m_liftStick -> GetRawButton(8);
		
		//Compressor switch broken; ASK MATT
		compressorSwitch -> Get();

		setDriveTrain();

		//If button A pressed
		if(XboxA == true){
			
			//levels of locking bars for lifting mechanism
			if(level < 3){

				//lifting mech. at bottom; TODOOOOOOOOO
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
					newLiftEncoder = m_liftEncoder->Get() + 670;
					m_solenoid -> Set(m_solenoid-> kForward);
					while((m_liftEncoder->Get()<= newLiftEncoder) && (XboxStart == 0)){
						m_robotLift -> Set(-1);
						setDriveTrain();
						XboxStart = m_liftStick -> GetRawButton(8);
						updatePnuematics();
					}
				}

				level = level + 1;
				m_solenoid -> Set(m_solenoid-> kReverse);
			}



		}
		// Same as A, but down.
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
			//Defer to joystick
			m_robotLift->Set(rightXboxY);
		}
		//Solenoid lock/unlock
		if (rightBumper == true){
			m_solenoid -> Set(m_solenoid-> kForward);
		}

		else if(leftBumper == true){
			m_solenoid -> Set(m_solenoid -> kReverse);


		}
		updatePnuematics();
		
		//Bug Testing; ASK MATT
		std::cout << m_liftEncoder->Get() << ": encoder" << std::endl;
		std::cout << compressorSwitch -> Get() << ": switch" << std::endl;
	}
	//Function... you can read; Compressor Switch still broken... surprise.
	void updatePnuematics()
	{
		if (compressorSwitch -> Get() == false){
			compressorA -> Start();
		}
		else{
			compressorA -> Stop();
		}
	}

	//function to update drive train controls (The stick of Jobs*)
	void setDriveTrain()
	{
		driveStickX = (m_driveStick->GetX())*-1;
		driveStickY = m_driveStick->GetY();
		driveThrottle = (((((m_driveStick->GetThrottle())*-1)+1)/4)+.5);
		driveStickX = driveStickX * driveThrottle;
		driveStickY = driveStickY * driveThrottle;
		m_robotDrive->ArcadeDrive(driveStickY, driveStickX);

	}
	//Run to see live window that is actually dead. RIP
	void TestPeriodic()
	{
		lw->Run();
	}
};
//*Joys

START_ROBOT_CLASS(FYRERobot);
