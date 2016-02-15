//As of 16.02.13. Conglomerated robot code before the bot has been assembled. Has to be tested.

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
	float xValues[800];
	float yValues[800];
	float zValues[800];
	int autoCounter;

	// Shooting motor controllers and joystick and limit switch
	Jaguar *motor23;
	Talon *motor1; //Will be Talon: Victor for testing purposes only
	Joystick *shootStick;
	DigitalInput *shooterSwitch;

	// Various drivy things and gyro and switch
	CANTalon *frontLeftTalon, *rearLeftTalon, *frontRightTalon;
	Victor *rearRightTalon;
	RobotDrive *robotDrive;	// robot drive system
	Joystick *flightStick;	// Flightstick
	ADXRS450_Gyro *yawGyro;			// How does the Gyro interface with the code? No channel #s are included

	// Camera setup
//	IMAQdxSession session;
//	Image *frame;
//	IMAQdxError imaqError;

//	Distance sensor
	AnalogInput *distanceSensor;
	AnalogInput *getVcc;


	int frontLeftChannel	 = 1;	// Channel Declarations
	int rearLeftChannel	     = 55;
	int frontRightChannel	 = 2;
	int rearRightChannel	 = 0;

	int flightstickChannel	 = 0;
	int xboxChannel		     = 1;

	int strafeButtonChannel  = 8;
	int arcadeButtonChannel  = 10;
	int fieldButtonChannel	 = 12;

	int gyroResetChannel 	 = 1;

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
		frontRightTalon = new CANTalon(frontRightChannel);
		rearRightTalon = new Victor(rearRightChannel);
		frontRightTalon->SetInverted(true);
		rearRightTalon->SetInverted(true);
		yawGyro = new ADXRS450_Gyro();
		robotDrive = new RobotDrive(frontLeftTalon, rearLeftTalon, frontRightTalon, rearRightTalon);
		flightStick = new Joystick(flightstickChannel);
		robotDrive -> SetSafetyEnabled(false);

//		Shooter variable declarations MAKE SURE YOU PLUG THEM INTO THE RIGHT PORTS WENDY
		motor1 = new Talon(4); // loook above
		motor23 = new Jaguar(5);
		shootStick = new Joystick(1);
		shooterSwitch = new DigitalInput(0);

//		Distance sensor declarations
		distanceSensor = new AnalogInput(3);
		getVcc = new AnalogInput(2);

//		Camera stuff
		// create an image
//		frame = imaqCreateImage(IMAQ_IMAGE_RGB, 0);
//		//the camera name (ex "cam0") can be found through the roborio web interface
//		imaqError = IMAQdxOpenCamera("cam0", IMAQdxCameraControlModeController, &session);
//		if(imaqError != IMAQdxErrorSuccess) {
//			DriverStation::ReportError("IMAQdxOpenCamera error: " + std::to_string((long)imaqError) + "\n");
//		}
//		imaqError = IMAQdxConfigureGrab(session);
//		if(imaqError != IMAQdxErrorSuccess) {
//			DriverStation::ReportError("IMAQdxConfigureGrab error: " + std::to_string((long)imaqError) + "\n");
//		}
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
				robotDrive->MecanumDrive_Cartesian(flightZ, flightY, flightX);
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
	}

	void Camera() { // Camera.
//		IMAQdxGrab(session, frame, true, NULL);
//		if(imaqError != IMAQdxErrorSuccess) {
//			DriverStation::ReportError("IMAQdxGrab error: " + std::to_string((long)imaqError) + "\n");
//		} else {
//			imaqDrawShapeOnImage(frame, frame, { 190, 270, 100, 100 }, DrawMode::IMAQ_DRAW_VALUE, ShapeMode::IMAQ_SHAPE_OVAL, 0.0f);
//			imaqDrawShapeOnImage(frame, frame, { 215, 295, 50, 50}, DrawMode::IMAQ_DRAW_VALUE, ShapeMode::IMAQ_SHAPE_OVAL, 0.0f);
//			imaqDrawLineOnImage(frame, frame, DrawMode::IMAQ_DRAW_VALUE, {320,165}, {320,315}, 0.0f);
//			imaqDrawLineOnImage(frame, frame, DrawMode::IMAQ_DRAW_VALUE, {245,240}, {395,240}, 0.0f);
//			imaqDrawShapeOnImage(frame, frame, { 237, 317, 6, 6}, DrawMode::IMAQ_PAINT_VALUE, ShapeMode::IMAQ_SHAPE_OVAL, 255.0f);
//			CameraServer::GetInstance()->SetImage(frame);
//		}

	}

	void DriveControl() { //Drives la wheels of the robot
		flightX = flightStick->GetX(); //Pull joystick side motion for later use
		flightY = flightStick->GetY(); //Pull joystick forward motion for later use (forward is -, backwards is +)
		flightZ = flightStick->GetZ(); //Pull joystick twist motion for later use
		flightThrottle = ((((flightStick->GetThrottle() - 1)*-1)/2) * .8 + .2); //Pull throttle to modify drive variables
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
		if (flightStick->GetRawButton(gyroResetChannel)){ //Reset gyro with the trigger
			yawGyro->Reset();
		}
		flightX = flightX * flightThrottle;
		flightY = flightY * flightThrottle;
		flightZ = flightZ * flightThrottle;

		if(driveMode == 1){

				robotDrive->MecanumDrive_Cartesian(flightZ, flightY, flightX);
				SmartDashboard::PutString("DriveMode", "Arcade");
		}
		else if(driveMode == 2){
				robotDrive->MecanumDrive_Cartesian(flightX, flightY, flightZ, yawGyro->GetAngle());
				SmartDashboard::PutString("DriveMode", "Field");

		}
		else{
				robotDrive->MecanumDrive_Cartesian(flightX, flightY, flightZ);
				SmartDashboard::PutString("DriveMode", "Strafe");

		}
		SmartDashboard::PutNumber("GyroAngle", yawGyro->GetAngle());
	}

	void PrimeMotors(){
		bPrime = shootStick -> GetRawButton(1);
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
			motor23 -> Set(1); // turn it on
		}
		else{
			motor23 -> Set(0); // turn it off
		}
	}

	void Load() { // See prime motors for logicy things
		bLoad = shootStick -> GetRawButton(5);
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
				motor1 -> Set(1);
			}
			else{
				motor1 -> Set(0);
			}
		}
	}

	void HighGoal(){ // shoots for a time period -- is not a toggle
		bHighGoal = shootStick -> GetRawAxis(3);

		if(bLowGoalShoot == false){
			if(bHighGoal > .6){
				bHighGoalShoot = true;
			}
			if(bHighGoalShoot == true){
				if(shooterCounter <= 200){
					shooterCounter++;
					motor1 -> Set(1);
				}
				else{
					shooterCounter = 0;
					bHighGoalShoot = false;
					motor1 -> Set(0);
				}
			}
		}
	}

	void LowGoal(){ // See HighGoal
		bLowGoal = shootStick -> GetRawAxis(2); // gets left trigger

		if (bHighGoalShoot == false) { // only runs if not shooting for high goal
			if (bLowGoal > 0.6) { // converting the int from the trigger to a bool of whether we should shoot or not
				bLowGoalShoot = true;
			}
			if (bLowGoalShoot == true) {
				if (shooterCounter <= 400){
					shooterCounter++;
					motor1 -> Set(-1);
				}
				else{
					shooterCounter = 0;
					bLowGoalShoot = false;
					motor1 -> Set(0);
				}
			}
		}
	}

	void ReadDistance() {
		// y = 0.1319x-1.3456
		// y is actual distance, x is raw value
		convertedDist = 0.1319*(distanceSensor->GetValue())-1.3456;
		SmartDashboard::PutNumber("Actual distance:", convertedDist);
	}

	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot)



