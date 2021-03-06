//As of 16.02.23.

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

	// Auto setup: arrays are for joystick motion
	float xValues[800] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.133538,0.138484,0.163214,0.207726,0.237402,0.301698,0.351156,0.390723,0.415453,0.425344,0.445128,0.45502,0.45502,0.45502,0.45502,0.45502,0.45502,0.45502,0.45502,0.45502,0.440182,0.361048,0.242347,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-0.181567,-0.201196,-0.211011,-0.211011,-0.211011,-0.211011,-0.206104,-0.191382,-0.166846,-0.161938,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.138484,0.148376,0.148376,0.153322,0.178051,0.182997,0.182997,0.182997,0.182997,0.182997,0.182997,0.187943,0.187943,0.187943,0.187943,0.182997,0.14343,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-0.166846,-0.225732,-0.260083,-0.289526,-0.299341,-0.299341,-0.299341,-0.299341,-0.299341,-0.299341,-0.299341,-0.299341,-0.299341,-0.299341,-0.299341,-0.299341,-0.309155,-0.348413,-0.368042,-0.368042,-0.368042,-0.368042,-0.368042,-0.368042,-0.368042,-0.368042,-0.314063,-0.240454,-0.17666,-0.166846,-0.161938,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-0.127588,-0.147217,-0.147217,-0.147217,-0.161938,-0.181567,-0.245361,-0.294434,-0.309155,-0.309155,-0.309155,-0.309155,-0.309155,-0.309155,-0.309155,-0.309155,-0.309155,-0.309155,-0.309155,-0.309155,-0.309155,-0.309155,-0.294434,-0.279712,-0.260083,-0.240454,-0.235547,-0.17666,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-0.127588,-0.147217,-0.161938,-0.166846,-0.171753,-0.17666,-0.17666,-0.181567,-0.181567,-0.181567,-0.181567,-0.181567,-0.181567,-0.181567,-0.161938,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.14343,0.178051,0.242347,0.301698,0.365994,0.415453,0.425344,0.425344,0.425344,0.425344,0.425344,0.425344,0.425344,0.425344,0.43029,0.43029,0.43029,0.43029,0.43029,0.43029,0.395669,0.242347 };
	float yValues[800] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-0.065096,-0.10908,-0.147785,-0.158342,-0.16186,-0.16186,-0.16186,-0.16186,-0.16186,-0.16186,-0.16186,-0.16186,-0.16186,-0.16186,-0.16186,-0.16186,-0.16186,-0.16186,-0.16186,-0.16186,-0.16362,-0.177694,-0.195288,-0.221678,-0.225197,-0.225197,-0.225197,-0.225197,-0.225197,-0.225197,-0.225197,-0.225197,-0.225197,-0.225197,-0.225197,-0.225197,-0.225197,-0.225197,-0.225197,-0.225197,-0.225197,-0.223438,-0.207603,-0.207603,-0.207603,-0.207603,-0.207603,-0.207603,-0.207603,-0.207603,-0.207603,-0.202325,-0.172416,-0.137229,-0.112598,-0.0879675,-0.0774114,-0.0738927,-0.0738927,-0.0738927,-0.070374,-0.0686147,-0.0686147,-0.0686147,-0.0686147,-0.0686147,-0.0686147,-0.0686147,-0.0686147,-0.0686147,-0.0686147,-0.0686147,-0.0686147,-0.0686147,-0.0686147,-0.0686147,-0.0686147,-0.0798844,-0.103088,-0.130598,-0.146752,-0.157776,-0.172933,-0.179823,-0.185335,-0.185335,-0.188091,-0.188091,-0.193602,-0.196358,-0.199114,-0.204626,-0.204626,-0.210138,-0.212894,-0.217028,-0.217028,-0.219783,-0.222539,-0.225295,-0.230807,-0.233563,-0.236319,-0.236319,-0.241831,-0.244587,-0.247343,-0.256176,-0.291511,-0.334129,-0.374766,-0.38798,-0.38798,-0.392163,-0.400513,-0.400513,-0.404663,-0.374097,-0.374097,-0.369238,-0.369238,-0.369238,-0.369238,-0.372949,-0.372949,-0.372949,-0.368042,-0.348413,-0.328784,-0.314063,-0.309155,-0.279712,-0.279712,-0.220825,-0.186475,-0.147217,-0.132495,-0.132495,-0.132495,-0.132495,-0.132495,-0.132495,-0.127588,-0.191382,-0.191382,-0.269897,-0.31897,-0.343506,-0.35332,-0.368042,-0.377856,-0.377856,-0.377856,-0.377856,-0.377856,-0.377856,-0.377856,-0.382764,-0.382764,-0.382764,-0.382764,-0.382764,-0.382764,-0.382764,-0.382764,-0.382764,-0.382764,-0.382764,-0.382764,-0.382764,-0.392578,-0.417114,-0.426929,-0.451465,-0.456372,-0.466187,-0.466187,-0.466187,-0.466187,-0.466187,-0.466187,-0.466187,-0.466187,-0.466187,-0.466187,-0.466187,-0.466187,-0.466187,-0.466187,-0.466187,-0.466187,-0.466187,-0.466187,-0.466187,-0.466187,-0.466187,-0.466187,-0.466187,-0.451465,-0.436743,-0.426929,-0.422021,-0.387671,-0.348413,-0.314063,-0.250269,-0.181567,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.197835,0.281914,0.346211,0.380832,0.425344,0.459966,0.459966,0.464911,0.464911,0.464911,0.464911,0.450074,0.43029,0.420399,0.420399,0.420399,0.420399,0.420399,0.420399,0.420399,0.420399,0.420399,0.420399,0.420399,0.420399,0.420399,0.420399,0.420399,0.420399,0.420399,0.420399,0.380832,0.321481,0.247293,0.178051,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.14343,0.207726,0.267077,0.291806,0.326427,0.326427,0.326427,0.326427,0.326427,0.291806,0.222564,0.182997,0.182997,0.182997,0.178051,0.178051,0.178051,0.178051,0.202781,0.247293,0.262131,0.262131,0.262131,0.262131,0.262131,0.262131,0.222564,0.168159,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.138484,0.14343,0.148376,0.14343,0.14343,0.14343,0.14343,0.14343,0.14343,0.14343,0.148376,0.178051,0.178051,0.178051,0.178051,0.14343,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-0.166846,-0.181567,-0.181567,-0.186475,-0.186475,-0.186475,-0.186475,-0.186475,-0.186475,-0.186475,-0.186475,-0.186475,-0.186475,-0.186475,-0.191382,-0.211011,-0.220825,-0.235547,-0.250269,-0.255176,-0.260083,-0.240454,-0.186475,0,0,0,0,0,0,0,0,0,0,0,0,0.14343,0.182997,0.22751,0.257185,0.267077,0.267077,0.267077,0.267077,0.267077,0.267077,0.267077,0.267077,0.222564,0.222564,0,0,0,0,0,0,0,0,0,0,0,0,0.153322,0.237402,0.316535,0.351156,0.356102,0.356102,0.356102,0.356102,0.356102,0.356102,0.356102,0.321481,0.257185,0.182997,0.128593,0.128593,0.128593,0.128593,0.128593,0.128593,0.14343,0.178051,0.178051,0.178051,0.178051,0.178051,0.182997,0.187943,0.187943,0.187943,0.187943,0,0,0,0,0,0,0,0,0,0,0,0,0,-0.127588,-0.127588,-0.127588,-0.132495,-0.171753,-0.17666,-0.206104,-0.235547,-0.235547,-0.240454,-0.240454,-0.240454,-0.240454,-0.240454,-0.240454,-0.240454,-0.240454,-0.26499,-0.289526,-0.314063,-0.338599,-0.343506,-0.343506,-0.343506,-0.343506,-0.343506,-0.343506,-0.343506,-0.343506,-0.343506,-0.343506,-0.314063,-0.191382,0,0,0,0,0,0,0,0,0,0.148376,0.207726,0.281914,0.346211,0.385778,0.425344,0.43029,0.43029,0.43029,0.43029,0.43029,0.43029,0.380832,0.331373,0.247293,0.182997,0.133538,0,0,0,0,0,0,0,0,0.207726,0.321481,0.385778,0.459966,0.479749,0.494587,0.494587,0.51437,0.519316,0.519316,0.519316,0.519316,0.519316,0.519316,0.519316,0.519316,0.499532,0.479749,0.445128,0.37094,0.336319,0.257185,0.222564,0.187943,0.182997,0.182997,0.168159,0.158268,0.153322,0.153322,0.148376,0.148376,0.14343,0.14343,0.14343,0.168159,0.187943,0.242347,0.267077,0.331373,0.346211,0.351156,0.351156,0.351156,0.351156,0.351156,0.351156,0.351156,0.351156,0.351156,0.351156,0.351156,0.351156,0.351156,0.28686,0.237402,0};
	float zValues[800];
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
	ADXRS450_Gyro *yawGyro;			// How does the Gyro interface with the code? No channel #s are included

	// Camera setup
	IMAQdxSession session;
	Image *frame;
	IMAQdxError imaqError;

//	Distance sensor
	AnalogInput *distanceSensor;
	AnalogInput *getVcc;

//  Servo
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
		// create an image
		frame = imaqCreateImage(IMAQ_IMAGE_RGB, 0);
		//the camera name (ex "cam0") can be found through the roborio web interface
		imaqError = IMAQdxOpenCamera("cam0", IMAQdxCameraControlModeController, &session);
		if(imaqError != IMAQdxErrorSuccess) {
			DriverStation::ReportError("IMAQdxOpenCamera error: " + std::to_string((long)imaqError) + "\n");
		}
		imaqError = IMAQdxConfigureGrab(session);
		if(imaqError != IMAQdxErrorSuccess) {
			DriverStation::ReportError("IMAQdxConfigureGrab error: " + std::to_string((long)imaqError) + "\n");
		}
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
		if(autoSelected == autoNameCustom){
			if(autoCounter<800) {
				flightX = xValues[autoCounter];
				flightY = yValues[autoCounter];
				flightZ = zValues[autoCounter];
				std::cout << flightY << std::endl;
				//robotDrive->MecanumDrive_Cartesian(flightX, flightY, flightZ, 0);
				autoCounter++;
			}
		} else {
			//Default Auto goes here
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
		DriveControl();
		PrimeMotors();
		Load();
		HighGoal();
		LowGoal();
		ReadDistance();
		Camera();
		UpdateServo();
		PrimeBack();
	}

	void Camera() { // Camera.
		IMAQdxGrab(session, frame, true, NULL);
		if(imaqError != IMAQdxErrorSuccess) {
			DriverStation::ReportError("IMAQdxGrab error: " + std::to_string((long)imaqError) + "\n");
		} else {
			imaqDrawShapeOnImage(frame, frame, { 155, 260, 100, 100 }, DrawMode::IMAQ_DRAW_VALUE, ShapeMode::IMAQ_SHAPE_OVAL, 0.0f);
			imaqDrawShapeOnImage(frame, frame, { 180, 280, 50, 50}, DrawMode::IMAQ_DRAW_VALUE, ShapeMode::IMAQ_SHAPE_OVAL, 0.0f);
			imaqDrawLineOnImage(frame, frame, DrawMode::IMAQ_DRAW_VALUE, {320,165}, {320,315}, 0.0f);
			imaqDrawLineOnImage(frame, frame, DrawMode::IMAQ_DRAW_VALUE, {245,240}, {395,240}, 0.0f);
			imaqDrawShapeOnImage(frame, frame, { 187, 315, 6, 6}, DrawMode::IMAQ_PAINT_VALUE, ShapeMode::IMAQ_SHAPE_OVAL, 255.0f);
			CameraServer::GetInstance()->SetImage(frame);
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
		//servoLocation = shootStick -> GetPOV();
		if(shootStick -> GetRawButton(3) == 1){
			doorLift -> SetAngle(0);
		}
		if(shootStick -> GetRawButton(4) == 1){
			doorLift -> SetAngle(135);
		}
	}

	void PrimeBack(){

	}

	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot)





