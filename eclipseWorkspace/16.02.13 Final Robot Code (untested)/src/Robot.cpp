//As of 16.02.13. Conglomerated robot code before the bot has been assembled. Has to be tested.

#include "WPILib.h"

class Robot: public IterativeRobot
{
private:
	LiveWindow *lw = LiveWindow::GetInstance();
	SendableChooser *chooser;
	const std::string autoNameDefault = "Default";
	const std::string autoNameCustom = "My Auto";
	std::string autoSelected;

	Jaguar *motor23;
	Talon *motor1; //Will be Talon: Victor for testing purposes only
	Joystick *shootStick;

	CANTalon *frontLeftTalon, *rearLeftTalon, *frontRightTalon;
	Victor *rearRightTalon;
	RobotDrive *robotDrive;	// robot drive system
	Joystick *flightStick;	// Flightstick
	ADXRS450_Gyro *yawGyro;			// How does the Gyro interface with the code? No channel #s are included
	DigitalInput *shooterSwitch;

	IMAQdxSession session;
	Image *frame;
	IMAQdxError imaqError;

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

	int driveMode = 0; // Default Drive Mode is Strafe
	float flightX = 0;
	float flightY = 0;
	float flightZ = 0;
	float flightThrottle = 0;


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

	float convertedDist = 0;

	void RobotInit()
	{
		chooser = new SendableChooser();
		chooser->AddDefault(autoNameDefault, (void*)&autoNameDefault);
		chooser->AddObject(autoNameCustom, (void*)&autoNameCustom);
		SmartDashboard::PutData("Auto Modes", chooser);

		frontLeftTalon = new CANTalon(frontLeftChannel);
		rearLeftTalon = new CANTalon(rearLeftChannel);
		frontRightTalon = new CANTalon(frontRightChannel);
		rearRightTalon = new Victor(rearRightChannel);
		yawGyro = new ADXRS450_Gyro();
		robotDrive = new RobotDrive(frontLeftTalon, rearLeftTalon, frontRightTalon, rearRightTalon);
		flightStick = new Joystick(flightstickChannel);
		robotDrive -> SetSafetyEnabled(false);

		motor1 = new Talon(4); // loook above
		motor23 = new Jaguar(5) ;
		shootStick = new Joystick(1);
		shooterSwitch = new DigitalInput(0);

		distanceSensor = new AnalogInput(3);
		getVcc = new AnalogInput(2);


		//drivecam = new USBCamera("cam0", false);
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
		std::cout << "Auto selected: " << autoSelected << std::endl;

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
	}

	void TeleopInit()
	{

	}

	void TeleopPeriodic()
	{
		DriveControl();
		PrimeMotors();
		Load();
		HighGoal();
		LowGoal();
		ReadDistance();

		IMAQdxGrab(session, frame, true, NULL);
		if(imaqError != IMAQdxErrorSuccess) {
			DriverStation::ReportError("IMAQdxGrab error: " + std::to_string((long)imaqError) + "\n");
		} else {
			imaqDrawShapeOnImage(frame, frame, { 190, 270, 100, 100 }, DrawMode::IMAQ_DRAW_VALUE, ShapeMode::IMAQ_SHAPE_OVAL, 0.0f);
			imaqDrawShapeOnImage(frame, frame, { 215, 295, 50, 50}, DrawMode::IMAQ_DRAW_VALUE, ShapeMode::IMAQ_SHAPE_OVAL, 0.0f);
			imaqDrawLineOnImage(frame, frame, DrawMode::IMAQ_DRAW_VALUE, {320,165}, {320,315}, 0.0f);
			imaqDrawLineOnImage(frame, frame, DrawMode::IMAQ_DRAW_VALUE, {245,240}, {395,240}, 0.0f);
			imaqDrawShapeOnImage(frame, frame, { 237, 317, 6, 6}, DrawMode::IMAQ_PAINT_VALUE, ShapeMode::IMAQ_SHAPE_OVAL, 255.0f);
			CameraServer::GetInstance()->SetImage(frame);
		}

	}

	void DriveControl() { //Drives la wheels of the robot
		flightX = flightStick->GetX(); //Pull joystick side motion for later use
		flightY = flightStick->GetY(); //Pull joystick forward motion for later use (forward is -, backwards is +)
		flightZ = flightStick->GetZ(); //Pull joystick twist motion for later use
		flightThrottle = ((((flightStick->GetThrottle() - 1)*-1)/2) * .8 + .2); //Pull throttle to modify drive variables
																		//Throttle value is between .2 and 1.0
		SmartDashboard::PutNumber("AbsFlightX", fabs(flightX));

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
			//yawGyro->Reset();
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
		//shootSwitch = shooterSwitch -> Get();
		SmartDashboard::PutNumber("A Button: ", bPrime);
		SmartDashboard::PutNumber("Load Running", bLoadRunning);
		SmartDashboard::PutNumber("Prime Running", bPrimeRunning);
		SmartDashboard::PutNumber("bPrimeOn", bPrimeOn);
		SmartDashboard::PutNumber("LBumber: ", bLoad);
		SmartDashboard::PutNumber("bLoadOn", bLoadOn);
		SmartDashboard::PutBoolean("Shooter Loaded", shooterSwitch);
		//std::cout << "Shoot Switch: "  << shooterSwitch <<std::endl;

		std::cout << "Load Running: " << bLoadRunning << std::endl;
		std::cout << "Prime Running: " << bPrimeRunning << std::endl;
		if(bLoadRunning >= 0){
			if(bPrime == true){
				if(bPrimeOn == false){
					bPrimeOn = true;
					bPrimeRunning = -bPrimeRunning;
				}
			}
			else{
				bPrimeOn = false;
			}

		}

		if(bPrimeRunning <= 0){
			motor23 -> Set(1);
		}
		else{
			motor23 -> Set(0);
		}
	}

	void Load() {
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

		if (bLowGoalShoot == false && bHighGoalShoot == false){
			if (bLoadRunning <= 0){
				motor1 -> Set(1);
			}
			else{
				motor1 -> Set(0);
			}
		}
	}

	void HighGoal(){
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

	void LowGoal(){
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
//		SmartDashboard::PutNumber("the raw distance from the distance sensor is", distanceSensor->GetValue());
//		SmartDashboard::PutNumber("the voltage is", distanceSensor->GetAverageVoltage());
//		SmartDashboard::PutNumber("the vcc is", getVcc->GetAverageVoltage());
//		scalingFactor = getVcc->GetAverageVoltage()/float(512);
//		SmartDashboard::PutNumber("using scaling factor", scalingFactor * distanceSensor->GetValue());
//		convertedDist = float(512) * distanceSensor->GetValue() / getVcc->GetAverageVoltage();
//		SmartDashboard::PutNumber("the converted distance is", convertedDist/1000);

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



