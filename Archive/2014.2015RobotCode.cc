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
	bool XboxSelect;
	int printing;
	bool selectPressed;

//Contains functions for declaring robot parts
public:
	//Function defining robot
	FYRERobot(void)
	{

		// Create a RobotDrive object using PWMs 0 & 1
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
		//Starts Compressor: First function clears faults which stop compressor from running, second starts it, third gives us control rather than Control Module (necessary due to broken Control Module)
		compressorA -> ClearAllPCMStickyFaults();
		compressorA -> Start();
		compressorA -> SetClosedLoopControl(false);
	}
	//Function for autonomous portion
	void AutonomousInit()
	{
		counter = 0;
		while(counter<777){
			m_RobotDrive->Drive(-0.2, 0);
			counter++;
		}
		
		m_RobotDrive->Drive(0, 0);	
	}

	//Periodic Function for Autonomous: Runs continuously in autonomous
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
		printing = 0;

	}

	//Iterative function for teleop; runs over and over and over and over and over and over and over and over and over and over and over and you really think I'm going to do this forever? You're wrong. It's only 12:33:30:19. I have better things to do tonight. ASK CONNOR
	void TeleopPeriodic()
	{

		// Set XBox + Controller variables; look up online which raw number matches which axis/button
		rightXboxY = m_liftStick->GetRawAxis(5);
		rightBumper = m_liftStick->GetRawButton(6);
		leftBumper = m_liftStick -> GetRawButton(5);
		XboxA = m_liftStick -> GetRawButton(1);
		XboxB = m_liftStick -> GetRawButton(2);
		XboxSelect = m_liftStick -> GetRawButton(7);
		XboxStart = m_liftStick -> GetRawButton(8);
		
		//Pnematics Control Module Broken: Tells us manually what value of compressor switch is
		compressorSwitch -> Get();

		setDriveTrain();

		//If button A pressed
		if(XboxA == true){
			//Print to show button is pressed
			std::cout << "Xbox Button A has been Pressed" << std::endl;
			//levels of locking bars for lifting mechanism
			if(level < 3){

				//lifting mech. at bottom
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
			std::cout << "Xbox Button B has been Pressed" << std::endl;
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
		if(printing == 0){
			std::cout << m_liftEncoder->Get() << ": encoder (Press SELECT to Change Modes)" << std::endl;
		}
		else if(printing == 1){
			std::cout << driveStickX -> Get() << ": Drive Stick X (Press SELECT to Change Modes)" << std::endl;
		}
		else if(printing == 2){
			std::cout << driveStickY -> Get() << ": Drive Stick Y (Press SELECT to Change Modes)" << std::endl;	
		}
		else{
			printing = 0
		}
		
		if(XboxSelect == true){
			if(selectPressed == 0){
				printing += 1;
				selectPressed = 1;
			}
		}
		else{
			selectPressed = 0;
		}
		
	
		
		
	}
	//Function to Update Pnuematics (Normally done by PCM [Pnuematics Control Module] but ours broke so this was to work around that)
	void updatePnuematics()
	{
		if (compressorSwitch -> Get() == false){
			compressorA -> Start();
		}
		else{
			compressorA -> Stop();
		}
	}

	//Function to update Joystick which controlled the wheels of the robot
	void setDriveTrain()
	{
		driveStickX = (m_driveStick->GetX())*-1;
		driveStickY = m_driveStick->GetY();
		driveThrottle = (((((m_driveStick->GetThrottle())*-1)+1)/4)+.5);
		driveStickX = driveStickX * driveThrottle;
		driveStickY = driveStickY * driveThrottle;
		m_robotDrive->ArcadeDrive(driveStickY, driveStickX);

	}
	//Meant to run the live window, but ours doesn't work
	void TestPeriodic()
	{
		lw->Run();
	}
};

//Runs Code Above
START_ROBOT_CLASS(FYRERobot);
