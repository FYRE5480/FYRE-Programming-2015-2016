#include "WPILib.h"

//#define I2C_SLAVE_ADR 0x53 // ADXL345 I2C device address
#define I2C_SLAVE_ADR 0xA8 // from this thread: http://www.chiefdelphi.com/forums/showthread.php?t=132572
						  // We think this is the address of the Roborio. 168
//							https://wpilib.screenstepslive.com/s/3120/m/7912/l/175524-sending-data-from-the-crio-to-an-arduino
//#define DEVICE   0x53 // ADXL345 I2C device address
//#define DEVICE 8 // for Arduino??? Maybe???
//#define ADXL_REG 0x32 // first axis-acceleration-data register on the ADXL345
#define ADXL_REG    0x8 // I am trying a thing.
#define TO_READ     8 // num of bytes we are going to read each time (two bytes for each axis)
#define I2C_CHANNEL 2 // The I2C bus on the upper left-hand side is bus 2
//#define ADXL_CONFIG_PORT 0x2D // the configuration port of the ADXL345
typedef unsigned char byte;
int num = 0;
int holdLen = 0;
int arrayLen = 0;
bool isABool = 0;

class Robot: public IterativeRobot
{
	I2C *i2cChannel2; // I ADDED THISSSSSS;KLADFSJKDSAMKSL
	byte *anArray = new byte[1];
	uint8_t *holderPtr = new uint8_t[1];
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

	void RobotInit()
	{
		i2cChannel2 = new I2C(I2C::Port::kOnboard, I2C_SLAVE_ADR); // ;kadfslkmads;lkmdsa;lkmf;oewianm;oimeai;

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

//		CameraServer::GetInstance()->SetQuality(50);
//		CameraServer::GetInstance()->StartAutomaticCapture("cam0");
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
		ReadDistance(); //KUFGHLIAEFWJK;M;AEWLKMOMAEW;LKMAEW;LKMC;LKM
	}

	void ReadDistance() { // THIS IS MY FUNCTIONA;FLKDMS;LKM F;DLKMF;LAKDSM;FLKMAD;SLMF;oifkmlads;flkadsg;lkafdsm;km
		byte buff[TO_READ];
//		i2cChannel2->Read(ADXL_REG, TO_READ, buff);
//		SmartDashboard::PutNumber("i2c is", i2cChannel2->Read(ADXL_REG, TO_READ, buff));
//		i2cChannel2->Write(ADXL_REG, 1);
//		i2cChannel2->Read(ADXL_REG, TO_READ, buff);    // request 1 bytes from slave device #8
//		long n = i2cChannel2 -> Read();
//		i2cChannel2->Transaction(NULL, 0, anArray, TO_READ);
//		i2cChannel2->Read(ADXL_REG, 1, holderPtr);
//		i2cChannel2->ReadOnly(ADXL_REG, holderPtr);
		i2cChannel2->Write(ADXL_REG, 1);
//		holdLen = sizeof(holderPtr);
		SmartDashboard::PutNumber("Connected via i2c?", i2cChannel2->AddressOnly());
//		SmartDashboard::PutNumber("holdLen:", holdLen);
//		SmartDashboard::PutNumber("sizeof(anArray):", sizeof(anArray));
		SmartDashboard::PutNumber("Transaction return is a bool:", i2cChannel2->Transaction(NULL, 0, anArray, TO_READ));
//		SmartDashboard::PutNumber("i2c Transaction return:", anArray[0]);
//		SmartDashboard::PutNumber("i2c Read return:", holderPtr[0]);
//		num++; // if putting 0 doesn't work
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

	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot)







