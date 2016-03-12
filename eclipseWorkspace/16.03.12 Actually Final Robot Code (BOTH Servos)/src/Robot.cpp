// As of 16.03.12, I've added both camera servos in. I wish I could test this thing.
// Line 214: Update with shooting angle. Line 150, 151: Update with port numbers.

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

	// Auto setup
	int autoCounter;

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

	// Camera setup
	Image *frame;
	USBCamera *camera1;
	USBCamera *camera2;

	// MY SUPER AWESOME DRIVING STRAIGHT AUTO CODE
	float Kp = 0.03;
	// This is the constant--make it larger to make the gyro correction faster
	// and smaller to make the correction slower.
	int driveAngle;

//	Distance sensor
	AnalogInput *distanceSensor;
	AnalogInput *getVcc;

//  Door servo
	Servo *doorLift;

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

//	Distance variable
	float convertedDist = 0;

//  Camera Variables
	int camtype = 1;
	bool camswitch = false;
	bool bSeven = false;
	int width = 320;
	int height = 240;

	// Camera servos
	Servo *frontCamServo;
	Servo *backCamServo;
	float frontAngle;
	float backAngle;

	//	Joystick *shootStick;
	float shooterY;

	void RobotInit()
	{
//		Auto chooser
		chooser = new SendableChooser();
		chooser->AddDefault(autoNameDefault, (void*)&autoNameDefault);
		chooser->AddObject(autoNameCustom, (void*)&autoNameCustom);
		SmartDashboard::PutData("Auto Modes", chooser);

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

//		Distance sensor declarations
		distanceSensor = new AnalogInput(0);
		getVcc = new AnalogInput(2);

//		Servo
		doorLift = new Servo(9);

//		Camera stuff
		camera1 = new USBCamera("cam0", false);
		camera2 = new USBCamera("cam1", false);
		camera1->OpenCamera();
		camera2->OpenCamera();
		camera1->StartCapture();
		camera2->StartCapture();

		//Camera servos
		frontCamServo = new Servo(10); // WHAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAT
		backCamServo = new Servo(11); // WHAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAT
	}

	void AutonomousInit()
		{
			autoSelected = *((std::string*)chooser->GetSelected());
			std::string autoSelected = SmartDashboard::GetString("Auto Selector", autoNameDefault);
			std::cout << "Auto selected: " << autoSelected << std::endl;

			autoCounter = 0;

			if(autoSelected == autoNameCustom){
				//Custom Auto goes here
			} else {
				//Default Auto goes here
			}

		}

		void AutonomousPeriodic()
		{
			yawGyro -> Reset();
			if(autoCounter<800) {
				driveAngle = yawGyro -> GetAngle();
				robotDrive->MecanumDrive_Cartesian(0, 1, -driveAngle*Kp, 0); // has params X, Y, and Z (which I think is angle)
				autoCounter++;
				Wait(0.004);
			}
		}


	void TeleopInit()
	{
		//Shooter variables
		bPrime = false;
		bPrimeOn = false;
		bPrimeRunning = 1;
		bLoad = false;
		bLoadOn = false;
		bLoadRunning = 1;
		bHighGoalShoot = false;
		bHighGoal = 0;
		bLowGoalShoot = false;
		bLowGoal = 0;
		shooterCounter = 0;
		shootSwitch = 0;
	}

	void TeleopPeriodic()
	{
		Camera();
		DriveControl();
		PrimeMotors();
		Load();
		HighGoal();
		LowGoal();
		ReadDistance();
		UpdateServo();
	}

	void Camera() { // Camera. Also updates camera servos.
		// forces default angle for shooting
		if(shootStick->GetRawButton(12)) {
			frontAngle = 60; // UPDATE WITH REAL VALUE
		}
		else {
			// Get joystick Y axis
			shooterY = shootStick->GetY();
			// add joystick axis to front and back angle
			frontAngle += shooterY; // Scaling is most likely needed. I don't know what the servo angle goes to/from, or what the shooter goes to/from
			backAngle += shooterY;
		}

		camswitch = flightStick -> GetRawButton(7);
		if(camtype == 1){
			frontCamServo -> SetAngle(frontAngle);
			camera1 -> GetImage(frame);
			imaqDrawShapeOnImage(frame, frame, { 70, 110, 100, 100 }, DrawMode::IMAQ_DRAW_VALUE, ShapeMode::IMAQ_SHAPE_OVAL, 0.0f);
			imaqDrawShapeOnImage(frame, frame, { 95, 135, 50, 50}, DrawMode::IMAQ_DRAW_VALUE, ShapeMode::IMAQ_SHAPE_OVAL, 0.0f);
			imaqDrawLineOnImage(frame, frame, DrawMode::IMAQ_DRAW_VALUE, {160,0}, {160,315}, 0.0f);
			imaqDrawLineOnImage(frame, frame, DrawMode::IMAQ_DRAW_VALUE, {0,120}, {395,120}, 0.0f);
			imaqDrawShapeOnImage(frame, frame, { 117, 157, 6, 6}, DrawMode::IMAQ_PAINT_VALUE, ShapeMode::IMAQ_SHAPE_OVAL, 255.0f);
		}
		else{
			backCamServo -> SetAngle(backAngle);
			camera2 -> GetImage(frame);
		}
		CameraServer::GetInstance()->SetImage(frame);
		if(camswitch == true){ // if we press the button
			if(bSeven == false){ // if the button is not pressed last iteration
				bSeven = true; // say button was pressed
				camtype = -camtype; // turn prime on or off
			}
		}
		else{
			bSeven = false; // when you let go, say the button was let go of
		}

	}

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


	void ReadDistance() {
		// y = 0.1309x + 0.0383
		// y is actual distance, x is raw value
		convertedDist = 0.1309*(distanceSensor->GetValue())+0.0383;
		SmartDashboard::PutNumber("Actual distance:", convertedDist);
	}

	void UpdateServo(){
		if(shootStick -> GetRawButton(3) == 1){
			doorLift -> SetAngle(0);
		}
		if(shootStick -> GetRawButton(4) == 1){
			doorLift -> SetAngle(135);
		}
	}

	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot)






