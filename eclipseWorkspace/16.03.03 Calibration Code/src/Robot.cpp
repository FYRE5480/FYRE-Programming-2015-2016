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

//	Joystick *shootStick;
	float shooterY;

	// YO SO WE STILL WANT TO SHOOT THIS ROBOT:
	// Shooting motor controllers and joystick and limit switch
	Jaguar *motor2, *motor3;
	Talon *motor1; //Will be Talon: Victor for testing purposes only
	Joystick *shootStick;
	DigitalInput *shooterSwitch;

	// Various drivy things and gyro and switch
	CANTalon *frontLeftTalon, *rearLeftTalon, *rearRightTalon;
	Victor *frontRightTalon;
	RobotDrive *robotDrive;	// robot drive system
	Joystick *flightStick;	// Flightstick
	ADXRS450_Gyro *yawGyro;

	int frontLeftChannel	 = 55;	// Channel Declarations
		int rearLeftChannel	     = 1;
		int frontRightChannel	 = 7;
		int rearRightChannel	 = 2;

		int flightstickChannel	 = 1;
		int xboxChannel		     = 0;

		int strafeButtonChannel  = 1;
		int arcadeButtonChannel  = 2;
		int fieldButtonChannel	 = 3;

		int gyroResetChannel 	 = 8;

		float deadZone			 = .2;

		//	Drive variables
		int driveMode = 0; // Default Drive Mode is Strafe
		float flightX = 0;
		float flightY = 0;
		float flightZ = 0;
		float flightThrottle = 0;

	//Shooter variables
		bool bPrime = false;
		bool bPrimeOn = false;
		int bPrimeRunning = 1;
		bool bLoad = false;
		bool bLoadOn = false;
		int bLoadRunning = 1;
		bool bHighGoalShoot = false;
		float bHighGoal = 0;
		bool bLowGoalShoot = false;
		float bLowGoal = 0;
		int shooterCounter = 0;
		bool shootSwitch = 0;
		int servoLocation = 0;

	void RobotInit()
	{
		chooser = new SendableChooser();
		chooser->AddDefault(camFront, (void*)&camFront);
		chooser->AddObject(camBack, (void*)&camBack);
		SmartDashboard::PutData("Which camera are you testing?", chooser);

//		Distance sensor declarations
		distanceSensor = new AnalogInput(3);
		getVcc = new AnalogInput(2);

		frontCamServo = new Servo(10); // WHAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAT
		backCamServo = new Servo(11); // WHAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAT

		shootStick = new Joystick(0);

			// WE LIKE TO DRIVE, DON'T WE, STEVEN
	//		Drive declarations
			frontLeftTalon = new CANTalon(frontLeftChannel);
			rearLeftTalon = new CANTalon(rearLeftChannel);
			frontRightTalon = new Victor(frontRightChannel);
			rearRightTalon = new CANTalon(rearRightChannel);
			//frontLeftTalon->SetInverted(true);
			rearLeftTalon->SetInverted(true);
			yawGyro = new ADXRS450_Gyro();
			robotDrive = new RobotDrive(frontLeftTalon, rearLeftTalon, frontRightTalon, rearRightTalon);
			flightStick = new Joystick(flightstickChannel);
			robotDrive -> SetSafetyEnabled(false);

	//		Shooter variable declarations MAKE SURE YOU PLUG THEM INTO THE RIGHT PORTS WENDY
			motor1 = new Talon(8); // loook above
			motor2 = new Jaguar(4);
			motor3 = new Jaguar(5);
			shootStick = new Joystick(0);
			shooterSwitch = new DigitalInput(0);
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

//	PLEASE, STEVEN, WE'RE ALL COUNTING ON YOU
	void DriveControl() { //Drives la wheels of the robot
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

		if (flightStick->GetRawButton(strafeButtonChannel)){ //Set drive to strafe mode
			driveMode = 0;
		}
		if (flightStick->GetRawButton(arcadeButtonChannel)){ //Set drive to arcade mode
			driveMode = 1;
		}
		if (flightStick->GetRawButton(fieldButtonChannel)){ //Set drive to field-centric mode
			driveMode = 2;
		}
		if (shootStick->GetRawButton(gyroResetChannel)){ //Reset gyro with the trigger
			yawGyro->Reset();
		}
		flightX = flightX * flightThrottle;
		flightY = flightY * flightThrottle;
		flightZ = flightZ * flightThrottle;

		if(driveMode == 1){
			robotDrive->MecanumDrive_Cartesian(flightZ, flightY, flightX, 0);
			SmartDashboard::PutString("DriveMode", "Arcade");
		}
		else if(driveMode == 2){
			robotDrive->MecanumDrive_Cartesian(flightX, flightY, flightZ, yawGyro->GetAngle());
			SmartDashboard::PutString("DriveMode", "Field");

		}
		else{
			robotDrive->MecanumDrive_Cartesian(flightX, flightY, flightZ, 0);
			SmartDashboard::PutString("DriveMode", "Strafe");

		}


		SmartDashboard::PutNumber("GyroAngle", yawGyro->GetAngle());
	}

	void PrimeMotors(){
		bPrime = shootStick -> GetRawButton(9);
		SmartDashboard::PutNumber("Load Running", bLoadRunning);
		SmartDashboard::PutNumber("Prime Running", bPrimeRunning);
		SmartDashboard::PutBoolean("Shooter Loaded", shooterSwitch);

		if(bLoadRunning >= 0){ // bLoadRunning is -1 or 1 (1 means not running)
			if(bPrime == true){ // if we press the button
				if(bPrimeOn == false){ // if the button is not pressed last iteration
					bPrimeOn = true; // say button was pressed
					bPrimeRunning = -bPrimeRunning; // turn prime on or off
				}
			}
			else{
				bPrimeOn = false; // when you let go, say the button was let go of
			}

		}


		if(bPrimeRunning <= 0){
			motor2 -> Set(1); // turn it on
			motor3 -> Set(-1);
			flightStick -> SetRumble(Joystick::kRightRumble, 1);
		}
		else{
			motor2 -> Set(0); // turn it off
			motor3 -> Set(0);
			flightStick -> SetRumble(Joystick::kRightRumble, 0);
		}
	}

	void Load() { // See prime motors for logicy things
		bLoad = shootStick -> GetRawButton(11);
		if(bPrimeRunning >= 0){
			if (bLoad == true) {
				if (bLoadOn == false) { // boolean for whether button is "on" or not
					bLoadOn = true;
					bLoadRunning = -bLoadRunning; // boolean for whether motor is running or not
				}
			}
			else {
				bLoadOn = false;
			}
		}

		if (bLowGoalShoot == false && bHighGoalShoot == false){ // make sure that it is not trying to shoot
			if (bLoadRunning <= 0){
				motor1 -> Set(.8);
				flightStick -> SetRumble(Joystick::kLeftRumble, 1);
			}
			else{
				motor1 -> Set(0);
				flightStick -> SetRumble(Joystick::kLeftRumble, 0);
			}
		}
	}

	void HighGoal(){ // shoots for a time period -- is not a toggle
		bHighGoal = flightStick -> GetRawAxis(3);

		if(bLowGoalShoot == false && bLoadRunning >= 0){
			if(bHighGoal > .6){
				if(shooterCounter < 4){
					motor1 -> Set(-.5);
				}
				else{
					motor1 -> Set(.5);
				}
				shooterCounter++;
				bHighGoalShoot = true;
			}
			else{
				motor1 -> Set(0);
				bHighGoalShoot = false;
				shooterCounter = 0;
			}

		}
	}

	void LowGoal(){ // See HighGoal
				bLowGoal = flightStick -> GetRawAxis(2); // gets left trigger

				if(bHighGoalShoot == false && bLoadRunning >= 0){
					if(bLowGoal > .6){
						motor1 -> Set(-1);
						bLowGoalShoot = true;
					}
					else{
						motor1 -> Set(0);
						bLowGoalShoot = false;
					}

				}
			}

	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot)



